<html>
	<head>
		<title>Event Viewer</title>
	</head>
	<body>
		<center>
			<form name="myForm" action="listTask.php">
				<table  border="1" cellspacing="0" cellpadding="0">
					<caption>Event Status List</caption>
					<tr><td colspan=7 align=right><a href="../logout.php">logout</a></td></tr>
						<th>Event Id</th>
						<th>Event Title</th>
						<th>Date</th>
						<th>Description</th>
						<th>Capacity</th>
						<th>Location</th>
						<th>Attend It</th>
<?php
session_start();
include("../config.php");

if(empty($_SESSION['USERTYPE']) || $_SESSION['USERTYPE'] == '00'){
	header("Location: ../login.php");
}

$link=mysql_connect("localhost", "root", $password);
mysql_select_db("mytask");
$exec="select * from my_task";
$result=mysql_query($exec);
while($rs=mysql_fetch_object($result)){
	$id=$rs->uid;
	$name=$rs->utaskname;
	$desc=$rs->utaskdesc;
	$cap=$rs->uspace;
	$bdate=$rs->ubegin;                    
	$loc = $rs->ulocation;
?>
					<tr align="center">
					<td><?php echo $id ?></td>
					<td><?php echo $name ?></td>
					<td><?php echo $bdate ?></td>
					<td><?php echo $desc ?></td>
					<td><?php echo $cap ?></td>
					<td><?php echo $loc ?></td>
					<td>
<?php 
	echo "<a href=addTask.php?tid=$id>Edit</a>&nbsp";
	echo "<a href=delTask.php?tid=$id>Delete</a>";
?> 
</td>
				</tr>
<?php
}
mysql_close();
?>
<tr><td colspan=7 align=left><a href="addTask.php">Add New Event</a></td></tr>
				</table>
			</form>
		</center>
	</body>
</html>
