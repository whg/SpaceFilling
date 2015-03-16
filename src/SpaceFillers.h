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
    std::vector<Rule> rules;
    LSystem ls;
    ls.addRule(Rule('A', "-BF+AFA+FB-"));
    ls.addRule(Rule('B', "+AF-BFB-FA+"));
    string s = ls.apply("A", n);
    
    return points(s);
}