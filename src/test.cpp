#include <iostream>
#include <unistd.h>
#include <math.h>
#include <fstream>
#include "mppt.h"

double q;
double kT;

double i_curve(double V, double t){
	return 10-pow(10,-26)*(exp(q*V/kT)-1);
}

int main(int argc, char* argv[]){

	if(argc == 1){
		std::cout << "Invalid number of arguments. Need an output file.\n";
		return 1;
	}

	std::cout << "Initializing variables...\n";
	q = pow(10,-19)*1.6;
	kT = pow(10,-23)*293*1.38;

	double V = 0;
	double I = i_curve(V, 0);
	MPPT tracker(V,I);

	std::ofstream dataFile;
	dataFile.open(argv[1]);

	std::cout << "Outputting data to " << argv[1] << "\n";

	std::cout << "Beginning simulation of MPPT...\n";

	for(int i = 0; i <= 1000; i++){
		I = i_curve(V, i*.0001);
		V += tracker.update(I,V);
		dataFile << i << "\t" << I << "\t" << V << "\t" << I*V << "\n";
		usleep(1000);
	}

	dataFile.close();

	return 0;
}