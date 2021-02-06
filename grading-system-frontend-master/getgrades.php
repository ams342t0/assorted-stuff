<?php
if (!isset($_GET["sid"])) return;

$studid = $_GET["sid"];
$cn = mysqli_connect("localhost","loginid","loginpw") or die(mysqli_error());
mysqli_select_db($cn,"dbname");
$q = "SELECT S.FULLNAME,S.QUIZ,S.QUIZCOUNT,S.ASSIGNMENT,S.DEPORTMENT,S.FINALEXAM,S.FECOUNT,S.FINALGRADE,S.RESULT, ".
     "G.ITEMID,G.GRADE, I.ITEM".
	 " FROM (GRADES AS G LEFT OUTER JOIN STUDENTS AS S ON G.STUDID=S.STUDID) LEFT OUTER JOIN ITEMS AS I ON G.ITEMID = I.ITEMID".
	 " WHERE G.STUDID = ".$studid." ORDER BY G.ITEMID";

$r = mysqli_query($cn,$q);

if (mysqli_num_rows($r)==0)
{
	print("<p>NO RECORDS FOUND.</p>");
	mysqli_close($cn);
	return;
}

print("<table width='100%'>");

//print("<p>Student Name: ".strtoupper(mb_convert_encoding($a["FULLNAME"],"utf-8","html-entities"))."</p>");

while ($a = mysqli_fetch_array($r))
{
	print("<tr>");
	print("<td width='60%' align='right'>".$a["ITEM"].":</td>");
	print("<td width='40%'>".$a["GRADE"]."</td>");
	print("</tr>");
	$DEP = $a["DEPORTMENT"];
	$QUIZ = $a["QUIZ"];
	$QC = $a["QUIZCOUNT"];
	$FE = $a["FINALEXAM"];
	$FEC = $a["FECOUNT"];
	$RES = $a["RESULT"];
	$FG = $a["FINALGRADE"];
}

	print("<tr>");
	print("<td width='60%' align='right'>ASSIGNMENT:</td>");
	print("</tr>");
	print("<tr>");
	print("<td width='60%' align='right'>DEPORTMENT:</td>");
	print("<td width='40%'>".$DEP."</td>");
	print("</tr>");
	print("<td width='60%' align='right'>QUIZ AVE.:</td>");
	print("<td width='40%'>".$QUIZ." (".$QC.")"."</td>");
	print("</tr>");
	print("<tr>");
	print("<td width='60%' align='right'>FINAL EXAM AVE.:</td>");
	print("<td width='40%'>".$FE." (".$FEC.")"."</td>");
	print("</tr>");
	print("<tr>");
	print("<td width='60%' align='right'>FINAL GRADE:</td>");
	print("<td width='40%'>".$FG." (".$RES.")"."</td>");
	print("</tr>");

print("</table>");
mysqli_close($cn);
?>