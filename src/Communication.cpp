/*
 *  SSerial.cpp
 *  Sybil
 *
 *  Created by Will Gallia on 18/04/2011.
 *  Copyright 2011 . All rights reserved.
 *
 */

#include "SSerial.h"
#include "ofMain.h"

bool penIsUp = true;



//constructor
SSerial::SSerial() {
		
	//connect to serial port
	//if(serial.setup("/dev/cu.usbserial-A600eI53", 28800)) {
//		printf("serial connected\n");
//	}
	
//	if(serial.setup("/dev/cu.usbserial-A700eyPP", 9600)) {
//			printf("serial connected\n");
//	}

//	isSetup = serial.setup("/dev/cu.usbserial-A600eI53", 28800);
//	isSetup = serial.setup("/dev/cu.usbserial-A600eI53", 9600);
    isSetup = serial.setup("/dev/cu.usbserial", 9600);
	
    
    cout << "serial is setup: " << isSetup << endl;
    
	//init variables
	finished = true;
	pc = 0;
	multipleMove = false;
	
	previewPtr = (SPreview*) ofGetAppPtr();
	
	//just for good measure...
	serial.flush(true, true);
	serial.setVerbose(true);
	
	counter = 0;
}

//destructor
SSerial::~SSerial() {

}

void SSerial::update() {
	
//	readBytes.clear();
//	while (serial.available()) {
//		readBytes.push_back(serial.readByte());
//	}
//	
//	if (readBytes.size() > 0) {
//		printf("readBytes = ");
//		for (int i = 0; i < readBytes.size(); i++) {
//			printf("%i ", (int) ((unsigned char) readBytes[i]));
//		}
//		printf("\n");
//	}
	
	
	if (checkSendMore()) {
		
		for(int i = 0; i < 10; i++) {
            if (counter < points.size()) {
                
                sendInstruction(points[counter].x, points[counter].y);
                counter++;
                
            }
            
            if (counter == points.size()) {
                sendFinish();
                printf("sent stop\n");
                printf("point size = %i", (int) points.size());
                previewPtr->stoppedDrawing();
                finished = true;
                counter++;
            }
        }
		
		printf("sent %i\n", counter);
		
	}
	
	previewPtr->setPointsDone(counter);
//	checkIsFinished();
	
	
}

void SSerial::sendInstruction(int x, int y) {
	
	switch (x) {
		case PEN_UP_POINT:
			sendPen("up");
			break;
		case PEN_DOWN_POINT:
			sendPen("down");
			break;
		case CHANGE_DELAY_POINT:
			sendDelayChange(y);
			break;
		default:
			sendMoveAbs(x, y);
			printf("sent moveAbs: x = %i, y = %i\n", x, y);
			break;
	}
	
}

void SSerial::sendFinish() {
	serial.writeByte((unsigned char) SERIAL_STX);
	serial.writeByte((unsigned char) COMMAND_CODE_FINISH);
	serial.writeByte((unsigned char) SERIAL_ETX);
}

void SSerial::checkIsFinished() {
	
	for (int i = 0; i < readBytes.size(); i++) {
		if (readBytes[i] == (unsigned char) PLOTTER_FINISHED_DRAWING) {
			previewPtr->stoppedDrawing();
			finished = true;
		}
	}	
	
}

void SSerial::sendMultipleMove(vector<SPoint> &points, bool finish) {
	
	this->points.clear();
	this->points = points;
	
	if (finish) {
		//add a pen up to finish...
		this->points.push_back(SPoint(PEN_UP_POINT, 0));
		this->points.push_back(SPoint(CHANGE_DELAY_POINT, 8));
		//and move to origin
		this->points.push_back(SPoint(0, 0));
		this->points.push_back(SPoint(CHANGE_DELAY_POINT, 16));
	}
		
	counter = 0;
	
	
	for (int i = 0; i < points.size(); i++) {
		printf("x = %i, y = %i\n", this->points[i].x, this->points[i].y);
	}
	
	//send the first, get the ball rolling...
	sendInstruction(this->points[counter].x, this->points[counter].y);
	counter++;
	
	finished = false;
	
	//get rid of any nasties that might be lurking in the buffer
	serial.flush(true, true);
	
	previewPtr->setNumPoints((int) this->points.size());
}

