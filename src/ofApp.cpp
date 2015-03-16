#include "ofApp.h"
#include "SpaceFillers.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
    cam.initGrabber(640, 480);
    gray.allocate(640, 480, OF_IMAGE_GRAYSCALE);
    
    hilbert(4);
}

void ofApp::update() {
    cam.update();
    if(cam.isFrameNew()) {
        convertColor(cam, gray, CV_RGB2GRAY);
        equalizeHist(gray);
        gray.update();
    }
}

void ofApp::draw() {
    cam.draw(0, 0);
    gray.draw(640, 0);
}
