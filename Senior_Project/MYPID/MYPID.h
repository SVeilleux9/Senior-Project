#ifndef MYPID_H
#define MYPID_H

class myPID{
private:
	bool notLinear;
	int maxOutput, minOutput;
	double kp, ki, kd;
	float adjustmentFactor;

public:
	
	myPID();
	~myPID();
	float getAdjustmentFactor();
	void setAdjustmentFactor(float x);
	void setValues(int maxOutput, int minOutput, double kp, double ki, double kd, bool notLinear);
	int PIDcalc(int poss, int desired);

};

#endif