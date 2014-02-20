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

#ifndef PHP_APN_H
#define PHP_APN_H

#include "capn/apn.h"

extern zend_module_entry apn_module_entry;
#define phpext_apn_ptr &apn_module_entry

#ifdef ZTS
#include "TSRM.h"
#endif

#ifndef PHP_FE_END
#define PHP_FE_END {NULL, NULL, NULL,}
#endif

#define PHP_APN_VERSION "1.0.3"

#define LE_APN_NAME           "Apple Push Notification"
#define LE_APN_PAYLOAD_NAME   "Apple Push Notification Payload"

ZEND_BEGIN_MODULE_GLOBALS(apn)
    char *apn_certificate;
    char *apn_private_key;
    char *apn_private_key_pass;
ZEND_END_MODULE_GLOBALS(apn)

PHP_MINIT_FUNCTION(apn);
PHP_MSHUTDOWN_FUNCTION(apn);
PHP_RINIT_FUNCTION(apn);
PHP_RSHUTDOWN_FUNCTION(apn);
PHP_MINFO_FUNCTION(apn);

PHP_FUNCTION(apn_init);
PHP_FUNCTION(apn_free);
PHP_FUNCTION(apn_connect);
PHP_FUNCTION(apn_close);
PHP_FUNCTION(apn_set_certificate);
PHP_FUNCTION(apn_set_private_key);
PHP_FUNCTION(apn_set_mode);
PHP_FUNCTION(apn_add_token);
PHP_FUNCTION(apn_add_tokens);
PHP_FUNCTION(apn_set_array);

PHP_FUNCTION(apn_send);

PHP_FUNCTION(apn_payload_init);
PHP_FUNCTION(apn_payload_free);
PHP_FUNCTION(apn_payload_set_badge);
PHP_FUNCTION(apn_payload_set_body);
PHP_FUNCTION(apn_payload_set_sound);
PHP_FUNCTION(apn_payload_set_expiry);
PHP_FUNCTION(apn_payload_add_token);
PHP_FUNCTION(apn_payload_add_tokens);
PHP_FUNCTION(apn_payload_set_localized_key);
PHP_FUNCTION(apn_payload_set_launch_image);
PHP_FUNCTION(apn_payload_set_localized_action_key);
PHP_FUNCTION(apn_payload_set_array);
PHP_FUNCTION(apn_payload_add_custom_property);

PHP_FUNCTION(apn_feedback_connect);
PHP_FUNCTION(apn_feedback);

#ifdef ZTS
#define PHP_APN_G(v) TSRMG(apn_globals_id, zend_apn_globals *, v)
#else
#define PHP_APN_G(v) (apn_globals.v)
#endif

#endif	/* PHP_APN_H */
