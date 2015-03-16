/*
 *  SSerial.h
 *  Sybil
 *
 *  Created by Will Gallia on 18/04/2011.
 *  Copyright 2011 . All rights reserved.
 *
 */


#ifndef _SSERIAL
#define _SSERIAL

#include "ofMain.h"
#include "STypes.h"
#include "SPreview.h"

class SPreview;

class SSerial {
	
private:
	ofSerial serial;
	bool finished;	
	int pc; //point counter
	bool isSetup;
	
	SPreview* previewPtr;
	
	vector<SPoint> points;
	bool multipleMove;
	int counter;
	vector<unsigned char> readBytes;
	
public:
	SSerial();
	~SSerial();
	
	void update();
	void sendLine(int x0, int y0, int x1, int y1);
	void sendSingleLine(int x0, int y0, int x1, int y1);
	void sendSingleMove(int x, int y);
	
	void sendMove(int t, int x, int y);
	void sendMoveAbs(int x, int y);
	void sendMoveRel(int x, int y);
	
	void sendMultipleMove(vector<SPoint> &points);
	void sendMultipleMove(vector<SPoint> &points, bool finish);
	
	bool sendPen(string command);
	void sendDelayChange(int delay_ms);
	
	SPoint getPos();
	
	void checkInput();
	void flush();
	
	int queryDelayed();
	void sendStart();
	int available();
	
	void sendFinish();
	void checkIsFinished();

	bool checkSendMore();
	
	void sendInstruction(int x, int y);
	
	bool isFinished();
	void setFinished(bool b);
	
	bool isConnected();
	
};




#endif