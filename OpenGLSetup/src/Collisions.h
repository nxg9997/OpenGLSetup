#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/constants.hpp>

#include "Camera.h"

#include "Sphere.h"
#include "Box.h"
#include "Light.h"

#include "Text.h"

#include <fstream>
#include <iostream>

/// - Returns an array of intersection points between a sphere and a line defined by 2 vectors
glm::vec3 * FindCollisionSphere(glm::vec3 origin, glm::vec3 dir, Sphere sphere);

/// - Returns an array of collisions between a Ray (defined by 2 points) and a Box
glm::vec3 * FindCollisionBox(glm::vec3 origin, glm::vec3 dir, Box box);