#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Shape.h"
#include "tinyxml2.h"
#include "Image.h" // NOT INCLUDED IN THE ORIGINAL FILE!

#include <cmath> // NOT IN THE ORIGINAL FILE


using namespace tinyxml2;

/* 
 * Must render the scene from each camera's viewpoint and create an image.
 * You can use the methods of the Image class to save the image as a PPM file. 
 */
void Scene::renderScene(void)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */

	const char* filename =  cameras[0]->imageName;
	//std::cout << filename << std::endl;
	//std::cout << cameras[0]->id << std::endl;

    int width = cameras[0]->imgPlane.nx;
    int height = cameras[0]->imgPlane.ny;
    const int numOfImages = cameras.size();
    
    unsigned char* image = new unsigned char [width * height * 3];    

    Image* img = new Image(width,height);

    int recursionTracker = 0;

    std::cout << width << endl;
    std::cout << height << endl; 


    for (int i = 0; i < width; ++i)
    {
    	for (int j = 0; j < height; ++j)
    	{
    		


    		Ray primaryRay = cameras[0]->getPrimaryRay(i,j);

    		Vec3f final_shade = shade(primaryRay, recursionTracker);

			img->setPixelValue(j,i,{final_shade.x,final_shade.y,final_shade.z});
			
   			
    	}
    }
    



 	img->saveImage(filename);


}


Vec3f Scene::shade(Ray primaryRay, int recursionTracker){

	recursionTracker -= 1;

	ReturnVal intersection_info;
	intersection_info.intersection=false;


	for (int k = 0; k < objects.size(); ++k)
	{


		intersection_info = objects[k]->intersect(primaryRay);

		if(intersection_info.intersection){
			
			int mat_id = objects[k]->matIndex;
			
			Vec3f ambient  =  ambientShader(mat_id);

			Vec3f diffuse  = Vec3f(0,0,0);
			Vec3f specular = Vec3f(0,0,0);
			Vec3f mirror   = Vec3f(0,0,0);

			for (int light_id = 0; light_id < lights.size(); ++light_id)
			{

				bool shadowRay_object_intersection = false;
				float epsilon =  0.0001;
				//epsilon =  1000000;


			    Vec3f light_position = lights[light_id]->position;


		    	Vec3f intersection_point_to_light = (light_position - intersection_info.intersection_point).normalize();

		    	Vector3f shadowRay_origin, shadowRay_direction;

		    	shadowRay_origin.x = intersection_info.intersection_point.x + intersection_point_to_light.x * epsilon;
		    	shadowRay_origin.y = intersection_info.intersection_point.y + intersection_point_to_light.y * epsilon;
		    	shadowRay_origin.z = intersection_info.intersection_point.z + intersection_point_to_light.z * epsilon;

		    	shadowRay_direction.x = intersection_point_to_light.x;
		    	shadowRay_direction.y = intersection_point_to_light.y;
		    	shadowRay_direction.z = intersection_point_to_light.z;

		        
		        Ray shadowRay = Ray(shadowRay_origin, shadowRay_direction );


		        float t_to_object;

		        ReturnVal shadowRay_intersection_info;



			    for (int object_id = 0; object_id < objects.size(); ++object_id)
			    {
			        


			        shadowRay_intersection_info = objects[object_id]->intersect(shadowRay);


			        if (shadowRay_intersection_info.intersection){

			            Vec3f intersection_point = intersection_info.intersection_point;
			            Vec3f shadowRay_intersection_point = shadowRay_intersection_info.intersection_point;

			            float light_intersectionPoint_distance    = (light_position - intersection_point).norm();
			            float intersectionPoint_obstacle_distance = (shadowRay_intersection_point - intersection_point).norm();

			            if (light_intersectionPoint_distance > intersectionPoint_obstacle_distance)
			            {
		            		shadowRay_object_intersection = true;
		        			break;
			            }



			        }    
			    }
			    if (!shadowRay_object_intersection)
			    {
					diffuse  = diffuse  + diffuseShader(mat_id, light_id, primaryRay, intersection_info.surface_normal, intersection_info.intersection_point);
					specular = specular + specularShader(mat_id, light_id, primaryRay, intersection_info.surface_normal, intersection_info.intersection_point);

				}



			}


			
		    

			if (recursionTracker > 0)
			{
				Ray ReflectanceRay = mirrorReflectanceRay(primaryRay,intersection_info );
				mirror = shade(ReflectanceRay, recursionTracker );

			    Vec3f mirrorShadingParams = materials[objects[k]->matIndex-1]->mirrorRef; // for RGB values -> between 0 and 1


			    float mirrorShadingRed   = mirrorShadingParams.x * mirror.x; 
			    float mirrorShadingGreen = mirrorShadingParams.y * mirror.y; 
			    float mirrorShadingBlue  = mirrorShadingParams.z * mirror.z; 
			    mirror = Vec3f(mirrorShadingRed, mirrorShadingGreen, mirrorShadingBlue);


			}



			



			Vec3f clamp_vector; 
			Vec3f clamped_shade = clamp_vector.clamp(ambient + diffuse + specular);

			return {clamped_shade.x,clamped_shade.y,clamped_shade.z};

			//img->setPixelValue(j,i,{clamped_shade.x,clamped_shade.y,clamped_shade.z});
			//img->setPixelValue(j,i,{200,200,200});
			


			break;
		}

	}
}

