<?php

class Login {
	function execute($path_params, $view) {
		session_start();

		if ($_SERVER['REQUEST_METHOD'] == 'POST')
			return $this->handle_login($view);

		$view->render('login.phtml');
	}

	function handle_login($view) {
		$user = $_POST['user'];
		$password = $_POST['password'];

		if ($user == 'admin' && $password == 'password') {
			$_SESSION['loggedin'] = true;
			session_write_close();
			header('Location: ../upload/');
		} else {
			$view->error = 'Wrong username or password.';
			$view->render('login.phtml');
		}
	}
}

?>