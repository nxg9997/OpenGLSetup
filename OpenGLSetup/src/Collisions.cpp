#include "Collisions.h"

glm::vec3 * FindCollisionSphere(glm::vec3 origin, glm::vec3 dir, Sphere sphere)
{
	//std::cout << "=== Sphere Collision ===" << std::endl;

	// get the values ofr the quatratic formula
	float* quadVals = FindQuadValues(origin, dir, sphere);

	// find the discriminant
	float disc = (pow(quadVals[1], 2) - 4 * quadVals[0] * quadVals[2]);

	// check for a collision, return nullptr if there is no collision
	if (disc < 0)
	{
		//std::cout << "No collision detected" << std::endl;
		return nullptr;
	}

	// find point(s) of intersection

	// find the results of the quadratic formula
	float pT = (-quadVals[1] + glm::sqrt(disc)) / (2 * quadVals[0]);
	float nT = (-quadVals[1] - glm::sqrt(disc)) / (2 * quadVals[0]);

	// first collision point
	float x1 = origin.x + ((dir.x - origin.x) * pT);
	float y1 = origin.y + ((dir.y - origin.y) * pT);
	float z1 = origin.z + ((dir.z - origin.z) * pT);
	glm::vec3 first(x1, y1, z1);
	std::cout << "First Collision: <" << x1 << ", " << y1 << ", " << z1 << ">" << std::endl;

	// 2nd collision point
	float x2 = origin.x + ((dir.x - origin.x) * nT);
	float y2 = origin.y + ((dir.y - origin.y) * nT);
	float z2 = origin.z + ((dir.z - origin.z) * nT);
	glm::vec3 second(x2, y2, z2);
	std::cout << "Second Collision: <" << x2 << ", " << y2 << ", " << z2 << ">" << std::endl;

	glm::vec3 * result = new glm::vec3[2];
	result[0] = first;
	result[1] = second;

	//if (result[0].x == -NAN || result[1].x == -NAN) return nullptr;

	return result;
}

float * FindQuadValues(glm::vec3 origin, glm::vec3 dir, Sphere sphere)
{
	// array holds a, b, and c from the quadratic formula
	float* result = new float[3];
	result[0] = pow((dir.x - origin.x), 2) + pow((dir.y - origin.y), 2) + pow((dir.z - origin.z), 2);
	result[1] = -2 * ((dir.x - origin.x) * (sphere.pos.x - origin.x) + (dir.y - origin.y) * (sphere.pos.y - origin.y) + (dir.z - origin.z) * (sphere.pos.z - origin.z));
	result[2] = pow((sphere.pos.x - origin.x), 2) + pow((sphere.pos.y - origin.y), 2) + pow((sphere.pos.z - origin.z), 2) - pow(sphere.radius, 2);
	return result;
}

float * FindPlaneCoeff(glm::vec3 p, glm::vec3 q, glm::vec3 r)
{
	// define the plane using vectors
	glm::vec3 pq = q - p;
	glm::vec3 pr = r - p;
	glm::vec3 n = glm::cross(pq, pr);

	// a(x-x0)+b(y-y0)+c(z-z0) = 0

	// find the coeffients for use in the equation of a plane
	float* result = new float[4];
	result[4] = (n.x * p.x) + (n.y * p.y) + (n.z * p.z);
	result[0] = n.x;
	result[1] = n.y;
	result[2] = n.z;

	return result;
}

glm::vec3 * FindCollisionPlane(glm::vec3 origin, glm::vec3 dir, glm::vec3 p, glm::vec3 q, glm::vec3 r)
{
	// infinate or NaN might crash the program
	try
	{
		// find parametric eq's
		glm::vec3 v = dir - origin;
		// [x,y,z] = a + bt, <r> = <origin> + t<v>

		// get the plane coeff's
		float * P = FindPlaneCoeff(p, q, r);
		//EQ: P[0]x + P[1]y + P[2]z = P[4]
		float tCoeff = (P[0] * (v.x)) + (P[1] * (v.y)) + (P[2] * (v.z));
		float xCoeff = P[0] * dir.x;
		float yCoeff = P[1] * dir.y;
		float zCoeff = P[2] * dir.z;
		float scalar = P[3] - (xCoeff + yCoeff + zCoeff);
		float t = scalar / tCoeff;

		float x = dir.x + (t * v.x);
		float y = dir.y + (t * v.y);
		float z = dir.z + (t * v.z);



		return new glm::vec3(x, y, z);
	}
	catch (exception)
	{
		return nullptr;
	}
	
}

glm::vec3 * FindCollisionBox(glm::vec3 origin, glm::vec3 dir, Box box)
{
	std::cout << "=== Box Collision ===" << std::endl;

	// Max number of collisions is 6 (one for each face)
	glm::vec3* result = new glm::vec3[6];

	// search for all possible collisions
	glm::vec3** intersections = new glm::vec3*[6] {
		FindCollisionPlane(origin, dir, box.minPos, glm::vec3(box.maxPos.x, box.minPos.y, box.minPos.z), glm::vec3(box.maxPos.x, box.minPos.y, box.maxPos.z)),
		FindCollisionPlane(origin, dir, glm::vec3(box.maxPos.x, box.maxPos.y, box.minPos.z), glm::vec3(box.maxPos.x, box.maxPos.y, box.maxPos.z), glm::vec3(box.minPos.x, box.maxPos.y, box.maxPos.z)),
		FindCollisionPlane(origin, dir, glm::vec3(box.minPos.x, box.maxPos.y, box.maxPos.z), glm::vec3(box.maxPos.x, box.maxPos.y, box.maxPos.z), glm::vec3(box.maxPos.x, box.minPos.y, box.maxPos.z)),
		FindCollisionPlane(origin, dir, glm::vec3(box.maxPos.x, box.minPos.y, box.minPos.z), glm::vec3(box.maxPos.x, box.minPos.y, box.maxPos.z), glm::vec3(box.maxPos.x, box.maxPos.y, box.maxPos.z)),
		FindCollisionPlane(origin, dir, glm::vec3(box.minPos.x, box.minPos.y, box.minPos.z), glm::vec3(box.maxPos.x, box.minPos.y, box.minPos.z), glm::vec3(box.maxPos.x, box.maxPos.y, box.minPos.z)),
		FindCollisionPlane(origin, dir, glm::vec3(box.minPos.x, box.minPos.y, box.minPos.z), glm::vec3(box.minPos.x, box.maxPos.y, box.minPos.z), glm::vec3(box.minPos.x, box.maxPos.y, box.maxPos.z))
	};

	// if all the collision points are null (non-existant), return nullptr. Otherwise, print all the collisions out
	int nullCount = 0;
	for (size_t i = 0; i < 6; i++)
	{
		if (intersections[i] == nullptr)
		{
			nullCount++;
		}
		else
		{
			std::cout << "Collision #" << i << ": <" << intersections[i]->x << ", " << intersections[i]->y << ", " << intersections[i]->z << ">" << std::endl;
		}
	}

	if (nullCount >= 6)
	{
		return nullptr;
	}

	return result;
}
