/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Anton Dobkin <anton.dobkin@gmail.com>                           |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>

#include "php_apn.h"

static zval *__php_apn_payload_array(apn_payload_ctx_ref payload TSRMLS_DC);

ZEND_DECLARE_MODULE_GLOBALS(apn)

#define PHP_APN_FETCH_RESOURCE(__res, __res_id, __rsrc_type, __ret_value) \
    do { \
        __res = (apn_ctx_ref) zend_list_find(__res_id, &__rsrc_type); \
         if (NULL == __res || __rsrc_type != le_apn) {  \
             php_error_docref(NULL TSRMLS_CC, E_WARNING, "invalid resource provided"); \
             __ret_value; \
         } \
    } while(0);

#define PHP_APN_PAYLOAD_FETCH_RESOURCE(__res, __res_id, __rsrc_type, __ret_value) \
    do { \
        __res = (apn_payload_ctx_ref) zend_list_find(__res_id, &__rsrc_type); \
         if (NULL == __res || __rsrc_type != le_payload) {  \
             php_error_docref(NULL TSRMLS_CC, E_WARNING, "invalid resource provided"); \
             __ret_value; \
         } \
    } while(0);

#define PHP_APN_OPTION_CHECK_STRING(__option_name, __option_value, __operator) \
{ \
    if(Z_TYPE_PP(__option_value) != IS_STRING) { \
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "incorrect value type of `%s'. Expected string, got %s",  __option_name, __php_apn_var_type(*__option_value)); \
        __operator; \
    } \
    if(Z_TYPE_PP(__option_value) == IS_STRING && Z_STRLEN_PP(__option_value) == 0) { \
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "value of `%s' cannot be empty", __option_name);  \
        __operator; \
    } \
}

#define PHP_APN_ADD_ASSOC_STRING_OR_NULL_TO_ARRAY(array, key, value) \
do { \
       if(NULL == value) { \
           add_assoc_null(array, key); \
       } else { \
           add_assoc_string(array, key, value, 1); \
       }\
} while(0);

#define PHP_APN_RETURN_STRING_OR_NULL(__string) \
{\
    if(__string) { \
        RETURN_STRING(__string, 1); \
    }  \
    RETURN_NULL(); \
}

enum __php_apn_options_code {
    APN_OPTION_CERTIFIACE,
    APN_OPTION_PRIVATE_KEY,
    APN_OPTION_PRIVATE_KEY_PASS,
    APN_OPTION_TOKENS,
    APN_OPTION_MODE
};

enum __php_apn_payload_options_code {
    APN_PAYLOAD_OPTION_BADGE = 0x1,
    APN_PAYLOAD_OPTION_SOUND,
    APN_PAYLOAD_OPTION_BODY,
    APN_PAYLOAD_OPTION_LAUNCH_IMAGE,
    APN_PAYLOAD_OPTION_LOC_ACTION_KEY,
    APN_PAYLOAD_OPTION_LOC_KEY,
    APN_PAYLOAD_OPTION_LOC_KEY_ARGS,
    APN_PAYLOAD_OPTION_TOKENS,
    APN_PAYLOAD_OPTION_EXPIRY
};

typedef struct __php_apn_option {
    char *option;
    int code;
} __php_apn_option;

static __php_apn_option __php_apn_options[] = {
    {"certificate", APN_OPTION_CERTIFIACE},
    {"private_key", APN_OPTION_PRIVATE_KEY},
    {"private_key_pass", APN_OPTION_PRIVATE_KEY_PASS},
    {"tokens", APN_OPTION_TOKENS},
    {"mode", APN_OPTION_MODE}
};

static __php_apn_option __php_apn_payload_options[] = {
    {"badge", APN_PAYLOAD_OPTION_BADGE},
    {"sound", APN_PAYLOAD_OPTION_SOUND},
    {"body", APN_PAYLOAD_OPTION_BODY},
    {"launch_image", APN_PAYLOAD_OPTION_LAUNCH_IMAGE},
    {"localized_action_key", APN_PAYLOAD_OPTION_LOC_ACTION_KEY},
    {"localized_key", APN_PAYLOAD_OPTION_LOC_KEY},
    {"localized_key_args", APN_PAYLOAD_OPTION_LOC_KEY_ARGS},
    {"tokens", APN_PAYLOAD_OPTION_TOKENS},
    {"expiry", APN_PAYLOAD_OPTION_EXPIRY}
};

ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_connect, 0, 0, 1)
ZEND_ARG_INFO(0, apn)
ZEND_ARG_INFO(1, error)
ZEND_ARG_INFO(1, errcode)
ZEND_END_ARG_INFO()
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_feedback_connect, 0, 0, 1)
ZEND_ARG_INFO(0, apn)
ZEND_ARG_INFO(1, error)
ZEND_ARG_INFO(1, errcode)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_send, 0, 0, 2)
ZEND_ARG_INFO(0, apn)
ZEND_ARG_INFO(0, payload)
ZEND_ARG_INFO(1, error)
ZEND_ARG_INFO(1, errcode)
ZEND_END_ARG_INFO()
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_close, 0, 0, 1)
ZEND_ARG_INFO(0, apn)
ZEND_END_ARG_INFO()
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_free, 0, 0, 1)
ZEND_ARG_INFO(0, apn)
ZEND_END_ARG_INFO()
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_set_certificate, 0, 0, 2)
ZEND_ARG_INFO(0, apn)
ZEND_ARG_INFO(0, certificate)
ZEND_END_ARG_INFO()
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_set_mode, 0, 0, 2)
ZEND_ARG_INFO(0, apn)
ZEND_ARG_INFO(0, mode)
ZEND_END_ARG_INFO()
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_set_private_key, 0, 0, 2)
ZEND_ARG_INFO(0, apn)
ZEND_ARG_INFO(0, private_key)
ZEND_ARG_INFO(0, private_key_pass)
ZEND_END_ARG_INFO()
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_add_token, 0, 0, 2)
ZEND_ARG_INFO(0, apn)
ZEND_ARG_INFO(0, token)
ZEND_END_ARG_INFO()
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_add_tokens, 0, 0, 2)
ZEND_ARG_INFO(0, apn)
ZEND_ARG_INFO(0, tokens)
ZEND_END_ARG_INFO()
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_set_array, 0, 0, 2)
ZEND_ARG_INFO(0, apn)
ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()
                
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_payload_free, 0, 0, 1)
ZEND_ARG_INFO(0, apn)
ZEND_END_ARG_INFO()
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_payload_set_badge, 0, 0, 2)
ZEND_ARG_INFO(0, payload)
ZEND_ARG_INFO(0, badge)
ZEND_END_ARG_INFO()
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_payload_set_body, 0, 0, 2)
ZEND_ARG_INFO(0, payload)
ZEND_ARG_INFO(0, body)
ZEND_END_ARG_INFO()
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_payload_set_sound, 0, 0, 2)
ZEND_ARG_INFO(0, payload)
ZEND_ARG_INFO(0, sound)
ZEND_END_ARG_INFO()  
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_payload_set_expiry, 0, 0, 2)
ZEND_ARG_INFO(0, payload)
ZEND_ARG_INFO(0, expiry)
ZEND_END_ARG_INFO()  
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_payload_set_localized_key, 0, 0, 3)
ZEND_ARG_INFO(0, payload)
ZEND_ARG_INFO(0, localized_key)
ZEND_ARG_INFO(0, localized_args)
ZEND_END_ARG_INFO()    
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_payload_set_localized_action_key, 0, 0, 2)
ZEND_ARG_INFO(0, payload)
ZEND_ARG_INFO(0, localized_action_key)
ZEND_END_ARG_INFO() 
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_payload_set_launch_image, 0, 0, 2)
ZEND_ARG_INFO(0, payload)
ZEND_ARG_INFO(0, launch_image)
ZEND_END_ARG_INFO() 
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_payload_add_token, 0, 0, 2)
ZEND_ARG_INFO(0, payload)
ZEND_ARG_INFO(0, token)
ZEND_END_ARG_INFO()
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_payload_add_tokens, 0, 0, 2)
ZEND_ARG_INFO(0, payload)
ZEND_ARG_INFO(0, tokens)
ZEND_END_ARG_INFO()
        
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_payload_set_array, 0, 0, 2)
ZEND_ARG_INFO(0, payload)
ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO() 
                
ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_payload_add_custom_property, 0, 0, 2)
ZEND_ARG_INFO(0, payload)
ZEND_ARG_INFO(0, key)
ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO() 
        
 ZEND_BEGIN_ARG_INFO_EX(arginfo_apn_feedback, 0, 0, 1)
