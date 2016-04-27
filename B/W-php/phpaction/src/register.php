<center>
 <table width=50% border=0>
 <form action=addUser.php method=post>

  <tr><td colspan=2> 
  <div align=center><strong>User Register</strong> </div> <hr>
  </td></tr>

  <tr><td> 
   <p align=left>User Email: 
  </td><td><input type=text name=email>*</p>
  </td></tr>
  <tr><td> 
   <p align=left>User Password: 
  </td> <td><input type=password name=pwd1>*</p>
  </td></tr>
  <tr><td> 
  <tr><td> 
   <p align=left>Password Again: 
  </td> <td><input type=password name=pwd2>*</p>
  </td></tr>
  <tr><td> 
   <p align=left>User Name: </td>
   <td> <input type=text name=uname></p>
  </td></tr>
  <tr><td> 
   <p align=left>User Phone:</td>
  <td> <input type=text name=uphone></p>
  </td></tr>
  <tr><td> 

	<tr><td colspan=2>
    <div align=left> <input type=submit value=Submit> </div>
	<?php if (isset($_GET['err'])) { echo $_GET['err'];}?>
    </td></tr>

 </form>
 </table>
</center>
