# Loads mkmf which is used to make makefiles for Ruby extensions
require 'mkmf'

extension_name = "smile"

unless $CFLAGS.gsub!(/ -O[\dsz]?/, ' -O3')
  $CFLAGS << ' -O3'
end
if CONFIG['CC'] =~ /gcc/
  $CFLAGS << ' -Wall -Werror'
end

dir_config(extension_name, "../../c/src", "../../c/src")
find_header("smile_decode.h")
find_library("smile_decode", "smile_decode_header")

create_makefile(extension_name)
