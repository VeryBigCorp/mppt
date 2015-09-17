# This script runs gnuplot and graphs the data
require 'fileutils'

files = Dir["./*.dat"]

IO.popen('gnuplot','w') {|io|
	io.puts 'set terminal pngcairo enhanced size 640,480 dashed'
	files.each do |file|
		basename = File.basename(file, '.dat')
		FileUtils.mkdir_p "plots/#{basename}"

		io.puts "set autoscale x"
		io.puts "set autoscale y"

		# Graph P vs V
		io.puts "set xlabel 'V (Volts)'"
		io.puts "set ylabel 'P (Watts)'"
		io.puts "set title 'Power vs Voltage'"
		io.puts "set output 'plots/#{basename }/PV.png'"
		io.puts "plot '#{file}' using 3:4 title '' w l"

		# Graph P vs t
		io.puts "set xlabel 't (milliseconds)"
		io.puts "set title 'Power vs time with MPPT'"
		io.puts "set xrange [0:1000]"
		io.puts "set output 'plots/#{basename}/Pt.png'"
		io.puts "plot '#{file}' using 1:4 title '' w l, '#{file}' using 1:5 title 'Actual MPP' w l lt 2"

		io.puts "set output 'plots/#{basename}/Pt_zoomed.png'"
		io.puts "set xrange [700:1000]"
		io.puts "plot '#{file}' using 1:4 title '' w l, '#{file}' using 1:5 title 'Actual MPP' w l lt 2"

		io.puts "set output 'plots/#{basename}/Pt_zoomed2.png'"
		io.puts "set xrange [400:1000]"
		io.puts "plot '#{file}' using 1:4 title '' w l, '#{file}' using 1:5 title 'Actual MPP' w l lt 2"

		io.puts "set autoscale x"

		# Graph I vs V
		io.puts "set xlabel 'V (Volts)'"
		io.puts "set ylabel 'I (Amps)'"
		io.puts "set title 'Current vs Voltage'"
		io.puts "set output 'plots/#{basename}/IV.png'"
		io.puts "set yrange [0:12]"
		io.puts "plot '#{file}' using 3:2 title '' w l lw 2 linecolor '#000000'"
	end
}