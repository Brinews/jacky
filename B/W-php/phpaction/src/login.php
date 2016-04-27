<html>
<head>
<meta http-equiv="content-type" content="text/html; charset=utf8">
<title>MyTask</title></head>
<body>
<center>
 <table width=50% border=1>
 <form action=loginchk.php method=post>
  <tr>
   <td>
   <div align=center><strong>Sign In</strong>
   </div>
   <hr>
   <p align=center>Username:
   <input type=text name=username></p>
   <p align=center>Password:
   <input type=password name=pwd></p>

   <p align=center><font color=red>
<?php 
if (isset($_GET["err"])) {
	echo  "*".$_GET["err"]; 
}?>
	</font></p>

   <div align=center>
    <input type=submit value=Login>
    <input type=reset value=Reset>
	<a href=register.php>Register</a>
   </div>
    </td>
  </tr>
 </form>
 </table>
</center>
</body>
</html>