Ray Scene::mirrorReflectanceRay(Ray primaryRay, ReturnVal intersection_info){
	
	Vec3f incomingRayDirection, surfaceNormal;
	
	incomingRayDirection = Vec3f(primaryRay.direction.x, primaryRay.direction.y, primaryRay.direction.z );
	surfaceNormal = Vec3f(intersection_info.surface_normal.x, intersection_info.surface_normal.y, intersection_info.surface_normal.z);

	Vec3f outGoingRayDirection  = incomingRayDirection + (surfaceNormal*(2*(surfaceNormal).dotProduct((-incomingRayDirection))));

	Vector3f direction;
	direction.x = outGoingRayDirection.x;
	direction.y = outGoingRayDirection.y;
	direction.z = outGoingRayDirection.z;

	Ray outGoingRay =  Ray(intersection_info.intersection_point,direction);


	return outGoingRay;


}

Vec3f Scene::mirrorShader(){
        /*
    if (mirrorShadingParams.x != 0 || mirrorShadingParams.y != 0 || mirrorShadingParams.z != 0 )
    {
        printf("MIRROR\n");

        float t1,t2,t3;

        sphereSurfaceNormal = sphereSurfaceNormal.normalize();

        eyeRay.d = eyeRay.d.normalize();

        Vec3f mirrorReflactanceRayDirection  = eyeRay.d + (sphereSurfaceNormal*(2*dotProduct(sphereSurfaceNormal,(-eyeRay.d)))) ;

        Ray mirrorReflactanceRay = Ray(pointOnTheSphere, mirrorReflactanceRayDirection );


        bool sphereIntersection = false;
        bool triangleIntersection = false;
        bool faceIntersection = false;


        Vec3f surfaceNormal; // "intersection" function will assign this variable 


        Vec3f sphereShade   = sphereShading(scene, eyeRay, t1,  lightPosition, lightIntensity,  spheres, image, sphereIntersection, index);


        Vec3f triangleShade = triangleShading(scene, eyeRay, t2,  lightPosition, lightIntensity,  scene.triangles, image, sphereIntersection, triangleIntersection, index, surfaceNormal);
        

        Vec3f faceShade   =  faceShading(scene, eyeRay, t3,  lightPosition, lightIntensity,  scene.meshes, image, sphereIntersection, triangleIntersection, faceIntersection, index, surfaceNormal);


    }


    */
}





