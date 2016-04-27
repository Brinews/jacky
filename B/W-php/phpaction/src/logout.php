<?php
session_start();
unset($_SESSION['USERID']);

session_destroy();

header("Location: login.php");

?>
