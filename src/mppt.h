class MPPT {
	public:
		double update(double I_new, double V_new);		// Steps the MPPT and calculates the new delta needed to attain the maximum
		double update2(double I_new, double V_new);		// As above, but uses a variable step size as in http://pearlx.snu.ac.kr/publication/jhlee_2006_epe.pdf
		MPPT(double I, double V);
	private:
		double I;	// State variables
		double V;
		double abs(double a) {
			return a < 0 ? -a : a;
		}
};