Vec3f Scene::diffuseShader(int mat_id, int light_id, Ray ray, Vec3f surface_normal, Vec3f intersection_point){


	Vec3f light_position = lights[light_id]->position;

	Vec3f vectorToLight = light_position - intersection_point;


    float cosTheta = (vectorToLight.normalize()).dotProduct(surface_normal.normalize());

    //printf("COSTHETA: %lf \n", cosTheta );


    cosTheta = (cosTheta < 0) ? 0 : cosTheta;
    //std::cout << cosTheta << endl;



    Vec3f diffuseShadingParams = materials[mat_id-1]->diffuseRef; // for RGB values -> between 0 and 1

    //printf("Diffuse parameters: %lf , %lf , %lf \n", diffuseShadingParams.x, diffuseShadingParams.y, diffuseShadingParams.z );




    //irradiance = lightIntensity * (1.0/(lightDistance*lightDistance));

    Vector3f intersection_point_vector3f ;
    intersection_point_vector3f.x = intersection_point.x; 
    intersection_point_vector3f.y = intersection_point.y; 
    intersection_point_vector3f.z = intersection_point.z; 
/*
    Vec3f irradiance = Vec3f(0,0,0);

    for (int i = 0; i < lights.size(); ++i)
    {
    	irradiance = irradiance + lights[i]->computeLightContribution(intersection_point_vector3f);

    }
*/
    Vec3f irradiance = lights[light_id]->computeLightContribution(intersection_point_vector3f);

    float diffuseShadingRed   = diffuseShadingParams.x * cosTheta * irradiance.x; 
    float diffuseShadingGreen = diffuseShadingParams.y * cosTheta * irradiance.y; 
    float diffuseShadingBlue  = diffuseShadingParams.z * cosTheta * irradiance.z; 

    return Vec3f(diffuseShadingRed,diffuseShadingGreen,diffuseShadingBlue);



}

Vec3f Scene::ambientShader(int mat_id){
    //////////////////////////////////// AMBIENT SHADING

    float ambientRadienceRed   = ambientLight.x;
    float ambientRadienceGreen = ambientLight.y;
    float ambientRadienceBlue  = ambientLight.z;


    Vec3f ambientShadingParams = materials[mat_id-1]->ambientRef; // for RGB values -> between 0 and 1


    float ambientShadingRed   = ambientShadingParams.x * ambientRadienceRed; 
    float ambientShadingGreen = ambientShadingParams.y * ambientRadienceGreen; 
    float ambientShadingBlue  = ambientShadingParams.z * ambientRadienceBlue; 

    Vec3f ambientShading = Vec3f(ambientShadingRed,ambientShadingGreen,ambientShadingBlue);

    return ambientShading;
    //////////////////////////////////// AMBIENT SHADING
}

Vec3f Scene::specularShader(int mat_id, int light_id, Ray ray, Vec3f surface_normal, Vec3f intersection_point){


	Vec3f light_position = lights[light_id]->position;

	Vec3f vectorToLight = light_position - intersection_point;

	Vec3f minus_ray_direction = ray.direction;
	minus_ray_direction = -minus_ray_direction;

    Vec3f halfWayVector = ((minus_ray_direction.normalize()) + vectorToLight.normalize()).normalize();

    float cosAlpha = (halfWayVector.normalize()).dotProduct(surface_normal.normalize()); // for specular shading

    cosAlpha = (cosAlpha < 0) ? 0 : cosAlpha;


    Vec3f specularShadingParams = materials[mat_id-1]->specularRef; // for RGB values -> between 0 and 1
    float phong_exponent = materials[mat_id-1]->phongExp; // for RGB values -> between 0 and 1
    float cosAlphaWithPhong = pow(cosAlpha,phong_exponent); 
    //printf("Specular : %lf %lf %lf  \n", specularShadingParams.x, specularShadingParams.y, specularShadingParams.z   );

    Vector3f intersection_point_vector3f ;
    intersection_point_vector3f.x = intersection_point.x; 
    intersection_point_vector3f.y = intersection_point.y; 
    intersection_point_vector3f.z = intersection_point.z; 

    Vec3f irradiance = lights[light_id]->computeLightContribution(intersection_point_vector3f);


    float specularShadingRed   = specularShadingParams.x * cosAlphaWithPhong * irradiance.x; 
    float specularShadingGreen = specularShadingParams.y * cosAlphaWithPhong * irradiance.y; 
    float specularShadingBlue  = specularShadingParams.z * cosAlphaWithPhong * irradiance.z; 

    return Vec3f(specularShadingRed,specularShadingGreen,specularShadingBlue);



}


// IMPLEMENT intersectionDetector FUNCTION

