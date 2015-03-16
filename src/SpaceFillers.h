//
//  SpaceFillers.h
//  SpaceFilling
//
//  Created by Will Gallia on 16/03/2015.
//
//

#pragma once

#include "math/ofVec2f.h"
#include "LSystem.h"

std::vector<ofVec2f> _points(string s, float dx, float dy) {
    std::vector<ofVec2f> pts;
    ofVec2f p(0, 0);
    ofVec2f d(dx, dy);
    d.normalize();
    
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == 'F') {
            pts.push_back(ofVec2f(p));
            p+= d;
        }
        else if(s[i] == '-') d.rotate(90);
        else if(s[i] == '+') d.rotate(-90);
    }
    pts.push_back(ofVec2f(p));
    return pts;
}

std::vector<ofVec2f> points(string s) {
    return _points(s, 1, 0);
}

std::vector<ofVec2f> hilbert(int n) {

    LSystem ls;
    ls.addRule(Rule('A', "-BF+AFA+FB-"));
    ls.addRule(Rule('B', "+AF-BFB-FA+"));
    string s = ls.apply("A", n);
    
    return points(s);
}

std::vector<ofVec2f> peano(int n) {

    LSystem ls;
    ls.addRule(Rule('R', "RFLFR+F+LFRFL-F-RFLFR"));
    ls.addRule(Rule('L', "LFRFL-F-RFLFR+F+LFRFL"));
    string s = ls.apply("L", n);
    
    return points(s);
}

std::vector<ofVec2f> quadradticGosper(int n) {

    LSystem ls;
    ls.addRule(Rule('X', "XFX-YF-YF+FX+FX-YF-YFFX+YF+FXFXYF-FX+YF+FXFX+YF-FXYF-YF-FX+FX+YFYF-"));
    ls.addRule(Rule('Y', "+FXFX-YF-YF+FX+FXYF+FX-YFYF-FX-YF+FXYFYF-FX-YFFX+FX+YF-YF-FX+FX+YFY"));
    string s = ls.apply("-YF", 2);
    
    return points(s);
}


std::vector<ofVec2f> accordian(int size) {

    std::vector<ofVec2f> pts;
    size--;
    int w = size;
    for (int i = 0; i < size+1; i++, w = w == size ? 0 : size) {
        for (int j = 0; j < size+1; j++) {
            pts.push_back(ofVec2f(i, abs(w-j)));
        }
    }
    return pts;
}

std::vector<ofVec2f> spiral(int size) {

    std::vector<ofVec2f> pts;
    
    int nlines = 2 * size + 1;
    ofVec2f d(1, 0);
    ofVec2f p(0, 0);
    
    pts.push_back(ofVec2f(p));
    for (int i = 0; i < nlines; i++) {
        for (int j = 0; j < ((i == 0) ? 3 : 2); j++) {
            for (int k = 0; k < size; k++) {
                p+= d;
                
                pts.push_back(ofVec2f(p));
            }
            d.rotate(90);
        }
        size--;
    }
    return pts;
}

std::vector<ofVec2f> zigzag(int size) {

    std::vector<ofVec2f> pts;
    
    ofVec2f p(0, 0);
    bool a = true;
    pts.push_back(p);
    float dir = 1;
    ofVec2f edgeStep = ofVec2f(1, 0);
    int t = 0, esr = 1;

    for (int i = 0; i < size*2 - 3; i++) {
        p+= edgeStep;
        pts.push_back(p);
        int q = abs(t - (i+1) % size);
        for (int j = 0; j < q; j++) {
            p+= ofVec2f(-dir, dir);
            pts.push_back(p);
        }
        
        dir = -dir;
        
        if (i == size-2) {
            t = i;
        }
        else {
            edgeStep.rotate(90 * esr);
            esr = -esr;
        }
    }
    
    p+= edgeStep;
    pts.push_back(p);
    
    return pts;
}