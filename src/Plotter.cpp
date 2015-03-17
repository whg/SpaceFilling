//
//  Plotter.cpp
//  SpaceFilling
//
//  Created by Will Gallia on 16/03/2015.
//
//

#include "Plotter.h"

Plotter::Plotter() {
    bool isSetup = serial.setup("/dev/cu.usbserial", 9600);
    
    cout << "serial is setup: " << isSetup << endl;
    
    finished = true;
    counter = 0;

    
    //just for good measure...
    serial.flush(true, true);
}

bool Plotter::checkSendMore() {
    
    serial.flush(true, true);
    
    const char *getAvailable = "\x1b.B";
    serial.writeBytes((unsigned char *) getAvailable, 3);
    
    
    int counter = 0;
    while (counter < 100) {
        if (serial.available() == 5) {
            unsigned char readBytes[5];
            serial.readBytes(readBytes, 5);

            int space = atoi((char*) readBytes);
            return space > 1300;
        }
        ofSleepMillis(100);
        counter++;
    }
    
    cout << "serial timeout" << endl;
    return false;
}


void Plotter::update() {
    
    if (finished) {
        return;
    }
    
    if (checkSendMore()) {
        
        for(int i = 0; i < INSTRUCTIONS_AT_A_TIME; i++) {
            if (counter < instructions.size()) {
                
                const char *comm = instructions[counter].c_str();
                
                serial.writeBytes((unsigned char*) comm, strlen(comm));
                
                counter++;
                percentDone = float(counter) / instructions.size() * 100.0;
//                printf("%sent: s\n", comm);
            }
            
            if (counter == instructions.size()) {
//                printf("sent stop\n");
//                printf("point size = %i", (int) instructions.size());
                finished = true;
                counter = 0;
                break;
            }
        }
        
        printf("counter =  %lu\n", counter);
        
    }
    
    
}

void Plotter::plotPaths(const PlotterPaths &paths) {
    instructions.clear();
    stringstream ss;
    float maxy;
    
    for(auto it = paths.begin(); it != paths.end(); ++it) {
        
        ss.str("");
        ss << "SP" << it->first << ";";
        instructions.push_back(ss.str());
        
        Path pts = it->second;
        PenPoint p;
        
        for (int i = 0; i < pts.size(); i++) {
            p = pts[i];
            p*= scale;
            p+= offset;
            p.y = 10320 - p.y;
            instructions.push_back(p.str());

            maxy = MAX(p.y, maxy);
        }
    }
    
    ss.str("");
//    ss << "PU0,0;";
    instructions.push_back(ss.str());
    
    cout << "starting plot" << endl;
    this->finished = false;


    // write everything to a file too
    ofstream outfile;
    outfile.open(ofToDataPath("output.hpgl").c_str());
    

    for (auto it = instructions.begin(); it != instructions.end(); it++) {
        outfile << *it;
    }

    outfile.close();
    cout << "wrote file" << endl;
    
//    ofExit();
}