/*
Vec3f Scene::intersectionDetector(parser::Scene& scene, Ray& eyeRay, float& t_final, Vec3f& surfaceNormal, char& objInfo_0, int& objInfo_1, int& objInfo_2 ){


    float t_min = 100000.0; // We assume that all the t values will be less that this number

    float t;

    Vec3f intersectionSurfaceNormal;

    //char objectInfo[] = {' ', ' ', ' '}; 
    objInfo_0 = ' ';
    objInfo_1 = 0;
    objInfo_2 = 0;


    for (int i = 0; i < scene.spheres.size(); ++i)
    {
        Vec3f center = scene.vertex_data[scene.spheres[i].center_vertex_id-1]; // center of the sphere 
        if (intersection(eyeRay, scene.spheres[i], center ,t , surfaceNormal) && t <= t_min){
            
            intersectionSurfaceNormal = surfaceNormal;
            t_min = t;
            objInfo_0 = 's';
            objInfo_1 = i;
        }
        
    }
    for (int i = 0; i < scene.triangles.size(); ++i)
    {
        if(intersection(eyeRay, scene.triangles[i].indices, scene ,t , surfaceNormal) && t <= t_min){

            intersectionSurfaceNormal = surfaceNormal;
            t_min = t;
            objInfo_0 = 't';
            objInfo_1 = i;
        }
    }
    for (int i = 0; i < scene.meshes.size(); ++i)
    {
        for (int j = 0; j < scene.meshes[i].faces.size(); ++j)
        {
            if (intersection(eyeRay, scene.meshes[i].faces[j], scene ,t , surfaceNormal) && t <= t_min)
            {

                intersectionSurfaceNormal = surfaceNormal;
                t_min = t;
                objInfo_0 = 'f';
                objInfo_1 = i;
                objInfo_2 = j;
            }
             
        }
    }


    t_final = t_min;
    surfaceNormal = intersectionSurfaceNormal;


}


*/






