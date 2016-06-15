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
    $tid = mysqli_real_escape_string($db, $_GET["tid"]);

    $query = "delete from $dbname.my_book where bno='$tid'";

    if (!mysqli_query($db, $query)){
      die('Error: ' . mysqli_error($db));
    }
    else{
      echo "Delete successful <br/>";
    }

    mysqli_close($db);  

	header("Location: listBooks.php");
?>
