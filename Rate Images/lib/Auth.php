<?php

class Auth {
	function Auth() {
		session_start();
		
		if (!isset($_SESSION['loggedin'])) {
			header('Location: ../login/');
		}
	}
}

?>