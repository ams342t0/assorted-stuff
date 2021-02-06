
<!DOCTYPE html>

<html>
<head>

</head>

<body>

<?php
	if (!isset($_GET["qc"])) return;
	if (!isset($_GET["ql"])) return;
	if (!isset($_GET["qs"])) return;
	if (!isset($_GET["qt"])) return;
	if (!isset($_GET["qr"])) return;
	if (!isset($_GET["qn"])) return;
	if (!isset($_GET["qrm"])) return;
	if (!isset($_GET["qclass"])) return;
	if (!isset($_GET["qtopic"])) return;	
	$qman = 0;
	$qc = $_GET["qc"];
	$ql = $_GET["ql"];
	$qs = $_GET["qs"];
	$qt = $_GET["qt"];
	$qr = $_GET["qr"];
	$qn = $_GET["qn"];
	$qrm = $_GET["qrm"];
	$qclass = $_GET["qclass"];
	$qtopic = $_GET["qtopic"];
	
	$cn = mysqli_connect("localhost","loginid","loginpw") or die(mysqli_error());
	mysqli_select_db($cn,"dbname");
	
	$qstring = "WHERE is_registered<0";
	

	if ($qt != '1048576')
		$qstring = $qstring ." and s.schedule = " . $qt;

	if ($qclass != '1048576')
	{
		$qstring = $qstring." and class = " . $qclass;
		$qman=1;
	}

	$r = mysqli_query($cn,"select s.studid,fullname,school,levelprefix from students as s inner join levels as l on s.ilevel = l.levelid ".  $qstring . " order by fullname") or die (mysqli_error($cn));
	
	if (mysqli_num_rows($r)>0)
	{
			$i = 1;
			while ($a = mysqli_fetch_array($r))
			{
					print("<tr id='".$a["studid"]."' style='height:70px'>");
					print("<td height = '26px' width='5%' align='right'>".$i.".</td>");
					print("<td width='70%'>");
					print("<p style='font-size:22px'>".mb_convert_encoding($a["fullname"],'UTF-8','HTML-ENTITIES')."</p>");
					print("<p style='font-size:12px'>(".$a["levelprefix"].") ");
					print(mb_convert_encoding($a["school"],'UTF-8','HTML-ENTITIES')."</p>");
					print("</td>");
					if($qman ==1 ) print("<td width='20%' align='center' style='font-weight:bold;font-size:30px'>".getgrade($a["studid"],$qtopic,$cn)."</td>");
					print("</tr>");
					$i++;
			}
	}
	else
	{
					print("<tr>");
					print("<td height='40px' align='center'>(EMPTY LIST)</td>");
					print("</tr>");
	
	}
	mysqli_close($cn);

	function getgrade($sid,$tid,$cn)
	{
		$r = mysqli_query($cn,"select grade from grades where studid = " . $sid . " and itemid = " . $tid);
		if (mysqli_num_rows($r) == 0) return "";
		$a=mysqli_fetch_array($r);
		return $a["grade"];
	}
	
?>

</body>

</html>