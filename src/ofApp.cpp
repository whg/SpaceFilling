#include "ofApp.h"
#include "SpaceFillers.h"
#include "ofxPosterize.h"

using namespace ofxCv;
using namespace cv;

const int numColours = 4;
int colSpace = ceil(255.0/numColours);

int displaySize = 450;
int padding = 10;
int targetSize = 64;
int progressHeight = 30;
int cols[numColours];

void ofApp::setup() {
    
    plotter.scale = 200;
    plotter.offset = ofVec2f(2000, 5000);
    
    processImg();
    processPoints();
    
    ofBackgroundHex(0xffffff);
    
    ofSetFrameRate(30);
    ofSetLineWidth(2);
    
    ofSetWindowShape(displaySize*3 + padding*5, displaySize+padding*2);
}

void ofApp::processImg() {

    srcImg.loadImage("hilbert.png");
    dstImg.allocate(srcImg.width, srcImg.height, OF_IMAGE_GRAYSCALE);

    
    copyGray(srcImg, dstImg);

    dstImg.resize(targetSize, targetSize);
    postImg.allocate(dstImg.width, dstImg.height, OF_IMAGE_GRAYSCALE);

    
    postImg = ofxPosterize::clusterize(dstImg, numColours, cols);
    sort(cols, cols+numColours);
    
//    equalizeHist(dstImg);
    dstImg.update();
    
    // make it look blocky!
    postImg.getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);;

}

void ofApp::processPoints() {
    
//    pts = peano(2);
    pts = hilbert(int(log2(targetSize)));
//    pts = spiral(targetSize);
//    pts = quadradticGosper(2);
//    pts = zigzag(targetSize);
    
    pathsFromImgAndPnts(postImg, pts);
    
}

void ofApp::update() {

    if (!plotter.finished) {
        plotter.update();
    }
}

void ofApp::draw() {

    ofTranslate(padding, padding);

    if (!plotter.finished) {
        ofSetColor(255, 100, 200);
        float w = ofMap(plotter.percentDone, 0, 100, 0, ofGetWidth()-padding*2);
        ofRect(0, displaySize+padding, w, progressHeight);
    }

    ofSetHexColor(0xffffff);
    srcImg.draw(0, 0, displaySize, displaySize);
    
    ofTranslate(displaySize + padding, 0);
    postImg.draw(0, 0, displaySize, displaySize);
    
    ofTranslate(displaySize + padding, 0);

    displayPlotting(displaySize);
    
    
    
}

void ofApp::keyPressed(int key) {


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
    
    this->paths.clear();

    
    for (int n = 0; n < numColours-1; n++) {
        
        vector<PenPoint> ps;
        
        for (int i = 0; i < pts.size(); i++) {
            int x = int(round(pts[i].x));
            int y = int(round(pts[i].y));
            ofColor b = pix.getColor(x, y);

            if (int(b.r) == cols[n]) {

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

void ofApp::displayPlotting(float toSize) {
    
    float d = toSize / targetSize;
    for(auto it = paths.begin(); it != paths.end(); ++it) {
        
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
        }
    }
}
