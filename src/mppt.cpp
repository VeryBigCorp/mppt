#include <iostream>
#include "mppt.h"

#define DX 0.01	// Used for static step size
#define Mmin .5/62
#define Mmax -3/62

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

double MPPT::update2(double I_new, double V_new) {
	double dI = I_new - I;
	double dV = V_new - V;

	// r = v/I
	// r_s = dv/di
	// m = 1+r/r_s
	// dp/dv = i+v/r_s

	double ratio = -I_new/V_new;
	double deriv = dI/dV;
	double dX = 0;
	//double dP = I + V * deriv;
	double dP = V_new * I_new - V * I;
	// Mr = 1 + v*di / i*dv
	double M_r = 1 + (V_new * dI) / (I_new * dV);
	if (M_r < Mmax) {
		return Mmax;
	}
	if (M_r > -Mmin && M_r < 0) {
		return -Mmin;
	} else {
		return M_r;
	}
	if (M_r < Mmin && M_r > 0) {
		return Mmin;
	} else {
		return M_r;
	}

// if I < 0 return negative step


	// if(dV == 0){
	// 	if(dI == 0){
	// 		dX = 0;
	// 	} else if(dI > 0) {
	// 		dX = DX;
	// 	} else {
	// 		dX = -DX;
	// 	}
	// } else {
	// 	//std::cout << "Derivative: " << deriv << "; Ratio: "<< ratio;
	// 	if(deriv == ratio) {	// Found the MPP!
	// 		dX = 0;
	// 	} else if(deriv > ratio){ // We're to the left of the MPP
	// 		dX = DX;
	// 	} else if(deriv < ratio){ // To the right
	// 		dX = -DX;
	// 	}
	// }

	// V = V_new;
	// I = I_new;

	// if (dV == 0) {
	// 	return dX;
	// }

	// return dX * abs(dP / dV);
}

// Uses a different way for incremental step size (scale by derivative)
double MPPT::variableStep(double I_new, double V_new) {
	double dI = I_new - I;
	double dV = V_new - V;

	double dP = I_new*V_new - I*V;

	// Do normal incremental conductance
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

	// But with a twist
	if(dV != 0){
		return dX * min(1,.005*abs(dP/dV)); // This scales it (like damping, where .005 is the damping factor)
	} else {
		return dX;
	}
}