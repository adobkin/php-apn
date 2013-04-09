dnl config.m4 for extension apush

PHP_ARG_WITH(apn, for apn support, [  --with-apn=[DIR]  Include Apple Push Notification Service support])

if test "$PHP_APN" != "no"; then
    AC_DEFINE(HAVE_CAPN,1,[whether to enable Apple Push Notification Service support])

    PHP_CONFIG="php-config"
    AC_MSG_CHECKING(for PHP 5.3.0 or greater)

    PHP_VER_NUM=`$PHP_CONFIG --vernum`
    if test "$PHP_VER_NUM" -ge 50300; then
      AC_MSG_RESULT($PHP_VER_NUM)
    else
      AC_MSG_ERROR(PHP version 5.3.0 or later is required)
    fi

    SEARCH_HEADERS_PATH="/usr/local /usr /opt/local"     
    SEARCH_HEADER_FILE="apn.h"  
    
    if test -r $PHP_APN/include/capn/$SEARCH_HEADER_FILE; then
        APN_DIR=$PHP_APN
    else
      AC_MSG_CHECKING([for $SEARCH_HEADER_FILE in default path])
      for i in $SEARCH_HEADERS_PATH; do
        if test -r $i/include/capn/$SEARCH_HEADER_FILE; then
           APN_DIR=$i
	    
           AC_MSG_RESULT(found in $i)
           break
        fi
      done
    fi
  
    if test -z "$APN_DIR"; then
      AC_MSG_RESULT([not found])
      AC_MSG_ERROR([libcapn is not found. Please visit to http://www.libcapn.org/php-apn for more information])
    fi

    APN_INCDIR=$APN_DIR/include/capn

    SEARCH_LIBS_PATH="/usr/local/lib /usr/lib /usr/lib64 /usr/local/lib64"
    SEARCH_LIB_FILE=libcapn.$SHLIB_SUFFIX_NAME
    if test -r $PHP_APN/capn/$SEARCH_LIB_FILE; then
        APN_LIBDIR=$PHP_APN
    else
      AC_MSG_CHECKING([for $SEARCH_LIB_FILE in default path])
      for i in $SEARCH_LIBS_PATH; do
        if test -r $i/capn/$SEARCH_LIB_FILE; then
           APN_LIBDIR=$i
           AC_MSG_RESULT(found in $i)
           break
        fi
      done
    fi

    if test -z "$APN_LIBDIR"; then
      AC_MSG_RESULT([not found])
      AC_MSG_ERROR([libcapn is not found. Please visit to http://www.libcapn.org/php-apn for more information])
    fi

    APN_LIBDIR=$APN_LIBDIR/capn

    PHP_ADD_INCLUDE($APN_INCDIR)
    PHP_ADD_LIBRARY_WITH_PATH(capn, $APN_LIBDIR, APN_SHARED_LIBADD)

    PHP_SUBST(APN_SHARED_LIBADD)
    PHP_NEW_EXTENSION(apn, php_apn.c, $ext_shared)
fi
