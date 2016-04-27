<html>
	<head>
		<title>Task Viewer</title>
	</head>
	<body>
		<center>
			<form name="myForm" action="joinTask.php">
				<table  border="1" cellspacing="0" cellpadding="0">
					<caption>Event Status List</caption>
					<tr><td colspan=7 align=right><a href="logout.php">logout</a></td></tr>
						<th>Event Id</th>
						<th>Event Title</th>
						<th>Date</th>
						<th>Description</th>
						<th>Capacity</th>
						<th>Location</th>
						<th>Attend It</th>
<?php
session_start();

include("config.php");

if(empty($_SESSION['USERID'])){
	header("Location: login.php");
}

$link=mysql_connect("localhost","root", $password);

mysql_select_db("mytask");

$now = date("Y/m/d");
$exec="select * from my_task where ubegin>'$now'";

$result=mysql_query($exec);
while($rs=mysql_fetch_object($result)){
	$id=$rs->uid;
	$name=$rs->utaskname;
	$desc=$rs->utaskdesc;
	$cap=$rs->uspace;
	$bdate=$rs->ubegin;                    
	$loc = $rs->ulocation;

	$exec2 = "select uid from my_attend where userid='".$_SESSION['USERID']."' and taskid='$id'";

	$result2=mysql_query($exec2);
	$rows = mysql_num_rows($result2);

	$exec3 = "select * from my_attend where taskid=$id";
	$result3 = mysql_query($exec3);
	$cnt = mysql_num_rows($result3);
?>
					<tr align="center">
					<td><?php echo $id ?></td>
					<td><?php echo $name ?></td>
					<td><?php echo $bdate ?></td>
					<td><?php echo $desc ?></td>
					<td><?php echo $cap ?></td>
					<td><?php echo $loc ?></td>
					<td>
<?php if ($rows > 0) 
{
	echo "<a href=quitTask.php?uid=".$_SESSION['USERID']."&tid=$id>Unregister</a>";
} else if ($cnt < $cap) {
	//echo "<input type='submit' value='Register'></input>"; 
	echo "<a href=joinTask.php?uid=".$_SESSION['USERID']."&tid=$id>Register</a>";
} else {
	echo "FULL";
}?> 
</td>
				</tr>
<?php
}
mysql_close();
?>
				</table>
			</form>
		</center>
	</body>
</html>
