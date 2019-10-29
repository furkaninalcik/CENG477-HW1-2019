#include "Shape.h"
#include "Scene.h"
#include <cstdio>

#include <cmath> // NOT IN THE ORIGINAL FILE

Shape::Shape(void)
{
}

Shape::Shape(int id, int matIndex)
    : id(id), matIndex(matIndex)
{
}

Sphere::Sphere(void)
{}

/* Constructor for sphere. You will implement this. */
Sphere::Sphere(int id, int matIndex, int cIndex, float R, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */

    this->cIndex = cIndex;
    this->R = R;
    this->vertices = pVertices;


}

/* Sphere-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Sphere::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */

    float t;
    ReturnVal intersection_info;

    Vec3f e = ray.origin;
    Vec3f d = ray.direction;

    //Vec3f center = Vec3f(-0.875, 1, -2);
    Vec3f center = (*vertices)[cIndex-1];

    float r = R; // radius of the sphere


    float a = d.dotProduct(d);           // a is A in the equation -> At^2 + Bt + C = 0 // 
    float b = 2*(d.dotProduct(e-center));       // b is B in the equation -> At^2 + Bt + C = 0 // 
    float c = (e-center).dotProduct(e-center) - r*r; // c is C in the equation -> At^2 + Bt + C = 0 // 

    float discriminant = b*b - 4*a*c;

    if (discriminant < 0.005) // 
    {
        intersection_info.intersection = false;
        //return false;
    }
    else{
        float x0 = (-b - sqrt(discriminant))/(2*a); // one of the real roots of the equation
        float x1 = (-b + sqrt(discriminant))/(2*a); // one of the real roots of the equation
        t = (x0 < x1) ? x0 : x1;
        //printf("t1 %lf \n", x0 );
        //printf("t2 %lf \n", x1 );
        if (t < 0)
        {   
            //std::cout << "Sphere Intersection" << t << endl;
            //std::cout << "Negative T: " << t << endl;
            
            intersection_info.intersection = false;

        } else{

            Vec3f pointOnTheSphere  = e + d*t; 

            Vector3f intersectionPoint;
            intersectionPoint.x = pointOnTheSphere.x; 
            intersectionPoint.y = pointOnTheSphere.y; 
            intersectionPoint.z = pointOnTheSphere.z;

            Vector3f surfaceNormal;
            Vec3f surfaceNormal_Temp = (pointOnTheSphere - center) * (1.0 / r);
            surfaceNormal.x = surfaceNormal_Temp.x; 
            surfaceNormal.y = surfaceNormal_Temp.y; 
            surfaceNormal.z = surfaceNormal_Temp.z;

            intersection_info.intersection_point = intersectionPoint;
            intersection_info.surface_normal = surfaceNormal;
            intersection_info.intersection = true;
            intersection_info.t = t;

            //return true;     
        }

   
    }

    return intersection_info;

    //Vec3f c = sphere.vertex_data[scene.center_vertex_id]; // center of the sphere
}

Triangle::Triangle(void)
{}

/* Constructor for triangle. You will implement this. */
Triangle::Triangle(int id, int matIndex, int p1Index, int p2Index, int p3Index, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */

    this->p1Index = p1Index;
    this->p2Index = p2Index;
    this->p3Index = p3Index;
    this->vertices = pVertices;


}

