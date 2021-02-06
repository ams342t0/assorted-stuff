<?php
session_start();

if (isset($_SESSION["l"]))
{
	header("location: main.php");
	exit;
}

?>