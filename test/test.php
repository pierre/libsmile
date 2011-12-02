<?php

include('../php/libsmile.php');

$smile_file = '../data/smile/test1.smile';

$fh = fopen($smile_file, 'r');
$size = filesize($myFile);
$smile = fread($fh, $size);
fclose($fh);

smile_decode_block_init();
smile_decode_block_exit();

?>
