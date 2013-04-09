#php-apn#

php-apn is a PHP module, wrapping [libcapn](http://libcapn.org) C library to introduce simple yet powerful interface for sending push notifications on iOS and OS X (>= 10.8) devices from within your PHP code, making integration with Apple Push Notification Service as easy as possible.

Please visit [libcapn.org/php-apn](http://libcapn.org/php-apn) for more information

##LICENSE##

The extension is licensed under the [PHP 3.01](http://www.php.net/license/3_01.txt) license; see LICENSE file.

##REQUIREMENTS##

* capn library (http://libcapn.org)
* capn library headers (to build extension)
* PHP 5.3+ SDK (to build extension)

##TODO##

* Add feedback service support 

##EXAMPLE##

__Exapmle one:__

```php

// APNS contex
$apn = apn_init();
apn_set_certificate($apn, 'apns-dev-cert.pem');
apn_set_private_key($apn, 'apns-dev-key.pem');
apn_add_tokens($apn, array(
          'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX',
          'ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ',
          'YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY'
      )
);

// Notification Payload context
$payload = apn_payload_init();
apn_payload_set_body($payload, 'This push was sent using PHP && php-apn');
apn_payload_set_sound($payload, 'default');
apn_payload_set_badge($payload, 10);
apn_payload_add_custom_property($payload, 'my_string_property', 'string');
apn_payload_add_custom_property($payload, 'my_int_property', 20);

$error = NULL;
// Opening Apple Push Notification Service connection...
if(apn_connect($apn, APN_SANDBOX, $error)) {
    // and if it ok, try send push notification....
    if(!apn_send($apn, $payload, $error)) {
        echo 'Could not sent push notification: ' . $error;
    }
} else {
    echo 'Could not connected to Apple Push Notification Servece: ' . $error;
}

apn_close($apn);
apn_payload_free($payload);
apn_free($apn);
```

__Exapmle two:__ 

```php

// APNS contex
$apn = apn_init();
apn_set_array($apn, array(
      'certificate' => 'apns-dev-cert.pem',
      'private_key' => 'apns-dev-key.pem',
      'tokens' => array (
          'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX',
          'ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ',
          'YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY'
      )
  ));

// Notification Payload context
$payload = apn_payload_init();
apn_payload_set_array($payload, array(
      'body' => 'This push was sent using PHP && php-apn',
      'sound' => 'default',
      'badge' =>34
));
apn_payload_add_custom_property($payload, 'my_string_property', 'string');
apn_payload_add_custom_property($payload, 'my_int_property', 20);

$error = NULL;
// Opening Apple Push Notification Service connection...
if(apn_connect($apn, APN_SANDBOX, $error)) {
    // and if ok, try send push notification....
    if(!apn_send($apn, $payload, $error)) {
        echo 'Could not sent push notification: ' . $error;
    }
} else {
    echo 'Could not connected to Apple Push Notification Servece: ' . $error;
}

apn_close($apn);
apn_payload_free($payload);
apn_free($apn);
```
