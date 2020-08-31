//
//  Terrian.cpp
//  CSE167_final
//
//  Created by 王源 on 12/4/18.
//  Copyright © 2018 Yan Sun. All rights reserved.
//

#include <stdio.h>
#include "Terrain.h"
#include "Window.h"
float Terrain::doubleRand(float scale) {
    return scale * (float(rand()) / (float(RAND_MAX) + 1.0));
}




void Terrain::setUp(){
    // initial x & z coord value of terrain map
    float xStart = -((size - 1)/2) * stride + oriX;
    float zStart = -((size - 1)/2) * stride + oriZ;
    
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            vertices[i][j] = glm::vec3(xStart + i * stride, seaLevel, zStart + j*stride);
            //printf("i = %d j=%d %f %f %f\n", i, j, vertices[i][j].x,vertices[i][j].y,vertices[i][j].z);
            
        }
        
    }
    
    for (int i = 0; i < size - 1; i++){
        
        for (int j = 0; j < size - 1; j++){
            indices[i * (size-1) + (j)][0]= (i) * size + (j);
            indices[i * (size-1) + (j)][1]= (i) * size + (j+1);
            indices[i * (size-1) + (j)][2]=(i+1) * size + (j);
            indices[i * (size-1) + (j)][3]=(i) * size + (j+1);
            indices[i * (size-1) + (j)][4]=(i+1) * size + (j+1);
            indices[i * (size-1) + (j)][5]=(i+1) * size + (j);
            
        }
    }
    
    //for (int i = 1; i < 100; i ++)printf("random from 0 to 10 is %f\n", doubleRand(10));
    
    
    float randMax = heightLimit;
    for (int step = size-1; step > 1; step /= 2){
        //printf("STEP = %d\n\n\n", step);
        for (int i = 0; i < size-1; i += step){
            for (int j = 0; j < size-1; j += step){
                diamond(i,j,step, randMax);
                
            }
            
        }
        for (int i = 0; i < size-1; i += step){
            for (int j = 0; j < size-1; j += step){
                
                square(i,j,step, randMax);
            }
            
        }
        randMax *= randScaler;
        
    }
    
    //    for (int i = 0; i < size ;i++)
    //        for (int j = 0; j < size;j++)
    //            printf("%f\n", vertices[i][j].y);
    
    
    // calculate outline
    for (int i = 0; i < size-1 ;i++){
        for (int j = 0; j < size - 1;j++){
            outlineVertices.push_back(vertices[i][j]);
            outlineVertices.push_back(vertices[i][j+1]);
            outlineVertices.push_back(vertices[i+1][j+1]);
            outlineVertices.push_back(vertices[i+1][j]);
            outlineVertices.push_back(vertices[i][j]);
            
            
        }
        for (int j = size - 2; j >=0; j--){
            outlineVertices.push_back(vertices[i][j]);
            
        }
        
    }
    
    
    
    
    //for(int i = 0; i < size; i++) for (int j = 0; j < size; j++)printf("%f %f %f\n", vertices[i][j].x,vertices[i][j].y,vertices[i][j].z);
    
    //printf("SIZE = %d \n\n\n", sizeof(vertices));
    
    toWorld = glm::mat4(1.0f);
    
    // Create array object and buffers. Remember to delete your buffers when the object is destroyed!
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO);
    
    // Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
    // Consider the VAO as a container for all your buffers.
    glBindVertexArray(VAO);
    
    // Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
    // you want to draw, such as vertices, normals, colors, etc.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
    // the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
    glBufferData(GL_ARRAY_BUFFER, size * size * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);
    // Enable the usage of layout location 0 (check the vertex shader to see what this is)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    
    // We've sent the vertex data over to OpenGL, but there's still something missing.
    // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO now so we don't accidentally tamper with it.
    // NOTE: You must NEVER unbind the element array buffer associated with a VAO!
    glBindVertexArray(0);
    
    
    //buffers for outline
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, (size-1) * (size-1)*(5+1) * sizeof(glm::vec3), outlineVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(GLfloat),(GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    

    
}

Terrain::Terrain(float stride, float heightLimit, float seaLevel, float randScaler, float x, float z){
    this->stride = stride;
    this->heightLimit = heightLimit;
    this->seaLevel = seaLevel;
    this->randScaler = randScaler;
    this->oriX = x;
    this->oriZ = z;
    setUp();
    
    
}

Terrain::Terrain(){
    setUp();
    
}

Terrain::~Terrain(){}

