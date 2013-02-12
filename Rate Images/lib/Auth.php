<?php

class Auth {
	function Auth() {
		if (!isset($_SESSION['loggedin'])) {
			header('Location: ../login/');
		}
	}
}

?>