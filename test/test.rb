require '../ruby/build/libsmile'

smile_file = '../data/smile/test1.smile'

smile = open(smile_file, "rb") { |io| io.read }

puts Libsmile.smile_decode_block_init
puts Libsmile.smile_decode_block_exit
