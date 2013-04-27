<?php
/**
 * Feedback Service
 */

// APNS contex
$apn = apn_init();
apn_set_array($apn, array(
      'certificate' => 'apns-dev-cert.pem',
      'private_key' => 'apns-dev-key.pem',
      'private_key_pass' => 'qwerty',
      'mode' => APN_SANDBOX
  ));

$error = NULL;
$errcode = 0;

if(apn_feedback_connect($apn, $error, $errcode)) {
    $tokens = apn_feedback($apn, $error, $errcode);
    if(!is_array($tokens)) {
        echo 'Failed to obtain device tokens: ' . $error;
    } else {
      foreach($tokens as $token) {
          echo 'Token: '.$token;
      }
    }
} else {
    echo 'Failed to connect to Apple Push Feedback Service: ' . $error;
}

apn_close($apn);
apn_free($apn);
