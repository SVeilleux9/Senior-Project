#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H


class myMotor{
public:
	int poss;
	long spd;
	
	myMotor();
	~myMotor();
	
	void zeroMotor();
	void setMotorSpeed(int speed);
};



#endif