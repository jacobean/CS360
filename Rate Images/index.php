<?php

include 'lib/autoload.inc';
date_default_timezone_set('America/Denver');
session_start();

$path_params = (preg_split('@/@', $_SERVER['PATH_INFO'], NULL, PREG_SPLIT_NO_EMPTY));
$controller = array_shift($path_params);

$path = 'controller/' . $controller . '.php';
if (file_exists($path)) {
	include $path;
	$controller = new $controller;
	$controller->execute($path_params, new View());
} else {
	header('HTTP/1.0 404 Not Found');
	$t = new View();
	$t->render('404.phtml');
}

?>