/* Triangle-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Triangle::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */

    ReturnVal intersection_info;

    Vec3f e = ray.origin; // origin 
    Vec3f d = ray.direction; // direction

    Vec3f p ; // the ray-plane intersection point (may or may not be inside the triangle) 

    float gama, beta; // variables for barycentric coordinates

    float t;


    Vec3f v1 = (*vertices)[p1Index-1];
    Vec3f v2 = (*vertices)[p2Index-1];
    Vec3f v3 = (*vertices)[p3Index-1];

    // calculating plane normal

    Vec3f vector_for_cross_product;


    Vec3f normalVector = vector_for_cross_product.crossProduct( v3-v2 , v2-v1);  // BE CAREFULL ABOUT THE ORDER OF THE VERTICES
    Vec3f surfaceNormal_Temp = -normalVector; // TO BE USED BY SHADING PART OF THE CODE

    if (normalVector.dotProduct(d)  < 0.000001) // if plane and ray are parallel 
    {
        intersection_info.intersection = false;
        return intersection_info;
        //return false;
    }

    t = ((v1 - e).dotProduct(normalVector))/(d.dotProduct(normalVector)); // calculating t to find the ray-plane intersection point "p"


    //printf("t : %lf \n" , t);

    p = e + d * t;



    if (t < 0)
    {   
        //std::cout << "Triangle Intersection" << t << endl;
        //std::cout << "Negative T: " << t << endl;
        intersection_info.intersection = false;
        return intersection_info;
    }




    //printf("TEST1\n");

    /*
    if (t <= 0.000001) // t_min
    {
        return false;
    }
    */

    //printf("TEST2\n");

    /////////////////////////////////////////////

    //calculating the barycentric coordanates
    

    /*

    https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates


    // Compute barycentric coordinates (u, v, w) for
    // point p with respect to triangle (a, b, c)
    void Barycentric(Point p, Point a, Point b, Point c, float &u, float &v, float &w)
    {
        Vector v0 = b - a, v1 = c - a, v2 = p - a;
        float d00 = Dot(v0, v0);
        float d01 = Dot(v0, v1);
        float d11 = Dot(v1, v1);
        float d20 = Dot(v2, v0);
        float d21 = Dot(v2, v1);
        float denom = d00 * d11 - d01 * d01;
        v = (d11 * d20 - d01 * d21) / denom;
        w = (d00 * d21 - d01 * d20) / denom;
        u = 1.0f - v - w;
    }

    */


    //a = v1 
    //b = v2 
    //c = v3 
    //v0 = v_21 
    //v1 = v_31 
    //v2 = v_p1 

    Vec3f v_21 = v2-v1;
    Vec3f v_31 = v3-v1;
    Vec3f v_p1 = p-v1;

    float p1 = v_21.dotProduct(v_21);
    float p2 = v_21.dotProduct(v_31);
    float p3 = v_31.dotProduct(v_31);
    float p4 = v_p1.dotProduct(v_21);
    float p5 = v_p1.dotProduct(v_31);


    float den = p1*p3 - p2*p2; // denominator

    gama = (p3*p4 - p2*p5) / den; // GAMA OR BETA ???

    //printf("GAMA : %lf \n", gama);

    if (gama < 0 || gama > 1 )
    {
        intersection_info.intersection = false;
        return intersection_info;
        //return false;
    }

    //printf("TEST3\n");


    beta = (p1*p5 - p2*p4) / den; // BETA OR GAMA ???

    if (beta < 0 || beta > 1-gama)
    {
        intersection_info.intersection = false;
        return intersection_info;

        //return false;
    }

    //printf("TEST4\n");
    else{

        Vec3f pointOnTheTriangle  = p; 

        Vector3f intersectionPoint;
        intersectionPoint.x = pointOnTheTriangle.x; 
        intersectionPoint.y = pointOnTheTriangle.y; 
        intersectionPoint.z = pointOnTheTriangle.z;

        Vector3f surfaceNormal;

        surfaceNormal.x = surfaceNormal_Temp.x; 
        surfaceNormal.y = surfaceNormal_Temp.y; 
        surfaceNormal.z = surfaceNormal_Temp.z;

        intersection_info.intersection_point = intersectionPoint;
        intersection_info.surface_normal = surfaceNormal;
        intersection_info.intersection = true;
        intersection_info.t = t;

        return intersection_info;

    //return true;

    }
        





}

Mesh::Mesh()
{}

/* Constructor for mesh. You will implement this. */
Mesh::Mesh(int id, int matIndex, const vector<Triangle>& faces, vector<int> *pIndices, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */

    this->faces = faces;
    this->pIndices = pIndices;
    this->vertices = pVertices;



}

/* Mesh-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Mesh::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */

    int t_min = 100000.0;

    int num_of_faces = faces.size();

    ReturnVal intersection_info_array[num_of_faces];

    ReturnVal first_intersection_info ;
    
    ReturnVal face_intersection_info ;
    ReturnVal closest_intersection_info ;

    //std::cout << faces.size() << endl;


    for (int i = 0; i < faces.size(); ++i)
    {    
        //intersection_info_array[i] = faces[i].intersect(ray); //assigning the list of intersection_infos of the faces
     

        face_intersection_info = faces[i].intersect(ray);

        if (face_intersection_info.intersection && face_intersection_info.t <= t_min)
        {

            t_min = face_intersection_info.t;
            closest_intersection_info = face_intersection_info;
            
        }


        //first_intersection_info = faces[i].intersect(ray);
        /*
        if(first_intersection_info.intersection){
            break;
        }
        */

    }


    //return intersection_info_array[0]; // returning the first element of the array that is the info about the first face of the mesh
    return closest_intersection_info; // returning the intersection_info of the first intersection without checking if it is the closest face to the camera 


}
