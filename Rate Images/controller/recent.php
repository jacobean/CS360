<?php

class Recent extends Controller {
	public function execute($path_params, $view) {
		$images = Image::getAll('public/images/uploaded/');

		usort($images, function($a, $b) {
			return $a->getDate() <= $b->getDate();
		});

		array_splice($images, 10);

		$view->title = 'Recent Images';
		$view->images = $images;
		$view->render('imagelist.phtml');
	}
}