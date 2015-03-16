//
//  LSystem.h
//  ofApp
//
//  Created by Will Gallia on 16/03/2015.
//
//

#pragma once

#include <vector>
#include <utility>

typedef std::pair<char, std::string> Rule;

class LSystem {
    std::vector<Rule> rules;
    
public:

    void addRule(const Rule &rule) {
        rules.push_back(rule);
    }

    std::string replace(std::string input) {
        std::string r = "";
        for (int i = 0; i < input.length(); i++) {
            bool found = false;
            for (int j = 0; j < rules.size(); j++) {
                if (input[i] == rules[j].first) {
                    r+= rules[j].second;
                    found = true;
                }
            }
            if (!found) {
                r+= std::string(&input[i], 1);
            }
        }
        return r;
    }
    
    
    std::string apply(std::string input, int n) {
        if (n == 1) {
            return replace(input);
        }
        else return apply(replace(input), n-1);
    }
};






