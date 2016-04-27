<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=utf8">
<title>Login</title>
</head>
<body>
<center>

<?php
 session_start();

 include("config.php");

 $name = $_POST['username'];
 $pwd = $_POST['pwd'];  
  
 $link = mysql_connect("localhost","root", $password);
 if(!$link){
  echo "Db connect error.";
  mysql_error();
 }
  
 mysql_select_db("mytask");

 $exec = "SELECT * FROM my_user WHERE uemail='$name'";
 $result = mysql_query($exec);
 $rows = mysql_num_rows($result);

 if ($rows>0) {
  $row = mysql_fetch_assoc($result);
  $lname=$row['uemail'];
  $lpwd=$row['upwd'];
  if($lpwd==$pwd){
   $_SESSION['olname']=$lname;
   $_SESSION['USERID']=$row['uid'];
   $_SESSION['USERTYPE']=$row['utype'];

   if ($row['utype'] == '01') {
	   header("Location: admin/listTask.php");
   } else 
	   header("Location: viewTask.php");
  }else{
   header("Location: login.php?err=WrongPassword");
  }
 }else{
   echo "No such user!<hr><p>";
   header("Location: login.php?err=NoSuchUser");
 }
?>

</center>
</body>
</html>
