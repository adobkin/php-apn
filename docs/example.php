<?php
/**
 * @package php-apn
 */

// APNS contex
$apn = apn_init();
apn_set_array($apn, array(
      'certificate' => 'apns-dev-cert.pem',
      'private_key' => 'apns-dev-key.pem',
      'tokens' => array (
    	  'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX',
          'YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY'
      )
  ));

// Notification Payload context
$payload = apn_payload_init();
apn_payload_set_array($payload, array(
      'body' => 'This push was sent using PHP && php-apn',
      'sound' => 'default',
      'badge' => 34
));
apn_payload_add_custom_property($payload, 'test', 56);

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
