#pragma once
typedef enum {
    FACE_NORMAL,
    FACE_HAPPY,
    FACE_SAD,
    FACE_SURPRISED,
    FACE_SLEEP,
    FACE_TALKING
} face_expression_t;

void face_init(void);
void face_set(face_expression_t e);
void face_update(void);
void face_draw(void);
