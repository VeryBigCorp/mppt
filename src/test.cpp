#include <iostream>
#include <unistd.h>
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include "mppt.h"

#define EPSILON .0000001

double q;
double kT;

// Random curves that somewhat resemble the actual curve (some with time dependence)
inline double i_curve_static(double V, double t){
	return fmax(0,10-pow(10,-26)*(exp(q*V/kT)-1));
}

inline double i_curve_sine(double V, double t){
	return fmax(i_curve_static(V, t) + 10*sin(t), 0);
}

inline double i_curve_linear_increase(double V, double t){
	return fmax(i_curve_linear_increase(V, t) + 10*t, 0);
}

inline double i_curve(int type, double V, double t){
	switch(type){
		case 1:
		return i_curve_static(V, t);
		case 2:
		return i_curve_sine(V, t);
		case 3:
		return i_curve_linear_increase(V, t);
	}
}

// Sweeps the i curve to find the voltage at mpp (to compare with the simulation)
double mppv(int type, double t){
	// Find when the derivative is 0
	double V = 0;
	double deriv = ((V+EPSILON)*i_curve(type, EPSILON, t)-V*i_curve(type, 0, t))/EPSILON;
	V = EPSILON;
	while(deriv > EPSILON){
		deriv = ((V+EPSILON)*i_curve(type, V + EPSILON, t)-V*i_curve(type, V, t))/EPSILON;
		V += fmax(EPSILON/2, .01*deriv);
	}
	return V;
}


// Actual simulation
int main(int argc, char* argv[]){

	if(argc < 5){
		std::cout << "Invalid number of arguments. Correct usage is mppt [output file] [runtime (milliseconds)] [refresh rate (Hz)] [curve type (1 = static, 2 = sine, 3 = linear increase)]\n";
		return 1;
	}

	std::cout << "Initializing variables...\n";
	q = pow(10,-19)*1.6;
	kT = pow(10,-23)*2930*1.38;

	double V = 0;
	double I = 0;
	MPPT tracker(I,V);

	int curveType = atoi(argv[4]);
	I = i_curve(curveType, V+.1, 0);
	tracker.variableStep(I,V+.1);

	std::ofstream dataFile;
	dataFile.open(argv[1]);

	double scl = 1/atof(argv[3]);

	std::cout << "Outputting data to " << argv[1] << "\n";
	std::cout << "Beginning simulation of MPPT...\n";

	int max = atoi(argv[2])*atoi(argv[3])/1000;
	for(int i = 0; i <= max; i++){
		I = i_curve(curveType, V, i*scl);
		V += tracker.variableStep(I,V);
		dataFile << i << "\t" << I << "\t" << V << "\t" << I*V << "\t" << I*mppv(curveType, i*scl) << "\n";
		std::cout << 100.0*i/max << "%" << '\r';
		std::flush(std::cout);
	}
	std::cout << "\n";

	dataFile.close();

	std::cout << "Simulation complete.\n";
	return 0;
}