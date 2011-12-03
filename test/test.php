<?php

$smile_file = dirname(__FILE__) . '/../data/smile/test1.smile';

$fh = fopen($smile_file, 'r');
$size = filesize($smile_file);
$smile = fread($fh, $size);
fclose($fh);

print_r(libsmile_decode($smile));

?>
