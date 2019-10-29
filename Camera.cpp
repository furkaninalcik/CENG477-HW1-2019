#include "Camera.h"
#include "Vec3f.h"


Camera::Camera(int id,                      // Id of the camera
               const char* imageName,       // Name of the output PPM file 
               const Vector3f& pos,         // Camera position
               const Vector3f& gaze,        // Camera gaze direction
               const Vector3f& up,          // Camera up direction
               const ImagePlane& imgPlane)  // Image plane parameters
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
     this->id = id;
     for(int i = 0; i < 32; ++i)
          this->imageName[i] = imageName[i]; //find a better way if there is
     this->imgPlane = imgPlane;

     this->pos = pos;
     this->gaze = gaze;
     this->up = up;

}

/* Takes coordinate of an image pixel as row and col, and
 * returns the ray going through that pixel. 
 */
Ray Camera::getPrimaryRay(int col, int row) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */

          


     Ray gazeRay = Ray(pos, gaze); // the eye ray which is perpendicular to the image plane

     Vec3f e = pos; // camera position, the origin of the rays we trace

     Vec3f w = gaze; // camera gaze vector in xyz coordinates
     Vec3f v = up; // camera up vector in xyz coordinates

     Vec3f vector_for_member_function ; // created to use crossProduct;

     Vec3f u = vector_for_member_function.crossProduct(v,-w); 


     Vec3f s;

     float s_u,s_v;

     int n_x = imgPlane.nx;
     int n_y = imgPlane.ny;

     float distance = imgPlane.distance; 

     float l = imgPlane.left;
     float r = imgPlane.right;
     float b = imgPlane.bottom;
     float t = imgPlane.top;




     // slide -> http://saksagan.ceng.metu.edu.tr/courses/ceng477/files/pdf/week_02.pdf ------- page 13/49




     Vec3f m = e + (w) * distance ;  // m is the intersection point of the gazeRay and the image plane

     Vec3f q = m + u*l + v*t; // find the coordanates of the point "q" (the point at the top-left of image plane )


     Ray eyeRay ;

     s_u = (r - l)*(row + 0.5)/n_x;
     s_v = (t - b)*(col + 0.5)/n_y;


     s = q + (u * s_u) - (v * s_v);


     Vector3f origin_point, direction_vector;
     
     origin_point.x = e.x;
     origin_point.y = e.y;
     origin_point.z = e.z;

     Vec3f normalized_direction_vector = (s-e).normalize();

     direction_vector.x = normalized_direction_vector.x;
     direction_vector.y = normalized_direction_vector.y;
     direction_vector.z = normalized_direction_vector.z;

     eyeRay = Ray(origin_point, direction_vector);

     return eyeRay;


}

