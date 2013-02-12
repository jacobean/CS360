<?php

class Logout {
	function execute($path_params, $view) {
		session_destroy();
		session_write_close();
		header('Location: ../login/');
	}
}

?>