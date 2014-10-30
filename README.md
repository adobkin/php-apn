# php-apn
[![](http://img.shields.io/badge/license-PHP 3.01-red.svg?style=flat)]()

php-apn is a PHP module, wrapping [libcapn](https://github.com/adobkin/libcapn) C library to introduce simple yet powerful interface for sending push notifications to iOS and OS X (>= 10.8) devices 
from within your PHP code, making integration with Apple Push Notification Service as easy as possible.

### LICENSE

The extension is licensed under the [PHP 3.01](http://www.php.net/license/3_01.txt) license; see LICENSE file.

### REQUIREMENTS

* __capn library 2.0 or later__ ([https://github.com/adobkin/libcapn](https://github.com/adobkin/libcapn/tree/experimental))
* PHP 5.3+ SDK
* libtool
* GCC or Clang (to build on Linux)
* Microsoft Visual Studio 2008 or later (to build on Windows)

### INSTALLATION

To install this module type the following:

- `cd php-apn`
- `phpize`
- `./configure`
- `make`
- `make install`

### Example

#### Send notification

```php
<?php 

// APNS contex
$apn = apn_init();
apn_set_array($apn, array(
      'certificate' => 'apns_test_cert.pem',
      'private_key' => 'apns_test_key.pem',
      'private_key_pass' => '12345678',
      'mode' => APN_SANDBOX
  ));

// Notification Payload
$payload = apn_payload_init();
apn_payload_set_array($payload, array(
      'body' => 'This push was sent using PHP && php-apn',
      'sound' => 'default',
      'badge' => 100,
      'tokens' => array (
    	  'XXXXXXX',
          'YYYYYYY',
      ),
      'priority' => APN_NOTIFICATION_PRIORITY_DEFAULT
));
apn_payload_add_custom_property($payload, 'test', 56);

$error = NULL;
$errcode = 0;

// Opening Apple Push Notification Service connection...
if(apn_connect($apn, $error, $errcode)) {
    // and if ok, try send push notification....
    if(!apn_send($apn, $payload, $error, $errcode)) {
        echo 'Could not sent push notification: ' . $error;
    }
} else {
    echo 'Could not connected to Apple Push Notification Service: ' . $error;
}

apn_close($apn);
apn_payload_free($payload);
apn_free($apn);
```

### Changes

__2.0.0 - 1.0.3__
