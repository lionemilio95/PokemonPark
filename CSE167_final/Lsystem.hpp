//
//  Lsystem.hpp
//  CSE167_final
//
//  Created by YUAN WANG on 12/8/18.
//  Copyright © 2018 Yan Sun. All rights reserved.
//

#ifndef Lsystem_hpp
#define Lsystem_hpp

#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class Lsystem{
public:
    vector<char> initStr;
    vector<char> finalStr;
    string rules[10];
    string filename;
    /*
     reserved rules form:
     rules[0] = A
     rules[1] = B
     rules[2] = C
     rules[3] = S
     
     */
    
    Lsystem(string filename);
    ~Lsystem();
    
    void readFile(string filename);
    void produce();
    
};


#endif /* Lsystem_hpp */