void Terrain::draw(GLuint shaderProgram)
{
    // Calculate the combination of the model and view (camera inverse) matrices
    glm::mat4 modelview = Window::V * toWorld;
    // We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
    // Consequently, we need to forward the projection, view, and model matrices to the shader programs
    // Get the location of the uniform variables "projection" and "modelview"
    uProjection = glGetUniformLocation(shaderProgram, "projection");
    uModelview = glGetUniformLocation(shaderProgram, "modelview");
    // Now send these values to the shader program
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    glUniform1f(glGetUniformLocation(shaderProgram, "isOutline"), 0);
    
    
    // Now draw the cube. We simply need to bind the VAO associated with it.
    glBindVertexArray(VAO);
    // Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
    glDrawElements(GL_TRIANGLES, (size)*(size)*6, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_LINE_STRIP, 0, size*size);
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
    
    if (drawOutline == false)return;
    
    glUniform1f(glGetUniformLocation(shaderProgram, "isOutline"), 1);
    glBindVertexArray(VAO2);
    // Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
    glDrawArrays(GL_LINE_STRIP, 0, (size-1) * (size-1) * 6);
    //glDrawArrays(GL_LINE_STRIP, 0, size*size);
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
    
    
}


void Terrain::diamond(int x, int y, int s, float r){
    // r is height's random limit
    
    
    
    
    if (s <= 1)return;
    //if (s < limit)return;
    
    srand((unsigned)time(NULL));
    //rand()
    
    //for (int i = 0; i < 10; i++)printf("rand = %d\n",rand());
    //printf("MAX_RAND = %d \n", RAND_MAX);
    
    
    int x1 = x;
    int y1 = y;
    
    int x2 = x1;
    int y2 = y1 + s;
    
    int x3 = x1 + s;
    int y3 = y1;
    
    int x4 = x1 + s;
    int y4 = y1 + s;
    
    int xm = x1 + (s/2);
    int ym = y1 + (s/2);
    
    float randHeight = doubleRand(r);
    
    vertices[xm][ym].y = vertices[x1][y1].y + vertices[x2][y2].y + vertices[x3][y3].y + vertices[x4][y4].y;
    vertices[xm][ym].y /= 4;
    vertices[xm][ym].y += randHeight;
    
    //printf("new vertices[%d][%d].y = %f\n", xm, ym, vertices[xm][ym].y);
    
    
    return;
}

void Terrain::square(int x, int y, int s, float r){
    
    
    
    if (s <= 1)return;
    srand((unsigned)time(NULL));
    
    int x1 = x + (s / 2);
    int y1 = y ;
    
    int x2 = x;
    int y2 = y + (s / 2);
    
    int x3 = x1;
    int y3 = y + s;
    
    int x4 = x + s;
    int y4 = y2;
    
    
    if (vertices[x1][y1].y == seaLevel){
        float randHeight = doubleRand(r);
        int cnt = 3;
        vertices[x1][y1].y = vertices[x][y].y + vertices[x1][y2].y + vertices[x4][y1].y;
        if ((y1 - (s/2)) >=0 ){
            cnt ++;
            vertices[x1][y1].y += vertices[x1][y1-(s/2)].y;
            
        }
        vertices[x1][y1].y /= cnt;
        vertices[x1][y1].y += randHeight;
        
        
    }
    
    if (vertices[x2][y2].y == seaLevel){
        float randHeight = doubleRand(r);
        int cnt = 3;
        vertices[x2][y2].y = vertices[x][y].y + vertices[x1][y2].y + vertices[x2][y3].y;
        if ((x2 - (s/2)) >=0 ){
            cnt ++;
            vertices[x2][y2].y += vertices[x2 - (s/2)][y2].y;
            
        }
        vertices[x2][y2].y /= cnt;
        vertices[x2][y2].y += randHeight;
        
    }
    
    if (vertices[x3][y3].y == seaLevel){
        float randHeight = doubleRand(r);
        int cnt = 3;
        vertices[x3][y3].y = vertices[x2][y3].y + vertices[x1][y2].y + vertices[x4][y3].y;
        if ((y3 + (s/2)) < size ){
            cnt ++;
            vertices[x3][y3].y += vertices[x3][y3+(s/2)].y;
            
        }
        vertices[x3][y3].y /= cnt;
        vertices[x3][y3].y += randHeight;
        
    }
    
    if (vertices[x4][y4].y == seaLevel){
        float randHeight = doubleRand(r);
        int cnt = 3;
        vertices[x4][y4].y = vertices[x4][y1].y + vertices[x1][y4].y + vertices[x4][y3].y;
        if ((x4 + (s/2)) < size ){
            cnt ++;
            vertices[x4][y4].y += vertices[x4 + (s/2)][y4].y;
            
        }
        vertices[x4][y4].y /= cnt;
        vertices[x4][y4].y += randHeight;
        
    }
//    printf("new vertices[%d][%d].y = %f\n", x1, y1, vertices[x1][y1].y);
//    printf("new vertices[%d][%d].y = %f\n", x2, y2, vertices[x2][y2].y);
//    printf("new vertices[%d][%d].y = %f\n", x3, y3, vertices[x3][y3].y);
//    printf("new vertices[%d][%d].y = %f\n", x4, y4, vertices[x4][y4].y);
//
    
    return;
}
