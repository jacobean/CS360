<?php
function __autoload($class_name) {
	$dirs = array('lib', 'model');
	foreach ($dirs as $dir) {
		$path = $dir . '/' . $class_name . '.php';
		if (file_exists($path)) {
			include $path;
			break;
 		}
	}
}
