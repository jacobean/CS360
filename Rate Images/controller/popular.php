<?php

class Popular {
	public function execute($path_params, $view) {
		$images = Image::getAll();

		usort($images, function($a, $b) {
			return $a->getRating() <= $b->getRating();
		});

		array_splice($images, 10);

		$view->title = 'Popular Images';
		$view->images = $images;
		$view->render('imagelist.phtml');
	}
}

?>