<?php

class Home {
	public function execute($path_params, $view) {
		$view->render('home.phtml');
	}
}

?>