ZEND_ARG_INFO(0, apn)
ZEND_ARG_INFO(1, error)
ZEND_ARG_INFO(1, errcode)
ZEND_END_ARG_INFO()
        
const zend_function_entry apn_functions[] = {
    PHP_FE(apn_init, NULL)
    PHP_FE(apn_connect, arginfo_apn_connect)
    PHP_FE(apn_close, arginfo_apn_close)
    PHP_FE(apn_free, arginfo_apn_free)
    PHP_FE(apn_set_certificate, arginfo_apn_set_certificate)
    PHP_FE(apn_set_private_key, arginfo_apn_set_private_key)
    PHP_FE(apn_set_mode, arginfo_apn_set_mode)
    PHP_FE(apn_add_token, arginfo_apn_add_token)
    PHP_FE(apn_add_tokens, arginfo_apn_add_tokens)
    PHP_FE(apn_set_array, arginfo_apn_set_array)
    PHP_FE(apn_send, arginfo_apn_send)
    PHP_FE(apn_payload_init, NULL)
    PHP_FE(apn_payload_free, arginfo_apn_payload_free)
    PHP_FE(apn_payload_add_token, arginfo_apn_payload_add_token)
    PHP_FE(apn_payload_add_tokens, arginfo_apn_payload_add_tokens)
    PHP_FE(apn_payload_set_badge, arginfo_apn_payload_set_badge)
    PHP_FE(apn_payload_add_custom_property, arginfo_apn_payload_add_custom_property)
    PHP_FE(apn_payload_set_expiry, arginfo_apn_payload_set_expiry)
    PHP_FE(apn_payload_set_body, arginfo_apn_payload_set_body)
    PHP_FE(apn_payload_set_sound, arginfo_apn_payload_set_sound)
    PHP_FE(apn_payload_set_localized_key, arginfo_apn_payload_set_localized_key)
    PHP_FE(apn_payload_set_launch_image, arginfo_apn_payload_set_launch_image)
    PHP_FE(apn_payload_set_localized_action_key, arginfo_apn_payload_set_localized_action_key)
    PHP_FE(apn_payload_set_array, arginfo_apn_payload_set_array)
    PHP_FE(apn_feedback_connect, arginfo_apn_feedback_connect)
    PHP_FE(apn_feedback, arginfo_apn_feedback)
    PHP_FE_END
};

