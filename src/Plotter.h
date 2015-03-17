//
//  Plotter.h
//  SpaceFilling
//
//  Created by Will Gallia on 16/03/2015.
//
//

#pragma once

#include "ofMain.h"

typedef enum {
    PEN_DOWN,
    PEN_UP,
} PenPos;

class PenPoint : public ofVec2f {
public:
    PenPos pp;
    PenPoint() {}
    PenPoint(PenPos pp, float x, float y): ofVec2f(x, y), pp(pp) {}
    string str() {
        stringstream ss;
        ss << (pp == PEN_DOWN ? "PD" : "PU") << this->x << "," << this->y << ";";
        return ss.str();
    }
};

typedef vector<PenPoint> Path;
typedef map<int, Path> PlotterPaths;

#define INSTRUCTIONS_AT_A_TIME 10

class Plotter {
    ofSerial serial;
    bool checkSendMore();
    size_t counter;
    vector<string> instructions;
public:
    bool finished;
    ofVec2f offset;
    float scale;
    float percentDone;
    
    Plotter();
    
    void update();
    
    void plotPaths(const PlotterPaths &paths);
    
};