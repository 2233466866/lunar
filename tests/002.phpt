--TEST--
lunar_test1() Basic test
--SKIPIF--
<?php
if (!extension_loaded('lunar')) {
	echo 'skip';
}
?>
--FILE--
<?php
$ret = lunar_test1();

var_dump($ret);
?>
--EXPECT--
The extension lunar is loaded and working!
NULL
