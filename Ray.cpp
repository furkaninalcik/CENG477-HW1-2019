#include "Ray.h"
#include "Vec3f.h"

Ray::Ray()
{
}

Ray::Ray(const Vector3f& origin, const Vector3f& direction)
    : origin(origin), direction(direction)
{
}

/* Takes a parameter t and returns the point accoring to t. t is the parametric variable in the ray equation o+t*d.*/
Vector3f Ray::getPoint(float t) const 
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */

    Vec3f origin_point = Vec3f(origin) ;
    Vec3f direction_vector = Vec3f(direction) ;

    Vec3f result = origin_point + (direction_vector*t);

    Vector3f point ;

    point.x = result.x ;
    point.y = result.y ;
    point.z = result.z ;
    return point;


}

/* Takes a point p and returns the parameter t according to p such that p = o+t*d. */
float Ray::gett(const Vector3f & p) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */

    // t = (p-o)/d


    Vec3f origin_point = Vec3f(origin) ;
    Vec3f direction_vector = Vec3f(direction) ;
    Vec3f p_point = Vec3f(p) ;

    float t = (p_point - origin_point).norm() / direction_vector.norm();

    return t;

}

