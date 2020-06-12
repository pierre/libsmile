require_relative '../ruby/libsmile'

@parser = Smile::Parser.new()

def read_file(path)
  return open(path, "rb") { |io| io.read }
end

def test_one_file(json_filename, smile_filename)
  puts "***  Ruby *** Testing #{smile_filename}\n";

  data_dir = "#{File.dirname(__FILE__)}/../data"

  smile_file = "#{data_dir}/smile/#{smile_filename}"
  smile = read_file(smile_file)
  actual = @parser.decode(smile)

  json_file = "#{data_dir}/json/#{json_filename}"
  expected = read_file(json_file)

  unless expected.eql? actual
    puts("Expected: #{expected}");
    puts("Actual:   #{actual}");
    exit(1);
  end
end

files = ['json-org-sample1', 'json-org-sample2', 'json-org-sample3', 'json-org-sample4', 'json-org-sample5',
         'numbers-int-4k', 'numbers-int-64k', 'test1', 'test2', 'test3'];

files.each do |file|
  test_one_file "#{file}.jsn", "#{file}.smile"
end
