#include "Collisions.h"

glm::vec3 * FindCollisionSphere(glm::vec3 origin, glm::vec3 dir, Sphere sphere)
{
	vec3 camEye = origin;
	vec3 distance = camEye - sphere.pos;

	// - Calculate values for the formula
	float a = dot(dir, dir);
	float b = 2 * dot(distance, dir);
	float c = dot(distance, distance) - sphere.radius * sphere.radius;
	float disc = b * b - 4 * a * c; // discriminate

	float finalDiscrim = -1;
	vec3* collisionPoint = nullptr;

	// - if the disciminate is less than 0 there is no collision
	if (disc >= 0)
	{
		finalDiscrim = (-b - glm::sqrt(disc)) / (2 * a);
	}
	if (finalDiscrim >= 0)
	{
		collisionPoint = new vec3(camEye + (dir * finalDiscrim));
	}
	return collisionPoint;
}

// - Code referenced from: http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-custom-ray-obb-function/
glm::vec3 * FindCollisionBox(glm::vec3 origin, glm::vec3 dir, Box box)
{
	vec4 boxMin = vec4(box.minPos, 1) * box.rotMat;
	vec4 boxMax = vec4(box.maxPos, 1) * box.rotMat;

	vec3 boxCenter = vec3(((box.maxPos.x - box.minPos.x) / 2 + box.minPos.x), ((box.maxPos.y - box.minPos.y) / 2 + box.minPos.y), (box.maxPos.z - box.minPos.z) / 2 + box.minPos.z);
	mat4 translateMat = glm::translate(mat4(), boxCenter);
	mat4 invRotModelMat = box.invRotMat * translateMat;
	mat4 realTranslateMat = glm::translate(mat4(), vec3(invRotModelMat[3].x / 2, invRotModelMat[3].y / 2, invRotModelMat[3].z / 2));
	mat4 modelMat = box.rotMat * translateMat;

	float tMin = 0.0f;
	float tMax = 100000.0f;

	vec3 OBBposition_worldspace(modelMat[3].x, modelMat[3].y, modelMat[3].z);
	vec3 delta = OBBposition_worldspace - origin;

	// - x axis
	vec3 xaxis(modelMat[0].x, modelMat[0].y, modelMat[0].z);
	float xe = glm::dot(xaxis, delta);
	float xf = glm::dot(dir, xaxis);

	float xt1 = (xe + box.minPos.x) / xf;
	float xt2 = (xe + box.maxPos.x) / xf;

	if (xt1 > xt2)
		swap(xt1, xt2);

	if (xt2 < tMax)
		tMax = xt2;

	if (xt1 > tMin)
		tMin = xt1;

	if (tMax < tMin)
		return nullptr;

	// - y axis
	vec3 yaxis(modelMat[1].x, modelMat[1].y, modelMat[1].z);
	float ye = glm::dot(yaxis, delta);
	float yf = glm::dot(dir, yaxis);

	float yt1 = (ye + box.minPos.y) / yf;
	float yt2 = (ye + box.maxPos.y) / yf;

	if (yt1 > yt2)
		swap(yt1, yt2);

	if (yt2 < tMax)
		tMax = yt2;

	if (yt1 > tMin)
		tMin = yt1;

	if (tMax < tMin)
		return nullptr;

	// - z axis
	vec3 zaxis(modelMat[2].x, modelMat[2].y, modelMat[2].z);
	float ze = glm::dot(zaxis, delta);
	float zf = glm::dot(dir, zaxis);

	float zt1 = (ze + box.minPos.z) / zf;
	float zt2 = (ze + box.maxPos.z) / zf;

	if (zt1 > zt2)
		swap(zt1, zt2);

	if (zt2 < tMax)
		tMax = zt2;

	if (zt1 > tMin)
		tMin = zt1;

	if (tMax < tMin)
		return nullptr;

	return new vec3(xt1, yt1, zt1);
}