zend_module_entry apn_module_entry = {
    STANDARD_MODULE_HEADER,
    "apn",
    apn_functions,
    PHP_MINIT(apn),
    PHP_MSHUTDOWN(apn),
    PHP_RINIT(apn),
    PHP_RSHUTDOWN(apn),
    PHP_MINFO(apn),
    PHP_APN_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_APN
ZEND_GET_MODULE(apn)
#endif

static int le_apn = -1;
static int le_payload = -1;

static const char *__php_apn_var_type(zval *var) {
    switch (Z_TYPE_P(var)) {
        case IS_BOOL:
            return "bool";
            break;
        case IS_LONG:
            return "long";
            break;
        case IS_DOUBLE:
            return "double";
            break;
        case IS_STRING:
            return "string";
            break;
        case IS_ARRAY:
            return "array";
            break;
        case IS_NULL:
            return "null";
            break;
        case IS_RESOURCE:
            return "resource";
            break;
        case IS_OBJECT:
            return "object";
            break;
        default:
            return "<unknown>";
    }
}

static int __php_apn_get_option_code(const char *option) {
    unsigned int i = 0;
    unsigned int array_size = sizeof(__php_apn_options)/sizeof(__php_apn_options[0]);
    for (i = 0; i < array_size; i++) {
        if (strcasecmp(option, __php_apn_options[i].option) == 0) {
            return __php_apn_options[i].code;
        }
    }
    return -1;
}

static int __php_apn_payload_get_option_code(const char *option) {
    unsigned int i = 0;
    unsigned int array_size = sizeof(__php_apn_payload_options)/sizeof(__php_apn_payload_options[0]);
    for (i = 0; i < array_size; i++) {
        if (strcasecmp(option, __php_apn_payload_options[i].option) == 0) {
            return __php_apn_payload_options[i].code;
        }
    }
    return -1;
}

static uint16_t __php_apn_phparray_to_carray(zval *php_array, char ***carray TSRMLS_DC) {
    HashPosition pointer_position = NULL;
    int array_key_type = 0;
    char *array_strkey = NULL;
    unsigned int array_strkey_len = 0;
    unsigned long array_numkey = 0;
    zval **array_item = NULL;
    char **array = NULL;
    uint16_t array_size = 0;
    char **tmp = NULL;

    *carray = NULL; 
    
    for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(php_array), &pointer_position);
            SUCCESS == zend_hash_has_more_elements_ex(Z_ARRVAL_P(php_array), &pointer_position); zend_hash_move_forward_ex(Z_ARRVAL_P(php_array), &pointer_position)) {
        array_key_type = zend_hash_get_current_key_ex(Z_ARRVAL_P(php_array), &array_strkey, &array_strkey_len, &array_numkey, 0, &pointer_position);
        
        if (HASH_KEY_IS_LONG != array_key_type) {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "key of array must be integer, got string");
            continue;
        }
                  
        if (SUCCESS == zend_hash_get_current_data_ex(Z_ARRVAL_P(php_array), (void**) &array_item, &pointer_position)) {
            if (Z_TYPE_PP(array_item) != IS_STRING) {               
                continue;
            }                        
            tmp = (char **) erealloc(array, sizeof(char *) + 1);
            if(!tmp) {
                return 0;
            }
            array = tmp;             
            array[array_size] = estrdup(Z_STRVAL_PP(array_item));            
            array_size++;
        }
    }
     
    if(array_size > 0) {       
        *carray = array;
    }
    
    return array_size;
}

static void __php_apn_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC) {
    apn_ctx_ref apn_ctx = (apn_ctx_ref) rsrc->ptr;
    if (apn_ctx) {
        apn_free(&apn_ctx);
    }
}

static void __php_apn_payload_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC) {
    apn_payload_ctx_ref apn_payload_ctx = (apn_payload_ctx_ref) rsrc->ptr;
    if (apn_payload_ctx) {
        apn_payload_free(&apn_payload_ctx);
    }
}

static void __php_apn_globals_init(zend_apn_globals *apn_globals) {
    apn_globals->apn_certificate = NULL;
    apn_globals->apn_private_key = NULL;
    apn_globals->apn_private_key_pass = NULL;
}

PHP_INI_BEGIN()
STD_PHP_INI_ENTRY("apn.certificate", "", PHP_INI_ALL, OnUpdateString, apn_certificate, zend_apn_globals, apn_globals)
STD_PHP_INI_ENTRY("apn.private_key", "", PHP_INI_ALL, OnUpdateLong, apn_private_key, zend_apn_globals, apn_globals)
STD_PHP_INI_ENTRY("apn.private_key_pass", "", PHP_INI_ALL, OnUpdateLong, apn_private_key_pass, zend_apn_globals, apn_globals)
PHP_INI_END()

PHP_MINIT_FUNCTION(apn) {
    REGISTER_LONG_CONSTANT("APN_PRODUCTION", APN_MODE_PRODUCTION, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("APN_SANDBOX", APN_MODE_SANDBOX, CONST_CS | CONST_PERSISTENT);
    
    le_payload = zend_register_list_destructors_ex(__php_apn_payload_dtor, NULL, LE_APN_PAYLOAD_NAME, module_number);
    le_apn = zend_register_list_destructors_ex(__php_apn_dtor, NULL, LE_APN_NAME, module_number); 

    ZEND_INIT_MODULE_GLOBALS(apn, __php_apn_globals_init, NULL);
    REGISTER_INI_ENTRIES();
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(apn) {
    return SUCCESS;
}

PHP_RINIT_FUNCTION(apn) {
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(apn) {
    return SUCCESS;
}

PHP_MINFO_FUNCTION(apn) {
    php_info_print_table_start();
    php_info_print_table_row(2, "Apple Push Notification Service support", "enabled");
    php_info_print_table_row(2, "Extension Version", PHP_APN_VERSION);
    php_info_print_table_row(2, "Version of libcapn", apn_version_string());
    php_info_print_table_end();
    DISPLAY_INI_ENTRIES();
}

PHP_FUNCTION(apn_init) {
    apn_ctx_ref apn_ctx = NULL;
    apn_error_ref error = NULL;

    if(ZEND_NUM_ARGS() != 0){
        WRONG_PARAM_COUNT;
    }

    if (apn_init(&apn_ctx, NULL, NULL, NULL, &error)) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "unable to initialize: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));
        apn_error_free(&error);
        RETURN_NULL();
    }

    ZEND_REGISTER_RESOURCE(return_value, apn_ctx, le_apn);
}

PHP_FUNCTION(apn_connect) {
    zval *res = NULL;
    zval *ref_error = NULL;
    int rsrc_type = 0;
    apn_error_ref error = NULL;
    apn_ctx_ref apn_ctx = NULL;
    zval *ref_errcode = NULL;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r|zz", &res, &ref_error, &ref_errcode)) {
        return;
    }

    PHP_APN_FETCH_RESOURCE(apn_ctx, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);

    if(ref_error){
        zval_dtor(ref_error); 
    }
    
    if(ref_errcode) {
        zval_dtor(ref_errcode);
    }
    
    if(apn_connect(apn_ctx, &error)) {        
        ZVAL_STRING(ref_error, apn_error_message(error), 1);
        ZVAL_LONG(ref_errcode, APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));
        apn_error_free(&error);
        RETURN_FALSE
    }  

    RETURN_TRUE
}

