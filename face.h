#ifndef FACE_H
#define FACE_H

typedef enum
{
    FACE_NORMAL,
    FACE_HAPPY,
    FACE_SAD,
    FACE_SLEEP,
    FACE_SURPRISED,
    FACE_TALKING

} FaceExpression;


void face_init(void);

void update_face(void);

void draw_face(void);

void set_expression(
    FaceExpression expression
);

FaceExpression get_expression(void);


#endif