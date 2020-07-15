--TEST--
Check if lunar is loaded
--SKIPIF--
<?php
if (!extension_loaded('lunar')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "lunar" is available';
?>
--EXPECT--
The extension "lunar" is available
