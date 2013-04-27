<?php
/**
 * Send Push Notification
 * @package php-apn
 */

// APNS contex
$apn = apn_init();
apn_set_array($apn, array(
      'certificate' => 'apns-dev-cert.pem',
      'private_key' => 'apns-dev-key.pem',
      'private_key_pass' => 'qwerty',
      'mode' => APN_SANDBOX
  ));

// Notification Payload context
$payload = apn_payload_init();
apn_payload_set_array($payload, array(
      'body' => 'This push was sent using PHP && php-apn',
      'sound' => 'default',
      'badge' => 34,
      'tokens' => array (
    	  'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX',
          'YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY'
      )
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
