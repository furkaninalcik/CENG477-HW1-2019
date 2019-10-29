#include "Vec3f.h"
#include "defs.h"

#include <cmath> // MAKE SURE THAT IT IS ALLOWED



Vec3f::Vec3f(){
    //printf("\n empty constructor \n");
}

Vec3f::Vec3f(Vector3f vector) : x(vector.x), y(vector.y), z(vector.z) {

}

Vec3f::Vec3f(float x, float y, float z) : x(x), y(y), z(z) {}

Vec3f Vec3f::operator * (float d) const { 
    //printf("Distance: %lf\n", d );
    //printf("MULTIPLICATION\n");
    return Vec3f(x*d, y*d, z*d); 
}

Vec3f Vec3f::operator + (Vec3f v) const { 
    return Vec3f(x+v.x, y+v.y, z+v.z); 
}

Vec3f Vec3f::operator - (Vec3f v) const { 
    return Vec3f(x-v.x, y-v.y, z-v.z); 
}

Vec3f Vec3f::operator = (Vector3f vector) const { 

    return Vec3f(vector); 
}

Vec3f Vec3f::operator-() const {
    Vec3f v;
    v.x = -x;
    v.y = -y;
    v.z = -z;
    return v;
}

float Vec3f::norm(){

    return sqrt(x*x + y*y + z*z);
}

Vec3f Vec3f::normalize() const {
    float length = sqrt(x*x + y*y + z*z);

    if (length > 0)
    {
        return Vec3f(x/length,y/length,z/length);
    }
    else{
        return *this;
    }


}


Vec3f Vec3f::crossProduct(Vec3f u , Vec3f v ){

    Vec3f result = Vec3f( (u.y*v.z - u.z*v.y) , (u.z*v.x - u.x*v.z) , (u.x*v.y - u.y*v.x) );

    return result;

}

float Vec3f::dotProduct(Vec3f u , Vec3f v){

    return (u.x*v.x + u.y*v.y + u.z*v.z);

}

float Vec3f::dotProduct(Vec3f v){

    return (x*v.x + y*v.y + z*v.z);
    
}


Vec3f Vec3f::clamp(Vec3f vector) {
  Vec3f v ;
  v.x = (vector.x > 255) ? 255 : (vector.x < 0) ? 0 : vector.x;
  v.y = (vector.y > 255) ? 255 : (vector.y < 0) ? 0 : vector.y;
  v.z = (vector.z > 255) ? 255 : (vector.z < 0) ? 0 : vector.z;
  return v;
}


/*
Vec3f normalize() const {
    //float norm = sqrt(x*x + y*y + z*z);
    //return Vec3f(x/norm,y/norm,z/norm);
    return Vec3f(1,1,1);
}
*/