PHP_FUNCTION(apn_close) {
    apn_ctx_ref apn_ctx = NULL;
    zval *res = NULL;
    int rsrc_type = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &res)) {
        return;
    }

    PHP_APN_FETCH_RESOURCE(apn_ctx, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);
    
    apn_close(apn_ctx);
}

PHP_FUNCTION(apn_free) {
    int rsrc_type = 0;
    apn_ctx_ref apn_ctx = NULL;
    zval *res = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &res) == FAILURE) {
        return;
    }

    PHP_APN_FETCH_RESOURCE(apn_ctx, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);

    if (FAILURE == zend_list_delete(Z_RESVAL_P(res))) {
        RETURN_FALSE
    }

    RETURN_TRUE
}

PHP_FUNCTION(apn_set_certificate) {
    char *value = NULL;
    int value_len = 0;
    int rsrc_type = 0;
    apn_ctx_ref apn_ctx = NULL;
    zval *res = NULL;
    apn_error_ref error = NULL;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &res, &value, &value_len)) {
        return;
    }

    PHP_APN_FETCH_RESOURCE(apn_ctx, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);

    if (value_len == 0) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "SSL certificate is not specified");
        RETURN_FALSE
    }
    
    if (apn_set_certificate(apn_ctx, value, &error)) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set SSL certificate: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));  
        apn_error_free(&error);
        RETURN_FALSE
    }
    RETURN_TRUE
}

PHP_FUNCTION(apn_set_mode) {
    int mode = 0;
    int rsrc_type = 0;
    apn_ctx_ref apn_ctx = NULL;
    zval *res = NULL;
    apn_error_ref error = NULL;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &res, &mode)) {
        return;
    }

    PHP_APN_FETCH_RESOURCE(apn_ctx, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);

    if (apn_set_mode(apn_ctx, mode, NULL)) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set mode: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));  
        apn_error_free(&error);
        RETURN_FALSE
    }
    RETURN_TRUE
}

PHP_FUNCTION(apn_set_private_key) {
    char *key = NULL;
    int key_len = 0;
    char *key_pass = NULL;
    int key_pass_len = 0;
    int rsrc_type = 0;
    apn_ctx_ref apn_ctx = NULL;
    zval *res = NULL;
    apn_error_ref error = NULL;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs|s", &res, &key, &key_len, &key_pass, &key_pass_len)) {
        return;
    }

    PHP_APN_FETCH_RESOURCE(apn_ctx, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);

    if (key_len == 0) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "private key is not specified");
        RETURN_FALSE
    }
    if (apn_set_private_key(apn_ctx, key, key_pass, NULL)) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set SSL private key: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));  
        apn_error_free(&error);
        RETURN_FALSE
    }
    RETURN_TRUE
}

static uint8_t __php_apn_add_token(apn_ctx_ref apn_ctx, const char *token, uint32_t token_length TSRMLS_DC) {
    apn_error_ref error = NULL;
    if (token_length == 0) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "device token is not specified");
        return 1;
    }
    if (apn_add_token(apn_ctx, token, &error)) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to add device token '%s': %s (%d)",  token, apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));  
        apn_error_free(&error);
        return 1;
    }
    
    return 0;
}

