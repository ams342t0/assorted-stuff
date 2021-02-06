<?php

if (!isset($_GET["sid"])) return;

$studid = $_GET["sid"];
$cn = mysqli_connect("localhost","loginid","loginpw") or die(mysqli_error());
mysqli_select_db($cn,"dbname");

$s = "SELECT sl.Fullname as Name, s.Sex, l.levelprefix as Level, sl.school as School, c.centername as Center, " .
             " c.city AS City, r.regionshort as Region, rm.roomname as Room,cl.classname as Class,is_noted,is_registered,qscore" .
             " FROM ((((((students AS sl INNER JOIN Centers AS c ON sl.center = c.centerid) " .
             " INNER JOIN Sexes AS s ON sl.sex = s.sexid) INNER JOIN Levels AS l ON sl.ilevel = l.levelid) " .
             " INNER JOIN Regions AS r ON c.region = r.regionid) INNER JOIN Areas AS a ON r.area = a.areaid) " .
             " LEFT OUTER  JOIN Rooms as rm on rm.roomid=sl.room) " .
			 " LEFT OUTER  JOIN classes as cl on cl.classid=sl.class " .
             " WHERE studid=".$studid." ORDER BY regionid,centername,fullname";

$r = mysqli_query($cn,$s);
if (!$r)
	print(mysqli_error($cn));

$a = mysqli_fetch_assoc($r);


print("<table>");

foreach($a as $v=>$i)
{
	if (isset($i))	
	{
		print("<tr>");
		if ($v=='is_noted' || $v=='is_registered')
		{
			if ($i==0) print("<td>".$v." : </td><td>NO</td>");
			else  print("<td>".$v." : </td><td>YES</td>");
		}
		else print("<td>".$v." : </td><td>".strtoupper(mb_convert_encoding($i,"utf-8","html-entities"))."</td>");
		print("</tr>");
	}
}
print("</table>");
mysqli_close($cn);
?>