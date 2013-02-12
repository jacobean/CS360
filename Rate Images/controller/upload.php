<?php

class Upload {
	function execute($path_params, $view) {
		if ($_SERVER['REQUEST_METHOD'] == 'POST')
			return $this->handle_upload();

		$view->scripts = array(
			'<script src="../../public/js/jquery-upload/vendor/jquery.ui.widget.js"></script>',
			'<script src="../../public/js/jquery-upload/jquery.iframe-transport.js"></script>',
			'<script src="../../public/js/jquery-upload/jquery.fileupload.js"></script>',
			'<script src="http://blueimp.github.com/JavaScript-Load-Image/load-image.min.js"></script>',
			'<script src="http://blueimp.github.com/JavaScript-Canvas-to-Blob/canvas-to-blob.min.js"></script>',
			'<script src="../../public/js/jquery-upload/jquery.fileupload-fp.js"></script>',
			'<script src="../../public/js/upload.js"></script>',
			'<script src="//cdnjs.cloudflare.com/ajax/libs/underscore.js/1.4.4/underscore-min.js"></script>'
		);
		$view->render('upload.phtml');
	}

	function handle_upload() {
		ob_start();
		$upload_handler = new UploadHandler(array(
			'upload_dir' => 'public/images/uploaded/',
			'param_name' => 'file'
		));
		$result = json_decode(ob_get_flush());

		$name = $result->file[0]->name;
		$image = new Image($name);
		$image->setTitle($_POST['title']);
		$image->setDescription($_POST['description']);
		$image->setDate();
		$image->setRating(0);
	}
}

?>