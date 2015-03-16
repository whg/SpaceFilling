#include "ofApp.h"
#include "SpaceFillers.h"

using namespace ofxCv;
using namespace cv;

int numColours = 4;
int colSpace = ceil(255.0/numColours);


void ofApp::setup() {
    
    plotter.scale = 30;
    plotter.offset = ofVec2f(5000, 2000);
    
    processImg();
    processPoints();
    
    ofBackgroundHex(0xffffff);
    
    ofSetFrameRate(30);
//    pts = hilbert(4);
}

void ofApp::processImg() {

    srcImg.loadImage("/Users/whg/Desktop/peano.png");
    dstImg.allocate(srcImg.width, srcImg.height, OF_IMAGE_GRAYSCALE);
    
    copyGray(srcImg, dstImg);
    equalizeHist(dstImg);
    dstImg.update();
    dstImg.resize(81, 81);
    
    

}

void ofApp::processPoints() {
    
    pts = peano(4);
    pathsFromImgAndPnts(dstImg, pts);
    
}

void ofApp::update() {

    if (!plotter.finished) {
        plotter.update();
    }
}

void ofApp::draw() {

    ofTranslate(10, 10);

    ofSetHexColor(0xffffff);
    dstImg.draw(0, 0, 480, 480);
    
    ofTranslate(490, 0);
    
//    float d = 5;
//    for (int i = 0; i < pts.size(); i++) {
//        
//        ofLine(pts[i-1].x*d, pts[i-1].y*d, pts[i].x*d, pts[i].y*d);
//        ofCircle(pts[i].x*d, pts[i].y*d, 1);
//    }


    displayPlotting();
    
    //            ofCircle(pts[c].x, pts[c].y, 2);
//    if (c < pts.size()) c++;
    
}

void ofApp::keyPressed(int key) {
//    if (key > '0' && key <= '9') {
//        n = atoi((const char*) &key);
//    }
//    pts = peano(n);

    if (key == 'q') {
        if (!plotter.finished) {
            plotter.finished = true;
        }

    }

    if (key == 'p') plotter.plotPaths(paths);
}


void ofApp::pathsFromImgAndPnts(ofImage &img, const vector<ofVec2f> &pts) {
    
    ofPixels pix = img.getPixelsRef();
    bool penDown = false;
    
    stringstream ss;
    int mult = 30;
    int dx = 5000;
    int dy = 2000;
    int vs[] = { 0, 85, 170 };

    
    this->paths.clear();
    
    
    for (int n = 0; n < numColours-1; n++) {
        
        vector<PenPoint> ps;
        
        for (int i = 0; i < pts.size(); i++) {
            int x = int(pts[i].x);
            int y = int(pts[i].y);
            float b = pix.getColor(x, y).getBrightness();

            if (b > colSpace * n && b < colSpace * (n+1)) {

                if (!penDown) {
                    ps.push_back(PenPoint(PEN_UP, x, y));
                    
                }

                ps.push_back(PenPoint(PEN_DOWN, x, y));
                penDown = true;
            }
            else {
                penDown = false;
            }
        }
        
        paths[n+1] = ps;
    }

}

void ofApp::displayPlotting() {
    for(auto it = paths.begin(); it != paths.end(); ++it) {
        float d = 5;
        ofSetColor((it->first - 1) * colSpace);
        Path pts = it->second;
        
        ofVec2f *lastPoint = NULL;
        for (int i = 0; i < pts.size(); i++) {
            if (pts[i].pp == PEN_DOWN) {
                if (lastPoint != NULL) {
                    ofLine(lastPoint->x*d, lastPoint->y*d, pts[i].x*d, pts[i].y*d);
                }
                lastPoint = &pts[i];
            }
            else {
                lastPoint = NULL;
            }
//            ofLine(pts[i-1].x*d, pts[i-1].y*d, pts[i].x*d, pts[i].y*d);
//            ofCircle(pts[i].x*d, pts[i].y*d, 1);
        }
    }
}
