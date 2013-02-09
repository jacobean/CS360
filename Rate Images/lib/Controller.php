<?php

class Controller {
	public function execute($path_params) {

	}

	public function render($template) {
		$t = new View();
		$t->render($template);
	}
}
