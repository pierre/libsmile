<?php

function read_file($path) {
  $fh = fopen($path, 'r');
  $size = filesize($path);
  $data = fread($fh, $size);
  fclose($fh);

  return $data;
}

function test_one_file($json_filename, $smile_filename) {
  print("***  PHP  *** Testing $smile_filename\n");

  $data_dir = dirname(__FILE__) . '/../data';

  $smile_file = "$data_dir/smile/$smile_filename";
  $smile = read_file($smile_file);
  $actual = libsmile_decode($smile);

  $json_file = "$data_dir/json/$json_filename";
  $expected = read_file($json_file);

  if (strcmp($actual, $expected) != 0) {
    print("Expected: $expected\n");
    print("Actual:   $actual\n");
    exit("Failed!\n");
  }
}

$files = array('json-org-sample1', 'json-org-sample2', 'json-org-sample3', 'json-org-sample4', 'json-org-sample5',
               'numbers-int-4k', 'numbers-int-64k', 'test1', 'test2', 'test3');

foreach ($files as $file) {
  test_one_File("$file.jsn", "$file.smile");
}

?>
