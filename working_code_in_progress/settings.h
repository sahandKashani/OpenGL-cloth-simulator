#ifndef SETTINGS_H
#define SETTINGS_H

#include "Cloth.h"
#include "Camera.h"
#include <sys/time.h>

// Global variables
extern float nearPlane;
extern float farPlane;

extern bool drawWireFrameEnabled;
extern bool drawNodesEnabled;
extern bool drawWorldAxisEnabled;
extern bool drawStructuralConstraintsEnabled;
extern bool drawShearConstraintsEnabled;
extern bool drawStructuralBendConstraintsEnabled;
extern bool drawShearBendConstraintsEnabled;

// camera angle increments in radians
// increment must be power of 2 for precision reasons
extern float angleIncrement;

// camera translation increments
// increment must be power of 2 for precision reasons
extern float translationIncrement;

// array which holds pressed status values of all keyboard keys other than the
// special ones (arrows + F1..F12 keys + home + end ...)
extern char keyboardStatus[256];

// the "timeval" structure contains 2 fields
//     long int tv_sec = number of whole seconds of elapsed time
//     long int tv_usec = number of microseconds of elapsed time (always lower than 1 million)
extern struct timeval oldTime;

// Cloth declaration
extern Cloth* cloth;

// Declare a camera at origin
extern Camera* camera;

#endif