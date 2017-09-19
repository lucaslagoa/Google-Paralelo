<?php 
	$query = $_GET['query'];
	$interface = shell_exec("./tp4 -i entrada.txt '$query' 5 5");

	echo $interface;
?>
