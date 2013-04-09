<?php

/**
 * Apple Push Notification Service
 *
 * Apple Push Notification service (APNs for short) transports and routes a notification from a given provider to a
 * given device. A notification is a short message consisting of two major pieces of data: the device token and the
 * payload.
 *
 * The device token is analogous to a phone number; it contains information that enables APNs to locate the device on
 * which the client application is installed. APNs also uses it to authenticate the routing of a notification.
 *
 * The Apple is not guarantee delivery, you should not depend on the remote-notifications facility
 * for delivering critical data to an application via the payload. And never include sensitive data in the payload.
 *
 * The payload specifies how the user of an application on a device is to
 * be alerted.
 *
 * @package php-apn
 *
 */

/**
 * Specifies that "production" server shall be used. This is the default mode
 */
define('APN_PODUCTION', 16);

/**
 * Specifies that "sandbox" server shall be used
 */
define('APN_SANDBOX', 32);

/**
 * Creates a new Apple Push Notification resource which is needed to hold the data for a connection to
 * Apple Push Notification/Feedback Service
 *
 * This function allocates memory  which should be freed - call apn_free() function
 * for it
 *
 * @category Apple Push Notification Service
 * @return resource|NULL Returns a APNs link identifier on success or NULL on failure.
 */
function apn_init() {}

/**
 * Opens Apple Push Notification Service connection
 *
 * @see apn_close()
 * @category Apple Push Notification Service
 * @param resource $apn Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @param string|NULL $error A reference to return error information. Pass NULL, if information should not
 * be returned
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_connect($apn, &$error){}

/**
 * Closes Apple Push Notification/Feedback Service connection
 *
 * @see apn_connect()
 * @category Apple Push Notification Service
 * @param resource $apn Apple Push Notification Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @return void
 */
function apn_close($apn){}

/**
 * Frees memory associated with the apn identifier result
 *
 * @category Apple Push Notification Service
 * @param resource $apn Apple Push Notification Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_free($apn){}

/**
 * Sets path to an SSL certificate which will be used to establish secure connection
 *
 * @category Apple Push Notification Service
 * @param resource $apn Apple Push Notification Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @param string $certificate Path to a SSL certificate file
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_set_certificate($apn, $certificate){}

/**
 * Sets a path to a private key which will be used to establish secure connection
 *
 * @category Apple Push Notification Service
 * @param resource $apn Apple Push Notification Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @param string $private_key Path to a private key file
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_set_private_key($apn, $private_key){}

/**
 * Adds a device token
 *
 * Device token are used for identification of targets
 * which will receive the notification
 *
 * @see apn_add_tokens()
 * @category Apple Push Notification Service
 * @param resource $apn Apple Push Notification Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @param string $token Device token
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_add_token($apn, $token){}

/**
 * Adds a device tokens
 *
 * Device token are used for identification of targets
 * which will receive the notification
 *
 * @see apn_add_token()
 * @category Apple Push Notification Service
 * @param resource $apn Apple Push Notification Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @param array $tokens Device tokens
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_add_tokens($apn, array $tokens){}

/**
 * Set multiple apn options
 *
 * @category Apple Push Notification Service
 * @param resource $apn Apple Push Notification Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @param array $options An associative array specifying which options to set and their values. Valid options are:
 * <li>certificate - path to an SSL certificate</li>
 * <li>private_key - path to a private key</li>
 * <li>tokens - array of tokens</li>
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_set_array($apn, array $options){}

/**
 * Returns a path to private key which used to establish secure connection
 *
 * @category Apple Push Notification Service
 * @param resource $apn Apple Push Notification Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @return string|NULL Returns string on success, or NULL on failure
 */
function apn_get_private_key($apn){}

/**
 * Returns a path to an SSL certificate used to establish secure connection
 *
 * @category Apple Push Notification Service
 * @param resource $apn Apple Push Notification Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @return string|NULL Returns string on success, or NULL on failure
 */
function apn_get_certificate($apn){}

/**
 * Returns an array of device tokens which should receive the notification
 *
 * @category Apple Push Notification Service
 * @param resource $apn Apple Push Notification Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @return array|NULL Returns tokens array on success, or NULL on failure
 */
function apn_get_tokens($apn){}

/**
 * Returns array that contains apn options
 *
 * @category Apple Push Notification Service
 * @api
 * @param resource $apn Apple Push Notification Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @return array|NULL Returns array on success, or NULL on failure
 */
function apn_get_array($apn){}

