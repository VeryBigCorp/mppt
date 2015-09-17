# This is an automation script for running the simulations with different curves and plotting them
# Run for a static IV curve (constant temperature, irradiance, etc)
# Simulations are for 10 kHz I/V sensors
echo "Running simulations..."
../bin/mppt "Static Curve.dat" 10000 10000 1
../bin/mppt "Sine Curve.dat" 10000 100000 2
../bin/mppt "Increasing Curve.dat" 10000 10000 3
echo "Plotting..."
ruby plot.rb