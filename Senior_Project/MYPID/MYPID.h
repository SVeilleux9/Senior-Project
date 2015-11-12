#ifndef MYPID_H
#define MYPID_H

class myPID{
	

public:
	int maxOutput, minOutput;
	double kp, ki, kd;
	double P=0,I=0,D=0;

	myPID();
	~myPID();
	void setValues(int maxOutput, int minOutput, double kp, double ki, double kd);
	int PIDcalc(int error, int poss);

};

#endif