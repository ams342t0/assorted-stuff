<?php
session_start();

if (isset($_SESSION["l"]))
{
	exit;
}
if (!isset($_SESSION["lc"]))
{
	$_SESSION["lc"]=1;
}
else
{
	$_SESSION["lc"]++;
	if ($_SESSION["lc"]>10)
	{
		print("Don't push it.");
		exit;
	}	
}

if (!isset($_GET["u"])) exit;
if (!isset($_GET["p"])) exit;

$uid = $_GET["u"];
$pwd = $_GET["p"];

if (strlen($uid)==0) print("Emtpy username ");
if (strlen($pwd)==0) print("Empty password ");
$h="*".strtoupper(hash("sha1",hash("sha1",$pwd,true)));
$cn = mysqli_connect("localhost","loginid","loginpw") or die(mysqli_error());
mysqli_select_db($cn,"dbname");
$r = mysqli_query($cn,"select user,password from user where user = '".$uid."' and password = '".$h."'");
if (mysqli_num_rows($r)==0) 
{
	print(" Denied ");
	mysqli_close($cn);
	exit;
}
print("ok");
mysqli_close($cn);
if (!isset($_SESSION["l"]))
	$_SESSION["l"]=1;
?>