// Parses XML file. 
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLError eResult;
	XMLElement *pElement;

	maxRecursionDepth = 1;
	shadowRayEps = 0.001;

	eResult = xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	pElement = pRoot->FirstChildElement("MaxRecursionDepth");
	if(pElement != nullptr)
		pElement->QueryIntText(&maxRecursionDepth);

	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%f %f %f", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	pElement = pRoot->FirstChildElement("ShadowRayEpsilon");
	if(pElement != nullptr)
		pElement->QueryFloatText(&shadowRayEps);

	pElement = pRoot->FirstChildElement("IntersectionTestEpsilon");
	if(pElement != nullptr)
		eResult = pElement->QueryFloatText(&intTestEps);

	// Parse cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while(pCamera != nullptr)
	{
        int id;
        char imageName[64];
        Vector3f pos, gaze, up;
        ImagePlane imgPlane;

		eResult = pCamera->QueryIntAttribute("id", &id);
		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &pos.x, &pos.y, &pos.z);
		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &gaze.x, &gaze.y, &gaze.z);
		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &up.x, &up.y, &up.z);
		camElement = pCamera->FirstChildElement("NearPlane");
		str = camElement->GetText();
		sscanf(str, "%f %f %f %f", &imgPlane.left, &imgPlane.right, &imgPlane.bottom, &imgPlane.top);
		camElement = pCamera->FirstChildElement("NearDistance");
		eResult = camElement->QueryFloatText(&imgPlane.distance);
		camElement = pCamera->FirstChildElement("ImageResolution");	
		str = camElement->GetText();
		sscanf(str, "%d %d", &imgPlane.nx, &imgPlane.ny);
		camElement = pCamera->FirstChildElement("ImageName");
		str = camElement->GetText();
		strcpy(imageName, str);

		cameras.push_back(new Camera(id, imageName, pos, gaze, up, imgPlane));

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// Parse materals
	pElement = pRoot->FirstChildElement("Materials");
	XMLElement *pMaterial = pElement->FirstChildElement("Material");
	XMLElement *materialElement;
	while(pMaterial != nullptr)
	{
		materials.push_back(new Material());

		int curr = materials.size() - 1;
	
		eResult = pMaterial->QueryIntAttribute("id", &materials[curr]->id);
		materialElement = pMaterial->FirstChildElement("AmbientReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->ambientRef.r, &materials[curr]->ambientRef.g, &materials[curr]->ambientRef.b);
		materialElement = pMaterial->FirstChildElement("DiffuseReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->diffuseRef.r, &materials[curr]->diffuseRef.g, &materials[curr]->diffuseRef.b);
		materialElement = pMaterial->FirstChildElement("SpecularReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->specularRef.r, &materials[curr]->specularRef.g, &materials[curr]->specularRef.b);
		materialElement = pMaterial->FirstChildElement("MirrorReflectance");
		if(materialElement != nullptr)
		{
			str = materialElement->GetText();
			sscanf(str, "%f %f %f", &materials[curr]->mirrorRef.r, &materials[curr]->mirrorRef.g, &materials[curr]->mirrorRef.b);
		}
				else
		{
			materials[curr]->mirrorRef.r = 0.0;
			materials[curr]->mirrorRef.g = 0.0;
			materials[curr]->mirrorRef.b = 0.0;
		}
		materialElement = pMaterial->FirstChildElement("PhongExponent");
		if(materialElement != nullptr)
			materialElement->QueryIntText(&materials[curr]->phongExp);

		pMaterial = pMaterial->NextSiblingElement("Material");
	}

	// Parse vertex data
	pElement = pRoot->FirstChildElement("VertexData");
	int cursor = 0;
	Vector3f tmpPoint;
	str = pElement->GetText();
	while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
		cursor++;
	while(str[cursor] != '\0')
	{
		for(int cnt = 0 ; cnt < 3 ; cnt++)
		{
			if(cnt == 0)
				tmpPoint.x = atof(str + cursor);
			else if(cnt == 1)
				tmpPoint.y = atof(str + cursor);
			else
				tmpPoint.z = atof(str + cursor);
			while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
				cursor++; 
			while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
				cursor++;
		}
		vertices.push_back(tmpPoint);
	}

	// Parse objects
	pElement = pRoot->FirstChildElement("Objects");
	
	// Parse spheres
	XMLElement *pObject = pElement->FirstChildElement("Sphere");
	XMLElement *objElement;
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int cIndex;
		float R;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Center");
		eResult = objElement->QueryIntText(&cIndex);
		objElement = pObject->FirstChildElement("Radius");
		eResult = objElement->QueryFloatText(&R);

		objects.push_back(new Sphere(id, matIndex, cIndex, R, &vertices));

		pObject = pObject->NextSiblingElement("Sphere");
	}

	// Parse triangles
	pObject = pElement->FirstChildElement("Triangle");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Indices");
		str = objElement->GetText();
		sscanf(str, "%d %d %d", &p1Index, &p2Index, &p3Index);

		objects.push_back(new Triangle(id, matIndex, p1Index, p2Index, p3Index, &vertices));

		pObject = pObject->NextSiblingElement("Triangle");
	}

	// Parse meshes
	pObject = pElement->FirstChildElement("Mesh");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;
		int cursor = 0;
		int vertexOffset = 0;
		vector<Triangle> faces;
		vector<int> *meshIndices = new vector<int>;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Faces");
		objElement->QueryIntAttribute("vertexOffset", &vertexOffset);
		str = objElement->GetText();
		while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
			cursor++;
		while(str[cursor] != '\0')
		{
			for(int cnt = 0 ; cnt < 3 ; cnt++)
			{
				if(cnt == 0)
					p1Index = atoi(str + cursor) + vertexOffset;
				else if(cnt == 1)
					p2Index = atoi(str + cursor) + vertexOffset;
				else
					p3Index = atoi(str + cursor) + vertexOffset;
				while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
					cursor++; 
				while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
					cursor++;
			}
			faces.push_back(*(new Triangle(-1, matIndex, p1Index, p2Index, p3Index, &vertices)));
			meshIndices->push_back(p1Index);
			meshIndices->push_back(p2Index);
			meshIndices->push_back(p3Index);
		}

		objects.push_back(new Mesh(id, matIndex, faces, meshIndices, &vertices));

		pObject = pObject->NextSiblingElement("Mesh");
	}

	// Parse lights
	int id;
	Vector3f position;
	Vector3f intensity;
	pElement = pRoot->FirstChildElement("Lights");

	XMLElement *pLight = pElement->FirstChildElement("AmbientLight");
	XMLElement *lightElement;
	str = pLight->GetText();
	sscanf(str, "%f %f %f", &ambientLight.r, &ambientLight.g, &ambientLight.b);

	pLight = pElement->FirstChildElement("PointLight");
	while(pLight != nullptr)
	{
		eResult = pLight->QueryIntAttribute("id", &id);
		lightElement = pLight->FirstChildElement("Position");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &position.x, &position.y, &position.z);
		lightElement = pLight->FirstChildElement("Intensity");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &intensity.r, &intensity.g, &intensity.b);

		lights.push_back(new PointLight(position, intensity));

		pLight = pLight->NextSiblingElement("PointLight");
	}
}

