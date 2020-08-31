//
//  Lsystem.cpp
//  CSE167_final
//
//  Created by 王源 on 12/8/18.
//  Copyright © 2018 Yan Sun. All rights reserved.
//

#include "Lsystem.hpp"
#include <iostream>
//vector<char> initStr;
//vector<char> finalStr;
//
//
//
//Lsystem(string filename);
//~Lsystem();
//
//void readFile(string filename);
//void produce();

Lsystem::Lsystem(string filename){
    this->filename = filename;
    cout <<"Generating L-System data..."<<endl;
    ifstream file(filename);
    if (!file)
    {
        cout <<"Could not open L file "<<filename<<endl;
        printf("\n\n\n\n\n\n");
        return;
    }
    int numbersRead = 0;
    float numbers[3];
    
    while (!file.eof()){
        string temp;
        char c;
        file >>c;
        if (c == '#'){
            getline(file, temp);
            continue;
        }
        file.putback(c);
        
        if (numbersRead<3)
        {
            file >>numbers[numbersRead++];
            continue;
        }
        if (c == '@')break;
        
        
        getline(file, temp);
        int equalPos = temp.find("=");
        if (equalPos == string::npos){
            initStr = vector<char>(temp.begin(), temp.end());
            
        }
        else{
            string index = temp.substr(0,equalPos);
            if (index == "A")rules[0] = temp.substr(equalPos+1);
            else if (index == "B")rules[1] = temp.substr(equalPos+1);
            else if (index == "C")rules[2] = temp.substr(equalPos+1);
            else if (index == "S")rules[3] = temp.substr(equalPos+1);
            
        }
        
        
       
        
        
        
    }
    int iterations = (int) numbers[0];
    printf("iterations == %d \n\n\n", iterations);
    for (int i = 0; i < iterations; i++)produce();
    for (const char &c : finalStr)cout<<c;
    
}

void Lsystem::produce(){
    
    finalStr.clear();
    
    for (int i = 0; i < initStr.size(); i++){
        
        if (initStr[i] == 'A'){
            for (int j = 0; j < rules[0].size(); j++)
                finalStr.push_back(rules[0][j]);
            
        }else if (initStr[i] == 'B'){
            for (int j = 0; j < rules[1].size(); j++)
                finalStr.push_back(rules[1][j]);
            
        }else if (initStr[i] == 'C'){
            for (int j = 0; j < rules[2].size(); j++)
                finalStr.push_back(rules[2][j]);
            
        }else if (initStr[i] == 'S'){
            for (int j = 0; j < rules[3].size(); j++)
                finalStr.push_back(rules[3][j]);
        }else{
            // just copy
            finalStr.push_back(initStr[i]);
            
        }
        
    }
    initStr.clear();
    initStr = vector<char>(finalStr);
    
}