static uint8_t __php_apn_add_tokens(apn_ctx_ref apn_ctx TSRMLS_DC, zval *tokens) {
    HashPosition pointer_position = NULL;
    int array_key_type = 0;
    char *array_strkey = NULL;
    unsigned int array_strkey_len = 0;
    unsigned long array_numkey = 0;
    zval **array_item = NULL;
    
    if (Z_TYPE_P(tokens) != IS_ARRAY) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to add device tokens: incorrect type of second argumet. Excepted array, got %s", __php_apn_var_type(tokens));
        return 1;
    }

    for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(tokens), &pointer_position);
            SUCCESS == zend_hash_has_more_elements_ex(Z_ARRVAL_P(tokens), &pointer_position); zend_hash_move_forward_ex(Z_ARRVAL_P(tokens), &pointer_position)) {
        array_key_type = zend_hash_get_current_key_ex(Z_ARRVAL_P(tokens), &array_strkey, &array_strkey_len, &array_numkey, 0, &pointer_position);
        
        if (HASH_KEY_IS_LONG != array_key_type) {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to add device token: key of array must be integer, got string");
            return 1;
        }
        
        if (SUCCESS == zend_hash_get_current_data_ex(Z_ARRVAL_P(tokens), (void**) &array_item, &pointer_position)) {
            if (Z_TYPE_PP(array_item) != IS_STRING) {
                php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to add device token: device token must be string, got `%s'", __php_apn_var_type(*array_item));
                return 1;
            }
            if(__php_apn_add_token(apn_ctx, Z_STRVAL_PP(array_item), Z_STRLEN_PP(array_item) TSRMLS_CC)){
                return 1;
            }
        }
    }
    
    return 0;
}

PHP_FUNCTION(apn_add_token) {
    char *value = NULL;
    int value_len = 0;
    int rsrc_type = 0;
    apn_ctx_ref apn_ctx = NULL;
    zval *res = NULL;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &res, &value, &value_len)) {
        return;
    }

    PHP_APN_FETCH_RESOURCE(apn_ctx, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);
    
    if(__php_apn_add_token(apn_ctx, value, value_len TSRMLS_CC)) {
        RETURN_FALSE
    }
    
    RETURN_TRUE
}

PHP_FUNCTION(apn_add_tokens) {
    apn_ctx_ref apn_ctx = NULL;
    zval *res = NULL;
    zval *array = NULL;
    int rsrc_type = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &res, &array)) {
        return;
    }
    PHP_APN_FETCH_RESOURCE(apn_ctx, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);
   
    if(__php_apn_add_tokens(apn_ctx TSRMLS_CC, array)) {
        RETURN_FALSE
    }
    
    RETURN_TRUE
}

PHP_FUNCTION(apn_set_array) {
    int rsrc_type = 0;
    apn_ctx_ref apn_ctx = NULL;
    zval *res = NULL;
    HashPosition pointer_position = NULL;
    zval *array = NULL;
    int array_key_type = 0;
    char *array_strkey = NULL;    
    unsigned int array_strkey_len = 0;
    unsigned long array_numkey = 0;
    zval **array_item = NULL;
    apn_error_ref error = NULL;
    int flag = -1;
    char *private_key = NULL;
    char *private_key_pass = NULL;
    
    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &res, &array)) {
        return;
    }

    PHP_APN_FETCH_RESOURCE(apn_ctx, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);

    if (Z_TYPE_P(array) != IS_ARRAY) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "incorrect type of second argumet. Excepted array, got %s", __php_apn_var_type(array));
        RETURN_FALSE
    }
    
    for(zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(array), &pointer_position); 
        SUCCESS == zend_hash_has_more_elements_ex(Z_ARRVAL_P(array), &pointer_position); zend_hash_move_forward_ex(Z_ARRVAL_P(array), &pointer_position)) {
        array_key_type = zend_hash_get_current_key_ex(Z_ARRVAL_P(array), &array_strkey, &array_strkey_len, &array_numkey, 0, &pointer_position);
       
        if(array_key_type == HASH_KEY_IS_STRING) {
             flag = __php_apn_get_option_code(array_strkey);                
        } else {
            continue;
            // emit error??
        }
       
       if(SUCCESS == zend_hash_get_current_data_ex(Z_ARRVAL_P(array), (void**)&array_item, &pointer_position)){           
            switch(flag) { 
                case APN_OPTION_CERTIFIACE:
                    PHP_APN_OPTION_CHECK_STRING(array_strkey, array_item, continue);
                    if (apn_set_certificate(apn_ctx, Z_STRVAL_PP(array_item), &error)) {
                        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set certificate: %s (%d)",  apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));
                        apn_error_free(&error);
                        RETURN_FALSE
                    }
                    break;
                case APN_OPTION_PRIVATE_KEY:
                    PHP_APN_OPTION_CHECK_STRING(array_strkey, array_item, continue);
                    private_key = estrndup(Z_STRVAL_PP(array_item), Z_STRLEN_PP(array_item));
                    break;
                case APN_OPTION_PRIVATE_KEY_PASS:
                    PHP_APN_OPTION_CHECK_STRING(array_strkey, array_item, continue);
                    private_key_pass = estrndup(Z_STRVAL_PP(array_item), Z_STRLEN_PP(array_item));
                    break;
                case APN_OPTION_MODE:
                    if(Z_TYPE_PP(array_item) != IS_LONG) {
                        php_error_docref(NULL TSRMLS_CC, E_WARNING, "incorrect value type of `%s'. Expected numeric, got %s",  array_strkey, __php_apn_var_type(*array_item));     
                        RETURN_FALSE
                    } else if (apn_set_mode(apn_ctx, (uint8_t)Z_LVAL_PP(array_item), &error)) {
                        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set mode: %s (%d)",  apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));  
                        apn_error_free(&error);
                        RETURN_FALSE
                    }
                    break;
                    
                case APN_OPTION_TOKENS:
                    if(Z_TYPE_PP(array_item) == IS_ARRAY) {
                        if(__php_apn_add_tokens(apn_ctx TSRMLS_CC, *array_item)){
                            RETURN_FALSE
                        }
                    } else if(Z_TYPE_PP(array_item) == IS_STRING) {
                        if(__php_apn_add_token(apn_ctx, Z_STRVAL_PP(array_item), Z_STRLEN_PP(array_item) TSRMLS_CC)){
                            RETURN_FALSE
                        }
                    } else {
                        php_error_docref(NULL TSRMLS_CC, E_WARNING, "incorrect value type of `%s'. Expected string or array, got %s",  array_strkey, __php_apn_var_type(*array_item)); 
                        RETURN_FALSE
                    }
                    break;
                default:
                    php_error_docref(NULL TSRMLS_CC, E_WARNING, "unknown option: `%s'", array_strkey);
            }
       }
    }    
    
    if(private_key) {
        if (apn_set_private_key(apn_ctx, private_key, private_key_pass, &error)) {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set private key: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));
            apn_error_free(&error);
            RETURN_FALSE
        }
        if(private_key_pass) {
            efree(private_key_pass);
        }
        efree(private_key);
    }
    
    RETURN_TRUE
}

PHP_FUNCTION(apn_send) {
    int rsrc_type = 0;
    zval *res = NULL;
    apn_ctx_ref apn_ctx = NULL;
    zval *ref_error = NULL;
    zval *ref_errcode = NULL;
    apn_error_ref error = NULL;
    apn_payload_ctx_ref payload = NULL;
    zval *payload_res = NULL;
	
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rr|zzz", &res, &payload_res, &ref_error, &ref_errcode) == FAILURE) {
        return;
    }
    
    PHP_APN_FETCH_RESOURCE(apn_ctx, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);
    PHP_APN_PAYLOAD_FETCH_RESOURCE(payload, Z_RESVAL_P(payload_res), rsrc_type, RETURN_FALSE);
    
    if(ref_error){
        zval_dtor(ref_error); 
    }
    if(ref_errcode) {
        zval_dtor(ref_errcode);
    }
    
    if(apn_send(apn_ctx, payload, &error)) {        
        ZVAL_STRING(ref_error, apn_error_message(error), 1);    
        if(APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)) == APN_ERR_TOKEN_INVALID) {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid token: %s", apn_error_invalid_token(error));
        }
        ZVAL_LONG(ref_errcode, APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));
        RETURN_FALSE
    }           
    
    RETURN_TRUE;
}

/* Payload */

static uint8_t __php_apn_payload_add_token(apn_payload_ctx_ref payload_ctx, const char *token, uint32_t token_length TSRMLS_DC) {
    apn_error_ref error = NULL;
    if (token_length == 0) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "device token is not specified");
        return 1;
    }
    if (apn_payload_add_token(payload_ctx, token, &error)) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to add device token '%s': %s (%d)",  token, apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));  
        apn_error_free(&error);
        return 1;
    }
    
    return 0;
}

