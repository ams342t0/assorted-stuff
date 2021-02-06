<?php
session_start();
if (!isset($_SESSION["l"])) 
{
	header("Location:http://localhost/c");
	exit;
}

?>

<!DOCTYPE html>

<html>
<head>
<title> IHG (Compact) </title>

<script type="text/javascript" src="i.js"></script>
<link rel="stylesheet" type="text/css" href="s.css"/>
<style>

.fmt1
{
	height:40px;
	font-size:16px;
	font-family:calibri;
}

</style>
</head>

<body class="fmt1">

<div id="divmt">
<br/>
<br/>

<hr>
<table id='maintable' border="1" width="95%">
</div>
</table>


<div id="details">
<table width="100%" align="center">
<tr>
<td align="center">
<input type="button" value="GRADES" id="quickview" style="height:40px">
<input type="button" value="CLOSE" id="closedetails" style="height:40px">
</td>
</tr>
</table>
<p id="pdetails"></p>
</div>

<div id="boxdiv">
<table width="100%">
<tr>
<td align="center">
<input style="height:40px" type="button" value="CLOSE" id="closeboxbtn" />
</td>
</tr>
</table>
<p id="boxpid"></p>
</div>


<div id="gradebox" style="width:400px">
<table>
<tr>
	<td style="width:200px">
		Score:<input type="number" align="center" id="txtgrade" size="3" class = "fmt1">
		<br/>
		<p id="gdescription"></p>
	</td>
	<td "width:200px">
		<input type="button" style="height:40px;width:60px" id="savezero" value="ZERO" >
		<input type="button" style="height:40px;width:60px" id="savenull" value="NULL"><br/><br/>
		<input type="button" style="height:60px;width:60px" id="savegrade" value = "SAVE">
		<input type="button" style="height:60px;width:60px" value="CLOSE" id="gboxclose">
	</td>
</tr>
</table>
</div>

<div id="divmainbar">
<table width="90%" id = "mainbar">
<tr>
<td width="50%">
	<select class="fmt1" id="cbschedule" style="width:300px"></select>
</td>
<td width="50%">
    <select class="fmt1"  id="cbclasses" style="width:300px"></select>
</td>
</tr>
<tr>
<td width="50%">
	<select id="cbtopics"  class="fmt1" style="width:300px"></select>
</td>
<td width="50%">
	<button id="btnrefresh" style="width:300px;height:40px">LOAD</button>
</td>
</tr>
</table>
</div>

<hr>
</body>

</html>