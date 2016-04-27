<?php

session_start();

include("../config.php");

if(empty($_SESSION['USERTYPE']) || $_SESSION['USERTYPE'] == '00'){
	header("Location: ../login.php");
}

if (isset($_GET["tid"])) {

$link=mysql_connect("localhost", "root", $password);
mysql_select_db("mytask");
$mtid=$_GET["tid"];
$exec="select * from my_task where uid=$mtid";
$result=mysql_query($exec);
while($rs=mysql_fetch_object($result)){
	$id=$rs->uid;
	$name=$rs->utaskname;
	$desc=$rs->utaskdesc;
	$cap=$rs->uspace;
	$bdate=$rs->ubegin; 
	$loc = $rs->ulocation;
}
?>

<center>
 <table width=50% border=0>
 <form action=updateTask.php method=post>
  <tr>
   <td colspan=2>
   <div align=center><strong>Task Edit</strong> </div>
   <hr>
	</td>
    </tr>
	<tr>
	<td> Task Title: </td> 
    <td> <input type=text name=title value=<?php echo $id ?> > </td>
	</tr>
	<tr>
    <td>Task Date: </td><td><input type=text name=tdate value=<?php echo $bdate ?> ></td>
	</tr>
	<tr>
   <td><p align=left>Task Capacity: </p> </td>
	<td>
   <input type=text name=tcap value=<?php echo $cap ?> ></td>
   <tr>
   <td>Task Location: </td><td>
   <input type=text name=tloc value=<?php echo $loc ?> ></td>
   </tr>
	<tr><td>
   Task Description: </td><td>
   <input type=text name=tdesc value=<?php echo $desc ?> ></td>
   <input type=hidden name=tid value=<?php echo $mtid ?> >

    <tr>
	<td colspan=2>
   <div align=left>
    <input type=submit value=Submit>
   </div>
    </td>
  </tr>
 </form>
 </table>
</center>

<?php
} else {
?>

<center>
 <table width=50% border=0>
 <form action=insertTask.php method=post>

  <tr><td colspan=2> 
  <div align=center><strong>Task Add</strong> </div> <hr>
  </td></tr>

  <tr><td> 
   <p align=left>Task Title: 
  </td><td><input type=text name=title></p>
  </td></tr>
  <tr><td> 
   <p align=left>Task Date: 
  </td> <td><input type=text name=tdate></p>
  </td></tr>
  <tr><td> 
   <p align=left>Task Capacity: </td>
   <td> <input type=text name=tcap></p>
  </td></tr>
  <tr><td> 
   <p align=left>Task Location:</td>
  <td> <input type=text name=tloc></p>
  </td></tr>
  <tr><td> 
   <p align=left>Task Description: </td>
  <td><input type=text name=tdesc></p> 
  </td></tr>
  <tr><td> 

	<tr><td colspan=2>
    <div align=left> <input type=submit value=Submit> </div>
    </td></tr>

 </form>
 </table>
</center>

<?php
}
?>
