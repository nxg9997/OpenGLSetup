#pragma once

#include <vector>
#include <iostream>
#include <GL/freeglut.h>

#include "Drawing.h"
#include "callbacks.h"


// Defines
#define elif else if
#define OSTOGL_Y(y,h) h-y
#define MAP(val,os,oe,ts,te) (float)ts + (((float)te - (float)ts) / ((float)oe - (float)os)) * ((float)val - (float)os)
#define SAFEDELETE(pointer) delete pointer; pointer = nullptr;
#define COLORTOGL(num) (float)num / 255.f