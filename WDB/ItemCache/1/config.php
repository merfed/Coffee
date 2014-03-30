<?php
/* -------------------------------------------------------------------------------- 
	[PHP] ItemHEX Engine
	Created by Vladinator
	Thanks for the service Wowhead is running for us!
	
	NOTE: For the site to run as it should, give the directory "dump" chmod 777
	Also in php.ini set "allow_url_fopen" to "On" or if you have no access to the 
	php.ini file, uncomment "ini_set('allow_url_fopen','on');" below.
	
----------------------------------------------------------------------------------- */

// Enable or disable error reporting, turn off by adding a comment "#" or "//" in front to debug fix the page
error_reporting(0);

// This will try to override php.ini settings
ini_set('allow_url_fopen','on');

// Configure the page
// NO CONFIGURATIONS NEEDED AT THE MOMENT, JUST DUMMY STRINGS FOR FUTURE USAGE!
$conf_myhost = "www.mysite.com";	// only the domain name, if the 
									// site is accessed by using "http://www.mysite.com/mypage/site/" 
									// then the field here should contain "www.mysite.com" only!

$conf_mydir = "mypage/site";		// only the directory if there is any, for sub-orientation, 
									// using the above example: if you access the site using 
									// http://www.mysite.com/mypage/site/ then you should fill 
									// out this field with "mypage/site"!

// The javascript will access this for configuration, fill out the fields below (public):
if($_GET['js']=="1") {
?>
/* -------------------------------------------------------------------------------- 
	ItemHEX Engine
	Created by Vladinator
	Thanks for the service Wowhead is running for us!
----------------------------------------------------------------------------------- */

var conf_myhost = "<?=$conf_myhost;?>";
var conf_mydir = "<?=$conf_mydir;?>";

<?php
}
?>
