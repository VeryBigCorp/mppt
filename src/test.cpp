#include <iostream>
#include <unistd.h>
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include "mppt.h"

double q;
double kT;

// This is just a random curve that somewhat resembles the actual curve
double i_curve(double V, double t){
	return 10-pow(10,-26)*(exp(q*V/kT)-1);
}

int main(int argc, char* argv[]){

	if(argc < 4){
		std::cout << "Invalid number of arguments. Correct usage is mppt [output file] [runtime (milliseconds)] [refresh rate (Hz)]\n";
		return 1;
	}

	std::cout << "Initializing variables...\n";
	q = pow(10,-19)*1.6;
	kT = pow(10,-23)*293*1.38;

	double V = 0;
	double I = 0;
	MPPT tracker(I,V);

	std::ofstream dataFile;
	dataFile.open(argv[1]);

	double scl = 1/atof(argv[3]);

	std::cout << "Outputting data to " << argv[1] << "\n";

	std::cout << "Beginning simulation of MPPT...\n";
	int max = atoi(argv[2])*atoi(argv[3])/1000;
	for(int i = 0; i <= max; i++){
		I = i_curve(V, i*scl);
		V += tracker.update(I,V);
		dataFile << i << "\t" << I << "\t" << V << "\t" << I*V << "\n";
	}

	dataFile.close();

	std::cout << "Simulation complete.\n";
	return 0;
}