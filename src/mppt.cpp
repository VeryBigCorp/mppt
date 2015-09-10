#include "mppt.h"

#define DX .01		// Used for static step size
#define Mmin = .000001
#define Mmax = 2

MPPT::MPPT(double I_o, double V_o){
	I = I_o;
	V = V_o;
}

double MPPT::update(double I_new, double V_new){
	double dI = I_new - I;
	double dV = V_new - V;

	double dX = 0;

	if(dV == 0){
		if(dI == 0){
			dX = 0;
		} else if(dI > 0) {
			dX = DX;
		} else {
			dX = -DX;
		}
	} else {
		double deriv = dI/dV;
		double ratio = -I_new/V_new;

		//std::cout << "Derivative: " << deriv << "; Ratio: "<< ratio;

		if(deriv == ratio) {	// Found the MPP!
			dX = 0;
		} else if(deriv > ratio){ // We're to the left of the MPP
			dX = DX;
		} else if(deriv < ratio){ // To the right
			dX = -DX;
		}
	}

	V = V_new;
	I = I_new;

	return dX;
}