<?php

session_start();

if (isset($_POST['submit']))
{
	include_once 'dbh.inc.php';

	$onoma = "";
	if(trim($_POST['onoma']) != "")
	{
		$onoma = mysqli_real_escape_string($conn, $_POST['onoma']);
	}
	else if(trim($_POST['onoma1']) != "")
	{
		$onoma = mysqli_real_escape_string($conn, $_POST['onoma1']);
	}

	$eponimo = "";
	if(trim($_POST['eponimo']) != "")
	{
		$eponimo = mysqli_real_escape_string($conn, $_POST['eponimo']);
	}
	else if(trim($_POST['eponimo1']) != "")
	{
		$eponimo = mysqli_real_escape_string($conn, $_POST['eponimo1']);
	}

	$taut = "";
	if(trim($_POST['taut']) != "")
	{
		$taut = mysqli_real_escape_string($conn, $_POST['taut']);
	}
	else if(trim($_POST['taut1']) != "")
	{
		$taut = mysqli_real_escape_string($conn, $_POST['taut1']);
	}

	$afm = "";
	if(trim($_POST['afm']) != "")
	{
		$afm = mysqli_real_escape_string($conn, $_POST['afm']);
	}
	else if(trim($_POST['afm1']) != "")
	{
		$afm = mysqli_real_escape_string($conn, $_POST['afm1']);
	}

	$am = "";
	if(trim($_POST['am']) != "")
	{
		$am = mysqli_real_escape_string($conn, $_POST['am']);
	}
	else if(trim($_POST['am1']) != "")
	{
		$am = mysqli_real_escape_string($conn, $_POST['am1']);
	}

	$doi = mysqli_real_escape_string($conn, $_POST['doi']);
	$ame = mysqli_real_escape_string($conn, $_POST['ame']);
	$odos = mysqli_real_escape_string($conn, $_POST['odos']);
	$arithmos = mysqli_real_escape_string($conn, $_POST['arithmos']);
	$poli = mysqli_real_escape_string($conn, $_POST['poli']);
	$til = mysqli_real_escape_string($conn, $_POST['til']);
	$amdias = mysqli_real_escape_string($conn, $_POST['amdias']);
	$email = mysqli_real_escape_string($conn, $_POST['email']);
	$uid = mysqli_real_escape_string($conn, $_POST['uid']);
	$pwd = mysqli_real_escape_string($conn, $_POST['pwd']);
	$user_id = $_SESSION['u_id'];

	$sql = "UPDATE users SET user_first='$onoma',user_last='$eponimo',user_email='$email',user_uid='$uid',user_pwd='$pwd',taut='$taut',afm='$afm',doi='$doi',ame='$ame',am='$am',odos='$odos',arithmos='$arithmos',poli='$poli',til='$til',amdias='$amdias' WHERE user_id ='$user_id' ";
	mysqli_query($conn, $sql);
	header("Location: ../profile.php?update=success");
	exit();

}
else
{
	header("Location: ../profile.php");
	exit();
}