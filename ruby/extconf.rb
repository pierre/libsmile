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

dir_config(extension_name, "../include", "../.libs")
find_header("smile.h")

create_makefile(extension_name)
