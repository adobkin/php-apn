#php-apn#

php-apn is a PHP module, wrapping [libcapn](http://libcapn.org) C library to introduce simple yet powerful interface for sending push notifications to iOS and OS X (>= 10.8) devices from within your PHP code, making integration with Apple Push Notification Service as easy as possible.

Please visit [libcapn.org/php-apn](http://libcapn.org/php-apn) for more information

##LICENSE##

The extension is licensed under the [PHP 3.01](http://www.php.net/license/3_01.txt) license; see LICENSE file.

##REQUIREMENTS##

* capn library v 1.0.x ([http://libcapn.org](http://libcapn.org))
* capn library headers (to build extension)
* PHP 5.3+ SDK (to build extension)
* libtool (to build extension)

##INSTALLATION##

###AUTOMATIC###

Just run the following as root:

`pecl install apn`

or 

- `cd php-apn`
- `pecl install package.xml`


###MANUAL###

To install this module type the following:

- `cd php-apn`
- `phpize`
- `./configure`
- `make`
- `make install`

##DOCUMENTATION##

[View documentation](http://libcapn.org/php-apn/doc/html)

##EXAMPLE##

[View examples](http://libcapn.org/php-apn/doc/html/examples.html)


[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/adobkin/php-apn/trend.png)](https://bitdeli.com/free "Bitdeli Badge")

