#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "Plotter.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
    void processImg();
    void processPoints();
    
    void pathsFromImgAndPnts(ofImage &img, const vector<ofVec2f> &pts);
    
    void displayPlotting(float toSize);
    
    ofImage srcImg, dstImg, postImg;
    
    vector<ofVec2f> pts;
    int c = 0, n = 1;
    PlotterPaths paths;
    
    Plotter plotter;
};