/**
 * Sends push notification
 *
 * @api
 * @category Apple Push Notification Service
 * @param resource $apn Apple Push Notification Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @param resource $payload Apple Push Notification Payload Resource returned by apn_payload_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @param string|NULL $error A reference to return error information. Pass NULL, if information should not
 * be returned
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_send($apn, $payload, &$error){}

/**
 * Creates a new Apple Push Notification Payload resource
 *
 * This function allocates memory for which should be freed - call apn_payload_free() function
 * for it
 *
 * @category Apple Push Notification Payload
 * @return void
 */
function apn_payload_init(){}

/**
 * Frees memory associated with the payload identifier result
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_payload_free($payload){}

/**
 * Sets a number to display as a badge on the application icon
 *
 * If this property is not set, previously set value is not changed. To remove the badge,
 * set the value to 0
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @param integer $badge A number to display as the badge
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_payload_set_badge($payload, $badge){}

/**
 * Sets a text of the alert message
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @param string $body Text. Must be a valid UTF-8 encoded Unicode string
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_payload_set_body($payload, $body){}

/**
 * Sets a name of a sound file in the application bundle
 *
 * This sound file is played as an alert. If the sound file doesn’t exist or default is specified as the value,
 * the default alert sound is played
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @param string $sound Name of a sound file. Must be a valid UTF-8 encoded Unicode string
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_payload_set_sound($payload, $sound){}

/**
 * Sets a key used to get a localized alert-message string and an array of strings to appear in place of the
 * format specifiers in `key`
 *
 * The `key` string can be formatted with %@ and %n$@ specifiers to take the variables specified in `args`
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @param string $localized_key Key of localized string
 * @param array $args Array of string values to appear in place of the format specifiers in `key`
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_payload_set_localized_key($payload, $localized_key, array $args){}

/**
 * Sets a filename of an image file in the application bundle
 *
 * Filename may include or not include the extension. The image is used as the launch image when users tap
 * the action button or move the action slider. If this property is not specified, the system either
 * uses the previous one, uses the image identified by the UILaunchImageFile key in the application’s
 * Info.plist file, or falls back to Default.png
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @param string $launch_image A filename of an image file
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_payload_set_launch_image($payload, $launch_image){}

/**
 * Sets a key used to get a localized string to use for the right button’s
 * caption instead of "View"
 *
 * If the value is null, the system displays an alert with a single OK button that simply
 * dismisses the alert when tapped
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @param string $localized_action_key Key for localized string
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_payload_set_localized_action_key($payload, $localized_action_key){}

/**
 * Returns a number to display as the badge of the application icon
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @return integer|NULL Returns integer on success, or NULL on failure
 */
function apn_payload_get_badge($payload){}

/**
 * Returns a text of an alert message
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @return string|NULL Returns string on success, or NULL on failure
 */
function apn_payload_get_body($payload){}

/**
 * Returns a name of a sound file in the application bundle which played as an alert
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @return string|NULL Returns string on success, or NULL on failure
 */
function apn_payload_get_sound($payload){}

/**
 * Returns a key used to get a localized alert-message string
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @return string|NULL Returns string on success, or NULL on failure
 */
function apn_payload_get_localized_key($payload){}

/**
 * Returns an array of strings to appear in place of the format specifiers in localized alert-message string
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @return array|NULL Returns array on success, or NULL on failure
 */
function apn_payload_get_localized_key_args($payload){}

/**
 * Returns a filename of an image file in the application bundle used as a launch image
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @return string|NULL Returns string on success, or NULL on failure
 */
function apn_payload_get_launch_image($payload){}

/**
 * Returns a key used to get a localized string for the right button’s
 * caption instead of "View"
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @return string|NULL Returns string on success, or NULL on failure
 */
function apn_payload_get_localized_action_key($payload){}

/**
 * Adds a custom property  to notification payload
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @param string $key
 * @param mixed $value
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_payload_add_custom_property($payload, $key, $value){}

/**
 * Returns array that contains payload properties
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @return array|NULL Returns array on success, or NULL on failure
 */
function apn_payload_get_array($payload){}

/**
 * Set multiple payload properties
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @param array $options An associative array specifying which payload properties to set and their values.
 * Valid properties are:
 * <li>badge - number to display as a badge on the application icon</li>
 * <li>sound - name of a sound file in the application bundle</li>
 * <li>body - text of an alert message</li>
 * <li>localized_key - key used to get a localized alert-message string</li>
 * <li>localized_key_args - array of strings to appear in place of the format specifiers in `key`</li>
 * <li>localized_action_key - key used to get a localized string for the right button’s
 * caption instead of "View"</li>
 * <li>launch_image - filename of an image file in the application bundle</li>
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_payload_set_array($payload, array $options){}