static uint8_t __php_apn_payload_add_tokens(apn_payload_ctx_ref payload_ctx TSRMLS_DC, zval *tokens) {
    HashPosition pointer_position = NULL;
    int array_key_type = 0;
    char *array_strkey = NULL;
    unsigned int array_strkey_len = 0;
    unsigned long array_numkey = 0;
    zval **array_item = NULL;
    
    if (Z_TYPE_P(tokens) != IS_ARRAY) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to add device tokens: incorrect type of second argumet. Excepted array, got %s", __php_apn_var_type(tokens));
        return 1;
    }

    for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(tokens), &pointer_position);
            SUCCESS == zend_hash_has_more_elements_ex(Z_ARRVAL_P(tokens), &pointer_position); zend_hash_move_forward_ex(Z_ARRVAL_P(tokens), &pointer_position)) {
        array_key_type = zend_hash_get_current_key_ex(Z_ARRVAL_P(tokens), &array_strkey, &array_strkey_len, &array_numkey, 0, &pointer_position);
        
        if (HASH_KEY_IS_LONG != array_key_type) {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to add device token: key of array must be integer, got string");
            return 1;
        }
        
        if (SUCCESS == zend_hash_get_current_data_ex(Z_ARRVAL_P(tokens), (void**) &array_item, &pointer_position)) {
            if (Z_TYPE_PP(array_item) != IS_STRING) {
                php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to add device token: device token must be string, got `%s'", __php_apn_var_type(*array_item));
                return 1;
            }
            if(__php_apn_payload_add_token(payload_ctx, Z_STRVAL_PP(array_item), Z_STRLEN_PP(array_item) TSRMLS_CC)) {
                return 1;
            }
        }
    }
    return 0;
}

PHP_FUNCTION(apn_payload_init) {
    apn_payload_ctx_ref payload = NULL;
    apn_error *error = NULL;

    if(ZEND_NUM_ARGS() != 0){
        WRONG_PARAM_COUNT;
    }
    
    if (apn_payload_init(&payload, &error)) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "unable to initialize: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));
        apn_error_free(&error);
        RETURN_NULL();
    }

    ZEND_REGISTER_RESOURCE(return_value, payload, le_payload);
}

PHP_FUNCTION(apn_payload_free) {
    int rsrc_type = 0;
    apn_payload_ctx_ref payload = NULL;
    zval *res = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &res) == FAILURE) {
        return;
    }

    PHP_APN_PAYLOAD_FETCH_RESOURCE(payload, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);

    if (FAILURE == zend_list_delete(Z_RESVAL_P(res))) {
        RETURN_FALSE
    }

    RETURN_TRUE
}

PHP_FUNCTION(apn_payload_set_badge) {
    long badge = 0;
    int rsrc_type = 0;
    apn_payload_ctx_ref payload = NULL;
    zval *res = NULL;
    apn_error_ref error = NULL;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &res, &badge)) {
        return;
    }

    PHP_APN_PAYLOAD_FETCH_RESOURCE(payload, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);

    if (apn_payload_set_badge(payload, (uint16_t)badge, &error)) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set badge: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));  
        apn_error_free(&error);
        RETURN_FALSE
    }
    RETURN_TRUE
}

PHP_FUNCTION(apn_payload_set_body) {
    char *value = NULL;
    int value_len = 0;
    int rsrc_type = 0;
    apn_payload_ctx_ref payload = NULL;
    zval *res = NULL;
    apn_error_ref error = NULL;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &res, &value, &value_len)) {
        return;
    }

    PHP_APN_PAYLOAD_FETCH_RESOURCE(payload, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);

    if (value_len == 0) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "body is not specified");
        RETURN_FALSE
    }
    if (apn_payload_set_body(payload, value, &error)) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set alert message: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));  
        apn_error_free(&error);
        RETURN_FALSE
    }
    RETURN_TRUE
}

PHP_FUNCTION(apn_payload_set_sound) {
    char *value = NULL;
    int value_len = 0;
    int rsrc_type = 0;
    apn_payload_ctx_ref payload = NULL;
    zval *res = NULL;
    apn_error_ref error = NULL;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &res, &value, &value_len)) {
        return;
    }

    PHP_APN_PAYLOAD_FETCH_RESOURCE(payload, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);

    if (value_len == 0) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "sound file name is not specified");
        RETURN_FALSE
    }
    if (apn_payload_set_sound(payload, value, &error)) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set sound: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));  
        apn_error_free(&error);
        RETURN_FALSE
    }

    RETURN_TRUE
}

PHP_FUNCTION(apn_payload_set_localized_key) {
    char *value = NULL;
    int value_len = 0;
    int rsrc_type = 0;
    apn_payload_ctx_ref payload = NULL;
    zval *res = NULL;
    zval *array = NULL;
    char **args = NULL;
    uint16_t args_count = 0;
    apn_error_ref error = NULL;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rsz", &res, &value, &value_len, &array)) {
        return;
    }

    PHP_APN_PAYLOAD_FETCH_RESOURCE(payload, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);

    if (value_len == 0) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "localization key is not set");
        RETURN_FALSE
    }
    
    if(Z_TYPE_P(array) != IS_ARRAY) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "incorrect value type of second argument. Expected array, got %s", __php_apn_var_type(array));        
        RETURN_FALSE
    }
    
    args_count = __php_apn_phparray_to_carray(array, &args TSRMLS_CC); 
    
    if (apn_payload_set_localized_key(payload, value, args, args_count, &error)) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set localized key: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));  
        apn_error_free(&error);
        RETURN_FALSE
    }

    RETURN_TRUE
}

PHP_FUNCTION(apn_payload_set_launch_image) {
    char *value = NULL;
    int value_len = 0;
    int rsrc_type = 0;
    apn_payload_ctx_ref payload = NULL;
    zval *res = NULL;
    apn_error_ref error = NULL;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &res, &value, &value_len)) {
        return;
    }

    PHP_APN_PAYLOAD_FETCH_RESOURCE(payload, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);

    if (value_len == 0) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "launch image file name is not set");
        RETURN_FALSE
    }

    if (apn_payload_set_launch_image(payload, value, &error)) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set launch image file name: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));  
        apn_error_free(&error);
        RETURN_FALSE
    }

    RETURN_TRUE
}

PHP_FUNCTION(apn_payload_set_localized_action_key) {
    char *value = NULL;
    int value_len = 0;
    int rsrc_type = 0;
    apn_payload_ctx_ref payload = NULL;
    zval *res = NULL;
    apn_error_ref error = NULL;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &res, &value, &value_len)) {
        return;
    }

    PHP_APN_PAYLOAD_FETCH_RESOURCE(payload, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);

    if (value_len == 0) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "localization key is not set");
        RETURN_FALSE
    }

    if (apn_payload_set_localized_action_key(payload, value, &error)) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set action key: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));  
        apn_error_free(&error);
        RETURN_FALSE
    }

    RETURN_TRUE
}

PHP_FUNCTION(apn_payload_add_custom_property) {
    int rsrc_type = 0;
    zval *res = NULL;
    apn_payload_ctx_ref payload = NULL;
    zval *value = NULL;
    char *key = NULL;
    int key_len = 0;
    apn_error_ref error = NULL;
    
    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rsz", &res, &key, &key_len, &value)) {
        return;
    }

    PHP_APN_PAYLOAD_FETCH_RESOURCE(payload, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);
    
    if (key_len == 0) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "property name is not set");
        RETURN_FALSE
    }
    
    switch (Z_TYPE_P(value)) {
        case IS_BOOL:
            apn_payload_add_custom_property_bool(payload, key, Z_BVAL_P(value), &error);
            break;
        case IS_LONG:
            apn_payload_add_custom_property_integer(payload, key, Z_LVAL_P(value), &error);
            break;
        case IS_DOUBLE:
            apn_payload_add_custom_property_double(payload, key, Z_DVAL_P(value), &error);
            break;
        case IS_STRING:
            apn_payload_add_custom_property_string(payload, key, Z_STRVAL_P(value), &error);
            break;
        case IS_NULL:
            apn_payload_add_custom_property_null(payload, key, &error);
            break;
        default:
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "unsupported type of property value");
            RETURN_FALSE
    }
    
    if(error) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set custom property: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));  
        apn_error_free(&error);
        RETURN_FALSE
    }

    RETURN_TRUE
}

PHP_FUNCTION(apn_payload_set_expiry) {
    int expiry = -1;
    int rsrc_type = 0;
    apn_payload_ctx_ref payload = NULL;
    zval *res = NULL;
    apn_error_ref error = NULL;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &res, &expiry)) {
        return;
    }

    PHP_APN_PAYLOAD_FETCH_RESOURCE(payload, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);

    if (apn_payload_set_expiry(payload, expiry, &error)) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set expiry: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));  
        apn_error_free(&error);
        RETURN_FALSE
    }

    RETURN_TRUE
}

PHP_FUNCTION(apn_payload_add_token) {
    char *value = NULL;
    int value_len = 0;
    int rsrc_type = 0;
    apn_payload_ctx_ref payload = NULL;
    zval *res = NULL;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &res, &value, &value_len)) {
        return;
    }

    PHP_APN_PAYLOAD_FETCH_RESOURCE(payload, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);
    
    if(__php_apn_payload_add_token(payload, value, value_len TSRMLS_CC)) {
        RETURN_FALSE
    }
    
    RETURN_TRUE
}

PHP_FUNCTION(apn_payload_add_tokens) {
    apn_payload_ctx_ref payload = NULL;
    zval *res = NULL;
    zval *array = NULL;
    int rsrc_type = 0;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &res, &array)) {
        return;
    }
    PHP_APN_PAYLOAD_FETCH_RESOURCE(payload, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);
   
    if(__php_apn_payload_add_tokens(payload TSRMLS_CC, array)) {
        RETURN_FALSE
    }
    
    RETURN_TRUE
}

PHP_FUNCTION(apn_payload_set_array) {
    int rsrc_type = 0;
    apn_payload_ctx_ref payload = NULL;
    zval *res = NULL;
    HashPosition pointer_position = NULL;
    zval *array = NULL;
    int array_key_type = 0;
    char *array_strkey = NULL;    
    unsigned int array_strkey_len = 0;
    unsigned long array_numkey = 0;
    zval **array_item = NULL;
    char *loc_key = NULL;
    int loc_key_length = 0;
    char **args = NULL;
    uint16_t args_count = 0;
    apn_error_ref error = NULL; 
    int flag = -1;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rz", &res, &array)) {
        return;
    }

    PHP_APN_PAYLOAD_FETCH_RESOURCE(payload, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);

    if (Z_TYPE_P(array) != IS_ARRAY) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "incorrect type of second argumet. Excepted array, got %s", __php_apn_var_type(array));
        RETURN_FALSE
    }
    
    for(zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(array), &pointer_position); 
        SUCCESS == zend_hash_has_more_elements_ex(Z_ARRVAL_P(array), &pointer_position); zend_hash_move_forward_ex(Z_ARRVAL_P(array), &pointer_position)) {
        array_key_type = zend_hash_get_current_key_ex(Z_ARRVAL_P(array), &array_strkey, &array_strkey_len, &array_numkey, 0, &pointer_position);
       
        if(array_key_type == HASH_KEY_IS_STRING) {
             flag = __php_apn_payload_get_option_code(array_strkey);                
        } else {
            continue;
            // emit error??
        }
       
       if(SUCCESS == zend_hash_get_current_data_ex(Z_ARRVAL_P(array), (void**)&array_item, &pointer_position)){                                           
            switch(flag) { 
                case APN_PAYLOAD_OPTION_EXPIRY:
                    if(Z_TYPE_PP(array_item) != IS_LONG) {
                        php_error_docref(NULL TSRMLS_CC, E_WARNING, "incorrect value type of `%s'. Expected numeric, got %s",  array_strkey, __php_apn_var_type(*array_item));     
                        RETURN_FALSE
                    } else if (apn_payload_set_expiry(payload, (uint32_t)Z_LVAL_PP(array_item), &error)) {
                        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set expiry: %s (%d)",  apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));  
                        apn_error_free(&error);
                        RETURN_FALSE
                    }
                    break;
                case APN_PAYLOAD_OPTION_BADGE:
                    if(Z_TYPE_PP(array_item) != IS_LONG) {
                        php_error_docref(NULL TSRMLS_CC, E_WARNING, "incorrect value type of `%s'. Expected numeric, got %s",  array_strkey, __php_apn_var_type(*array_item));     
                        RETURN_FALSE
                    } else if (apn_payload_set_badge(payload, (int32_t)Z_LVAL_PP(array_item), &error)) {
                        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set badge: %s (%d)",  apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));  
                        apn_error_free(&error);
                        RETURN_FALSE
                    }
                    break;
                case APN_PAYLOAD_OPTION_LAUNCH_IMAGE:
                    PHP_APN_OPTION_CHECK_STRING(array_strkey, array_item, continue);
                    if (apn_payload_set_launch_image(payload, Z_STRVAL_PP(array_item), &error)) {
                        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set launch image file name: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));
                        apn_error_free(&error);
                        RETURN_FALSE
                    }
                    break;
                case APN_PAYLOAD_OPTION_LOC_ACTION_KEY:
                    PHP_APN_OPTION_CHECK_STRING(array_strkey, array_item, continue);
                    if (apn_payload_set_localized_action_key(payload, Z_STRVAL_PP(array_item), &error)) {
                        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set localized action key: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));
                        apn_error_free(&error);
                        RETURN_FALSE
                    }
                    break;
                case APN_PAYLOAD_OPTION_SOUND:
                    PHP_APN_OPTION_CHECK_STRING(array_strkey, array_item, continue);
                    if (apn_payload_set_sound(payload, Z_STRVAL_PP(array_item), &error)) {
                        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set sound file name: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));
                        apn_error_free(&error);
                        RETURN_FALSE
                    }
                    break;
                case APN_PAYLOAD_OPTION_BODY:
                    PHP_APN_OPTION_CHECK_STRING(array_strkey, array_item, continue);
                    if (apn_payload_set_body(payload, Z_STRVAL_PP(array_item), &error)) {
                        php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set body: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));
                        apn_error_free(&error);
                        RETURN_FALSE
                    }
                    break;
                case APN_PAYLOAD_OPTION_LOC_KEY: 
                    PHP_APN_OPTION_CHECK_STRING(array_strkey, array_item, continue);
                    loc_key = Z_STRVAL_PP(array_item);
                    loc_key_length = Z_STRLEN_PP(array_item);                    
                break;
                case APN_PAYLOAD_OPTION_LOC_KEY_ARGS:
                    if (Z_TYPE_PP(array_item) == IS_ARRAY) {
                        args_count = __php_apn_phparray_to_carray(*array_item, &args TSRMLS_CC);
                    } else {
                        php_error_docref(NULL TSRMLS_CC, E_WARNING, "incorrect value type of `%s'. Expected array, got %s", array_strkey, __php_apn_var_type(*array_item));
                        RETURN_FALSE
                    }
                    break;
                case APN_PAYLOAD_OPTION_TOKENS:
                    if(Z_TYPE_PP(array_item) == IS_ARRAY) {
                        if(__php_apn_payload_add_tokens(payload TSRMLS_CC, *array_item)){
                            RETURN_FALSE
                        }
                    } else if(Z_TYPE_PP(array_item) == IS_STRING) {
                        if(__php_apn_payload_add_token(payload, Z_STRVAL_PP(array_item), Z_STRLEN_PP(array_item) TSRMLS_CC)){
                            RETURN_FALSE
                        }
                    } else {
                        php_error_docref(NULL TSRMLS_CC, E_WARNING, "incorrect value type of `%s'. Expected string or array, got %s",  array_strkey, __php_apn_var_type(*array_item)); 
                        RETURN_FALSE
                    }
                    break;
                default:
                    php_error_docref(NULL TSRMLS_CC, E_WARNING, "unknown option: `%s'", array_strkey);
            }
        }
    }

    if (loc_key && loc_key_length > 0) {
        if (apn_payload_set_localized_key(payload, loc_key, args, args_count, &error)) {
            php_error_docref(NULL TSRMLS_CC, E_WARNING, "failed to set localized key: %s (%d)", apn_error_message(error), APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));
            apn_error_free(&error);
            RETURN_FALSE
        }
    }

    RETURN_TRUE
}

