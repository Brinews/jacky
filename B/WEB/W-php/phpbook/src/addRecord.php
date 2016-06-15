<head>
<meta http-equiv="content-type" content="text/html; charset=utf8">
</head>

<center>
 <table width=50% border=0>
 <form action="insertRecord.php" method=post>

  <tr><td colspan=2> 
  <div align=center><strong>填写借阅信息</strong> </div> <hr>
  </td></tr>

  <tr><td> 
   <p align=left>书籍编号: 
  </td><td> <input type=text name=rbid value=<?php echo $_GET["bid"] ?> >
  </td></tr>

  <tr><td> 
   <p align=left>读者卡号: 
  </td><td><input type=text name=rsid></p>
  </td></tr>

  <tr><td> 
   <p align=left>借出日期: 
  </td> <td><input type=text name=from>(YYYY-MM-DD)</p>
  </td></tr>

  <tr><td> 
   <p align=left>归还日期: 
  </td> <td><input type=text name=to>(YYYY-MM-DD)</p>
  </td></tr>

  <tr><td colspan=2>
  <div align=left> <input type=submit value="确认添加"> </div>
  </td></tr>

 </form>
 </table>
</center>
