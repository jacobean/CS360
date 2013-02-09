<?php

class Home extends Controller {
	public function execute($path_params) {
		$this->render('home.phtml');
	}
}