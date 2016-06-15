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

	$no = mysqli_real_escape_string($db, $_POST["no"]);
    $name = mysqli_real_escape_string($db, $_POST["name"]);
    $sex = mysqli_real_escape_string($db, $_POST["sex"]);

    $query = "INSERT INTO $dbname.my_student (sno, sname, ssex) VALUES ('$no', '$name', '$sex')";

    echo "Your query statement: " . $query . "<br />";

    if (!mysqli_query($db, $query)){
      die('Error: ' . mysqli_error($db));
    }
    else{
      echo "Insert successful <br/>";
    }

    mysqli_close($db);  

	header("Location: listUsers.php");
?>
