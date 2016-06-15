<?php
	include("config.php");

    $servername = "localhost";

    // Create connection
    $db = mysqli_connect($servername, $username, $password, $dbname);

    // Check connection
    if (!$db) {
      die("Connection failed: " . mysqli_connect_error()) . " <br/>";
    }
    echo "Connected successfully <br/>";

    // get FORM contents below
	$no = mysqli_real_escape_string($db, $_POST["no"]);
    $name = mysqli_real_escape_string($db, $_POST["name"]);
    $sex = mysqli_real_escape_string($db, $_POST["sex"]);

    $query = "update $dbname.my_student set sname='$name', ssex='$sex' where sno='$no'";

    //echo $query;

    if (!mysqli_query($db, $query)){
      die('Error: ' . mysqli_error($db));
    }
    else{
      echo "update successful <br/>";
    }

    mysqli_close($db);  

	header("Location: listBooks.php");
?>
