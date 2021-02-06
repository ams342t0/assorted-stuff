<?php
if (!isset($_GET['opt']))
{
	exit;
}

$cn = mysqli_connect("localhost","loginid","loginpw") or die(mysqli_error());
mysqli_select_db($cn,"dbname");
$o = $_GET['opt'];

switch ($o)
{
	case '4':
		$r = mysqli_query($cn,"select scheduleid,quickdescription from schedules order by quickdescription");
		print("<option id='1048576'>-ALL SCHEDULES-</option>");
		while ($a = mysqli_fetch_array($r))
		{
			print("<option id='".$a['scheduleid']."'>".$a['quickdescription']."</option>");
		}
		break;

	case '6':
		$tid = $_GET["tid"];
		if ($tid == "1048576") 
			$r = mysqli_query($cn,"select classid,classname from classes order by classname");
		else 
		$r = mysqli_query($cn,"select classid,classname from classes where schedule = " .$tid." order by classname");
		while ($a = mysqli_fetch_array($r))
		{
			print("<option id='".$a['classid']."'>".$a['classname']."</option>");
		}
		break;


	case '8':
		$tid = $_GET["tid"];
		$r = mysqli_query($cn,"select itemid,item from items order by item");
		while ($a = mysqli_fetch_array($r))
		{
			print("<option id='".$a['itemid']."'>".$a['item']."</option>");
		}
		break;
}

mysqli_close($cn);


?>