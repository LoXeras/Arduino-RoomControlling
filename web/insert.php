<?php
	$host='localhost';
	$uname='INSERT USERNAME';
	$pwd='PASSWORD HERE';
	$db="INSERT DATABASE";

	$con = mysqli_connect($host,$uname,$pwd,$db);
	 
	$temperature=$_REQUEST['t'];
	$humidity=$_REQUEST['h'];
	$room=$_REQUEST['r'];

	$flag['code']=0;

	if($r=mysqli_query($con,"INSERT INTO environment(room,temperature,humidity) VALUES ('$room','$temperature','$humidity') "))
	{
		$flag['code']=1;
		echo"success";
	}

	print(json_encode($flag));
	mysqli_close($con);
?>