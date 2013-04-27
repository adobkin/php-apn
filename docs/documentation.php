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
 * @license   http://www.php.net/license/3_01.txt PHP 3.01
 * @link      http://libcapn.org/php-apn
 *
 */

/**
 * APN_PRODUCTION Specifies that "production" server shall be used. This is the default mode
 */
define('APN_PRODUCTION', 0);

/**
 * Specifies that "sandbox" server shall be used
 */
define('APN_SANDBOX', 1);

/**
 * Creates a new Apple Push Notification resource which is needed to hold the data for a connection to
 * Apple Push Notification/Feedback Service
 *
 * This function allocates memory  which should be freed - call apn_free() function
 * for it
 *
 * @category Apple Push Notification Service
 * @return resource|null Returns a APNs link identifier on success or null on failure.
 */
function apn_init() {}

/**
 * Opens Apple Push Notification Service connection
 *
 * @see apn_close()
 * @category Apple Push Notification Service
 * @param resource $apn Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @param string|null $error A reference to return error message. Pass null, if message should not
 * be returned
 * @param int|null $errcode A reference to return error code. Pass null, if code should not
 * be returned
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_connect($apn, &$error, &$errcode){}

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
 * Sets connection mode
 *
 * Each connection limited to one of two modes, each with its own assigned IP address:
 *
 * APN_PRODUCTION - Use the production mode when building the production version of the provider
 * application.  This mode uses gateway.push.apple.com, outbound TCP port 2195.
 *
 * APN_SANDBOX -  Use the sandbox mode for initial development and testing of the provider
 * application. It provides the same set of services as the production mode. The sandbox mode also acts
 * as a virtual device, enabling simulated end-to-end testing. This mode uses
 * gateway.sandbox.push.apple.com, outbound TCP port 2195.
 *
 * You must get separate certificates for the sandbox mode and the production mode.
 *
 * Default mode is :APN_PRODUCTION
 *
 * @category Apple Push Notification Service
 * @since 1.0.0
 * @param resource $apn Apple Push Notification Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @param int $mode Mode APN_PRODUCTION or APN_SANDBOX
 */
function apn_set_mode($apn, $mode){}

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
 * @param string|null $private_key_pass Private key passphrase or null
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_set_private_key($apn, $private_key, $private_key_pass = null){}

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
 * Valid options are:
 *
 * <ul>
 * <li>mode - Sets connection mode</li>
 * <li>certificate - path to an SSL certificate</li>
 * <li>private_key - path to a private key</li>
 * <li>private_key_pass - private key passphrase</li>
 * <li>tokens - array of device tokens</li>
 * </ul>
 *
 * @category Apple Push Notification Service
 * @param resource $apn Apple Push Notification Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @param array $options An associative array specifying which options to set and their values
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_set_array($apn, array $options){}

/**
 * Sends push notification
 *
 * @category Apple Push Notification Service
 * @param resource $apn Apple Push Notification Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @param resource $payload Apple Push Notification Payload Resource returned by apn_payload_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @param string|null $error A reference to return error message. Pass null, if message should not
 * be returned
 * @param int|null $errcode A reference to return error code. Pass null, if code should not
 * be returned
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_send($apn, $payload, &$error, &$errcode){}

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
 * Sets expiration time of notification
 *
 * Expiration time is a fixed UNIX epoch date expressed in seconds (UTC) that identifies when the notification
 * is no longer valid and can be discarded. You can specify zero or a value less than zero
 * to request that APNs not store the notification at all.
 * Default value is 0.
 *
 * @since 1.0.0
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @param integer $expiry Time in seconds
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_payload_set_expiry($payload, $expiry){}

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
function apn_payload_set_localized_key($payload, $localized_key, array $args = null){}

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
 * Adds a device token to payload
 *
 * Device token are used for identification of targets
 * which will receive the notification
 *
 * @since 1.0.0
 * @see apn_add_tokens()
 * @see apn_payload_add_tokens()
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @param string $token Device token
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_payload_add_token($payload, $token){}

/**
 * Adds a device tokens to payload
 *
 * Device token are used for identification of targets
 * which will receive the notification
 *
 * @since 1.0.0
 * @see apn_add_token()
 * @see apn_payload_add_token()
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @param array $tokens Device tokens
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_payload_add_tokens($payload, array $tokens){}

/**
 * Set multiple payload properties
 *
 * Valid properties are:
 *
 * <ul>
 * <li>badge - number to display as a badge on the application icon</li>
 * <li>sound - name of a sound file in the application bundle</li>
 * <li>body - text of an alert message</li>
 * <li>localized_key - key used to get a localized alert-message string</li>
 * <li>localized_key_args - array of strings to appear in place of the format specifiers in `key`</li>
 * <li>localized_action_key - key used to get a localized string for the right button’s
 * caption instead of "View"</li>
 * <li>launch_image - filename of an image file in the application bundle</li>
 * <li>tokens - array of device tokens</li>
 * <li>expiry - expiration time of notification</li>
 * </ul>
 *
 * @category Apple Push Notification Payload
 * @param resource $payload Apple Push Notification Resource returned by apn_payload_init(). If a non-resource is
 * used for the apn, an error of level E_WARNING will be emitted
 * @param array $properties An associative array specifying which payload properties to set and their values
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_payload_set_array($payload, array $properties){}

/**
 * Opens Apple Push Feedback Service connection
 *
 * @see apn_close()
 * @category Apple Push Feedback Service
 * @param resource $apn Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @param string|null $error A reference to return error message. Pass null, if message should not
 * be returned
 * @param int|null $errcode A reference to return error code. Pass null, if code should not
 * be returned
 * @return bool Returns TRUE on success or FALSE on failure
 */
function apn_feedback_connect($apn, &$error, &$errcode){}


/**
 * Returns array of device tokens which no longer exists
 *
 * @see apn_feedback_connect()
 * @category Apple Push Feedback Service
 * @param resource $apn Resource returned by apn_init(). If a non-resource is used for the apn,
 * an error of level E_WARNING will be emitted
 * @param string|null $error A reference to return error message. Pass null, if message should not
 * be returned
 * @param int|null $errcode A reference to return error code. Pass null, if code should not
 * be returned
 * @return array|null Returns array of devices token on success or null on failure
 */
function apn_feedback($apn, &$error, &$errcode){}
