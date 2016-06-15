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

	$rsno = mysqli_real_escape_string($db, $_POST["rsid"]);
	$rbno = mysqli_real_escape_string($db, $_POST["rbid"]);
    $from = mysqli_real_escape_string($db, $_POST["from"]);
    $to = mysqli_real_escape_string($db, $_POST["to"]);

    $query = "INSERT INTO $dbname.my_record (rsno, rbno, fromdate, todate) VALUES ('$rsno', '$rbno', '$from', '$to')";

    echo "Your query statement: " . $query . "<br />";

    if (!mysqli_query($db, $query)){
      die('Error: ' . mysqli_error($db));
    }
    else{
      echo "Insert successful <br/>";
    }

    mysqli_close($db);  

	header("Location: listBooks.php");
?>
