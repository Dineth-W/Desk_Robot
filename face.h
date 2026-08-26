#ifndef FACE_H
#define FACE_H

#include <Arduino.h>

// Face Expressions
enum FaceExpression
{
    FACE_NORMAL,
    FACE_HAPPY,
    FACE_SAD,
    FACE_SLEEP,
    FACE_SURPRISED,
    FACE_TALKING
};

// Initialization
void faceInit();

// Updates
void updateFace();

// Drawing
void drawFace();

// Expression Control
void setExpression(FaceExpression expression);
FaceExpression getExpression();
#endif
