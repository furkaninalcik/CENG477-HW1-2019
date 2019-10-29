#include "defs.h"

using namespace std;


class Vec3f 
{
    public:

    float x, y, z;

    Vec3f();
    Vec3f(Vector3f vector);
    Vec3f(float x, float y, float z) ;
    
    Vec3f operator* (float d)const;
    Vec3f operator+ (Vec3f v)const;
    Vec3f operator- (Vec3f v)const;
    Vec3f operator= (Vector3f vector) const;
    Vec3f operator- () const;

    float norm();

    Vec3f normalize() const;

    Vec3f crossProduct(Vec3f u , Vec3f v );

    float dotProduct(Vec3f u , Vec3f v );
    float dotProduct( Vec3f v );
    Vec3f clamp(Vec3f vector);
    
};