#ifndef MYPID_H
#define MYPID_H

class myPID{
	int maxOutput, minOutput;
	double kp, ki, kd;
public:
	myPID();
	~myPID();
	void setValues(int maxOutput, int minOutput, double kp, double ki, double kd);
	int PIDcalc(int poss);

};

#endif