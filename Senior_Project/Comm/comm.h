#ifndef COMM_H
#define COMM_H


class comm{

public:
	float value;
	String input;
	
	comm();
	~comm();
	
	void setValue();
	bool aNumber(String str);
	String getString();
	bool timeout();
};


#endif