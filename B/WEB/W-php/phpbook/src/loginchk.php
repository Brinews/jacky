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
  
 $link = mysqli_connect("localhost", $username, $password, $dbname);
 if(!$link){
  echo "Db connect error.";
  mysql_error();
 }
  
 $exec = "SELECT * FROM my_student WHERE sno='$name'";
 $result = mysqli_query($link, $exec);

 if ($row=mysqli_fetch_array($result, MYSQL_ASSOC)) {
  $lname=$row['sno'];
  $lpwd=$row['sname'];
  if($lpwd==$pwd){
   $_SESSION['olname']=$lname;
   $_SESSION['USERID']=$row['sid'];
   $_SESSION['USERTYPE']=$row['sno'];

   header("Location: listBooks.php");
  }else{
   header("Location: login.php?err=WrongPassword");
  }
 }else{
     echo $exec;
   echo "No such user!<hr><p>";
   //header("Location: login.php?err=NoSuchUser");
 }
?>

</center>
</body>
</html>
