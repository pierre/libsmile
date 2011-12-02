# Loads mkmf which is used to make makefiles for Ruby extensions
require 'mkmf'

extension_name = "libsmile"

unless $CFLAGS.gsub!(/ -O[\dsz]?/, ' -O3')
  $CFLAGS << ' -O3'
end
if CONFIG['CC'] =~ /gcc/
  $CFLAGS << ' -Wall -Werror'
end
$LDFLAGS << ' -lsmile'

dir_config(extension_name, "../../include", "../../")
find_header("smile.h")
find_library("libsmile", "smile_decode_block_init")
find_library("libsmile", "smile_decode_block")
find_library("libsmile", "smile_decode_block_exit")

create_makefile(extension_name)