void SSerial::sendMultipleMove(vector<SPoint> &points) {
	
	sendMultipleMove(points, true);
}


//looks through readBytes array to see if there is a send next...
bool SSerial::checkSendMore() {

    serial.flush(true, true);
    
    const char *getAvailable = "\x1b.B";
    serial.writeBytes((unsigned char *) getAvailable, 3);
    
	
    
    int counter = 0;
    while (counter < 100) {
        if (serial.available() == 5) {
            unsigned char readBytes[5];
            serial.readBytes(readBytes, 5);
            cout << "serial in = ";
            for (int j = 0; j < 5; j++) {
                cout << readBytes[j] << " ";
            }
            cout << endl;
            cout << (((int)readBytes[0])*10 + ((int)readBytes[1])) << endl;
            cout << atoi((char*) readBytes) << endl;
            int space = atoi((char*) readBytes);
            return space > 1300;
        }
        ofSleepMillis(100);
        counter++;
    }
    
    cout << "serial timeout" << endl;
    return false;

    
//	for (int i = 0; i < readBytes.size(); i++) {
//		if (readBytes[i] == (unsigned char) RX_BUFFER_STX_ERROR) {
//			counter--;
//			printf("FOUND RX_BUFFER_STX_ERROR");
//			previewPtr->writeProgressErrorMessage("ERROR: No Start Command Found; resending data...");
//			return true;
//		}
//		else if (readBytes[i] == (unsigned char) RX_BUFFER_ETX_ERROR) {
//			counter--;
//			printf("FOUND RX_BUFFER_ETX_ERROR");
//			previewPtr->writeProgressErrorMessage("ERROR: Message length invalid; resending data...");
//			return true;
//		}
//		else if (readBytes[i] == (unsigned char) RX_BUFFER_LENGTH_ERROR) {
//			counter--;
//			printf("FOUND RX_BUFFER_LENGTH_ERROR");
//			previewPtr->writeProgressErrorMessage("ERROR: No End Command Found; resending data...");
//			return true;
//		}
//		else {
//			//clear the error writting if it's all ok now...
//			previewPtr->writeProgressErrorMessage("");
//		}
//
//	}
//	
//	for (int i = 0; i < readBytes.size(); i++) {
//		if (readBytes[i] == (unsigned char) SEND_FOR_NEXT_COMMANDS) {
//			return true;
//		}
//	}
//	
//	return false;
}



void SSerial::sendMoveAbs(int x, int y) {
	sendMove(0, x, y);
}

void SSerial::sendMoveRel(int x, int y) {
	sendMove(1, x, y);
}

void SSerial::sendMove(int t, int x, int y) {
	
	//convert to 1 byte
	unsigned char x00 = (unsigned char) x;
	unsigned char x01 = (unsigned char) (x>>8);
	unsigned char y00 = (unsigned char) y;
	unsigned char y01 = (unsigned char) (y>>8);

    int xs0 = ((x01<<8) | x00);
    int ys0 = ((y01<<8) | y00);
    
    char command[50];

	
	//write type... move
//	serial.writeByte((unsigned char) SERIAL_STX);
	
	if (t) {
        sprintf(command, "PR %d,%d;", x, y);
//		serial.writeByte((unsigned char) COMMAND_CODE_MOVE_REL);
	}
	else {
        
        if (penIsUp) {
            sprintf(command, "PU %d,%d;", x, y);
        }
        else {
            sprintf(command, "PD %d,%d;", x, y);
        }
//		serial.writeByte((unsigned char) COMMAND_CODE_MOVE_ABS);
	}
    
    serial.writeBytes((unsigned char*) command, strlen(command));
	
//	serial.writeByte(x00);
//	serial.writeByte(x01);
//	serial.writeByte(y00);
//	serial.writeByte(y01);
//	
//	serial.writeByte((unsigned char) SERIAL_ETX);
	

			
}

