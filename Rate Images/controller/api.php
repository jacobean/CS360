<?php

class Api {
	function execute($path_params, $view) {
		$image = new Image($path_params[0]);
		$action = $path_params[1];

		$this->$action($image, $view);
	}

	function vote_up($image, $view) {
		$image->incRating();
		$view->json = array('title' => $image->getTitle());
		$view->render('json.ptpl', false /* don't wrap in layout */);
	}

	function vote_down($image, $view) {
		$image->decRating();
		$view->json = array('title' => $image->getTitle());
		$view->render('json.ptpl', false /* don't wrap in layout */);
	}
}

?>