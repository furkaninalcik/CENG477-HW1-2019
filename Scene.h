#ifndef _SCENE_H_
#define _SCENE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "Ray.h"
#include "defs.h"
#include "Vec3f.h"

// Forward declarations to avoid cyclic references
class Camera;
class PointLight;
class Material;
class Shape;

using namespace std;

// Class to hold everything related to a scene. 
class Scene
{
public:
	int maxRecursionDepth;			// Maximum recursion depth
	float intTestEps;				// IntersectionTestEpsilon. You will need this one while implementing intersect routines in Shape class
	float shadowRayEps;				// ShadowRayEpsilon. You will need this one while generating shadow rays. 
	Vector3f backgroundColor;		// Background color
	Vector3f ambientLight;			// Ambient light radiance

	vector<Camera *> cameras;		// Vector holding all cameras
	vector<PointLight *> lights;	// Vector holding all point lights
	vector<Material *> materials;	// Vector holding all materials
	vector<Vector3f> vertices;		// Vector holding all vertices (vertex data)
	vector<Shape *> objects;		// Vector holding all shapes

	Scene(const char *xmlPath);		// Constructor. Parses XML file and initializes vectors above. Implemented for you. 

	void renderScene(void);			// Method to render scene, an image is created for each camera in the scene. You will implement this. 

private:
    // Write any other stuff here


	Vec3f diffuseShader( int mat_id, int light_id, Ray ray, Vec3f surface_normal, Vec3f intersection_point);
	Vec3f ambientShader( int mat_id);
	Vec3f specularShader(int mat_id, int light_id, Ray ray, Vec3f surface_normal, Vec3f intersection_point);


    //DECLARE intersectionDetector function
    //Vec3f intersectionDetector(this, Ray& eyeRay, float& t_final, Vec3f& surfaceNormal, char& objInfo_0, int& objInfo_1, int& objInfo_2 );
};

#endif
