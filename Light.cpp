#include "Light.h"
#include "Vec3f.h" //NOT IN THE ORIGINAL FILE

/* Constructor. Implemented for you. */
PointLight::PointLight(const Vector3f & position, const Vector3f & intensity)
    : position(position), intensity(intensity)
{
}

// Compute the contribution of light at point p using the
// inverse square law formula
Vector3f PointLight::computeLightContribution(const Vector3f& p)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */

	Vec3f point_vector 	  = p;
	Vec3f position_vector = position;

	Vec3f intensity_vector = intensity;

	float distance = (point_vector - position_vector).norm();

	intensity_vector = intensity_vector * (1/(distance*distance));

	Vector3f intensity_result ;
	intensity_result.x = intensity_vector.x; 
	intensity_result.y = intensity_vector.y; 
	intensity_result.z = intensity_vector.z; 


	return intensity_result;

}
