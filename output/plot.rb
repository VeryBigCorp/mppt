# This script runs gnuplot and graphs the data
require 'fileutils'

files = Dir["./*.dat"]

IO.popen('gnuplot','w') {|io|
	io.puts 'set terminal pngcairo'
	files.each do |file|
		basename = File.basename(file, '.dat')
		FileUtils.mkdir_p "plots/#{basename}"

		# Graph P vs V
		io.puts "set xlabel 'V (Volts)'"
		io.puts "set ylabel 'P (Watts)'"
		io.puts "set title 'Power vs Voltage'"
		io.puts "set output 'plots/" + basename + "/power.png'"
		io.puts "plot '#{file}' using 3:4 title '' w l"

		# Graph P vs t
		io.puts "set xlabel 't (milliseconds)"
		io.puts "set title 'Power vs time with MPPT'"
		io.puts "set output 'plots/#{basename}/pvst.png"
		io.puts "plot '#{file}' using 1:4 title '' w l"
	end
}