PHP_FUNCTION(apn_feedback_connect) {
    zval *res = NULL;
    zval *ref_error = NULL;
    int rsrc_type = 0;
    apn_error_ref error = NULL;
    apn_ctx_ref apn_ctx = NULL;
    zval *ref_errcode = NULL;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r|zz", &res, &ref_error, &ref_errcode)) {
        return;
    }

    PHP_APN_FETCH_RESOURCE(apn_ctx, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);

    if(ref_error){
        zval_dtor(ref_error); 
    }
    
    if(ref_errcode) {
        zval_dtor(ref_errcode);
    }
    
    if(apn_feedback_connect(apn_ctx, &error)) {        
        ZVAL_STRING(ref_error, apn_error_message(error), 1);
        ZVAL_LONG(ref_errcode, APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));
        apn_error_free(&error);
        RETURN_FALSE
    }  

    RETURN_TRUE
}

PHP_FUNCTION(apn_feedback) {
    zval *res = NULL;
    zval *ref_error = NULL;
    int rsrc_type = 0;
    apn_error_ref error = NULL;
    char **tokens = NULL;
    uint32_t tokens_count = 0;
    uint32_t i = 0;
    apn_ctx_ref apn_ctx = NULL;
    zval *ref_errcode = NULL;
    zval *php_tokens = NULL;

    if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r|zz", &res, &ref_error, &ref_errcode)) {
        return;
    }

    PHP_APN_FETCH_RESOURCE(apn_ctx, Z_RESVAL_P(res), rsrc_type, RETURN_FALSE);

    if(ref_error){
        zval_dtor(ref_error); 
    }
    
    if(ref_errcode) {
        zval_dtor(ref_errcode);
    }
    
    if(apn_feedback(apn_ctx, &tokens, &tokens_count, &error)) {        
        ZVAL_STRING(ref_error, apn_error_message(error), 1);
        ZVAL_LONG(ref_errcode, APN_ERR_CODE_WITHOUT_CLASS(apn_error_code(error)));            
        apn_error_free(&error);
        RETURN_NULL()
    }  
    
    array_init(return_value);
    
    for(i = 0; i< tokens_count; i++) {
        add_index_string(return_value, i, tokens[i], 1);
    }
    
    apn_feedback_tokens_array_free(tokens, tokens_count);
}