SPoint SSerial::getPos() {
	
	//let's do this now...
	serial.flush(true, true);

	serial.writeByte((unsigned char) SERIAL_STX);
	serial.writeByte((unsigned char) COMMAND_CODE_GET_POS);
	serial.writeByte((unsigned char) SERIAL_ETX);
	
		
	//wait until all results are there...
	//don't allow yourself to get into an infinite loop
	//wait until you can count to a big number... 
	
	//NB wait for done byte as well... not
	long l = 0;
	while (serial.available() != 4 && l < 99999) { 
		l++;
	}
	
	printf("no bytes available = %i \n", serial.available());

	
	if (l != 99999) {
	
		
		//fetch results
		unsigned char results[4];
		serial.readBytes(results, 4);
		
		//combine the 8bit results to two 16bit shorts
		short int x = (results[0] | (results[1]<<8));
		short int y = (results[2] | (results[3]<<8));
		
		return SPoint(x, y);
	}
	
	//this is not possible, ie. the get didn't work
	return SPoint(-1, -1);
	
}

bool SSerial::sendPen(string command) {
    
	if (command == "up") {
		
//		serial.writeByte((unsigned char) SERIAL_STX);
//		serial.writeByte((unsigned char) COMMAND_CODE_PEN_UP);
//		serial.writeByte((unsigned char) SERIAL_ETX);
//
        command = "PU;";
        serial.writeBytes((unsigned char*) command.c_str(), command.size());
        
		//sendStart();
		
		printf("sent pen up\n");
        penIsUp = true;
		return true;
	}
	else if(command == "down") {
		
//		serial.writeByte((unsigned char) SERIAL_STX);
//		serial.writeByte((unsigned char) COMMAND_CODE_PEN_DOWN);
//		serial.writeByte((unsigned char) SERIAL_ETX);
        
        command = "PD;";
        serial.writeBytes((unsigned char*) command.c_str(), command.size());
        
		
		//sendStart();
		penIsUp = false;
		printf("sent pen down\n");
		return true;
	}
	
	//if none of the above worked return false
	return false;

}

void SSerial::sendDelayChange(int delay_ms) {
	
	serial.writeByte((unsigned char) SERIAL_STX);
	serial.writeByte((unsigned char) COMMAND_CODE_CHANGE_STEP_DELAY);
	serial.writeByte((unsigned char) delay_ms);
	serial.writeByte((unsigned char) SERIAL_ETX);
}

void SSerial::sendLine(int x0, int y0, int x1, int y1) {
	
	//convert to 1 byte
	unsigned char x00 = (unsigned char) x0;
	unsigned char x01 = (unsigned char) (x0>>8);
	unsigned char y00 = (unsigned char) y0;
	unsigned char y01 = (unsigned char) (y0>>8);
	
	
	unsigned char x10 = (unsigned char) x1;
	unsigned char x11 = (unsigned char) (x1>>8);
	unsigned char y10 = (unsigned char) y1;
	unsigned char y11 = (unsigned char) (y1>>8);
	
	//write type
	serial.writeByte((unsigned char) 2);
	
	serial.writeByte(x00);
	serial.writeByte(x01);
	serial.writeByte(y00);
	serial.writeByte(y01);
	serial.writeByte(x10);
	serial.writeByte(x11);
	serial.writeByte(y10);
	serial.writeByte(y11);
	
	
	//these are just for printing out...
	int xs0 = ((x01<<8) | x00);
	int ys0 = ((y01<<8) | y00);	
	int xs1 = ((x11<<8) | x10);
	int ys1 = ((y11<<8) | y10);
	
	printf("wrote via serial %i %i %i %i %i \n", 2, xs0, ys0, xs1, ys1);
		
}

void SSerial::flush() {
	serial.flush(true, true);
}

int SSerial::available() {
	return serial.available();
}

bool SSerial::isFinished() {
	return finished;
}

void SSerial::setFinished(bool b) {
	finished = b;
}

bool SSerial::isConnected() {
	return isSetup;
}
