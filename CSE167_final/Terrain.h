//
//  Terrain.cpp
//  CSE167_final
//
//  Created by Yuan Wang on 12/4/18.
//  Copyright © 2018 Yuan Wang. All rights reserved.
//
#ifndef _TERRAIN_H_
#define _TERRAIN_H_
#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>


class Terrain{
    
public:
    unsigned int VAO;
    unsigned int VBO, EBO;
    
    unsigned int VAO2, VBO2;//for outline
    
    glm::mat4 toWorld;
    GLuint uProjection, uModelview;
    bool drawOutline = false;
    
    float oriX = 0;
    float oriZ = 0;
    static const int size = 513;
    
    
    float stride = 0.5;
    float heightLimit = 40;
    float seaLevel = -14;
    float randScaler = 0.4;
    
    glm::vec3 vertices[size][size];
    GLuint indices[(size - 1)*(size - 1)][6];
    
    // this vector is for render a outline shape of terrain 
    std::vector<glm::vec3> outlineVertices;
    
    
    
    Terrain();
    Terrain(float stride, float heightLimit, float seaLevel, float randScaler, float x, float z);
    ~Terrain();
    void setUp();
    void draw(GLuint shaderProgram);
    void diamond(int x, int y, int s, float r);
    void square(int x, int y, int s, float r);
    float doubleRand(float scale);
    
};

#endif
