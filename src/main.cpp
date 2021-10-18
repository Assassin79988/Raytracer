#include <cstdlib>
#include <vector>
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "PointLight.h"
#include "Scene.h"
#include "Mesh.h"
#include "Material.h"
#include "BVH.h"
#include <random>

using namespace raytracer;

/*
* This File Is Just For Testing.
*/
int main()
{

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> randAxis(0, 2);
	int axis = randAxis(rng);

	//std::cout << axis <<std::endl;
	//Triangle t1 = Triangle(Vec3(2, 2, -2), Vec3(1, -1, -1), Vec3(1, 2, -6));
	//Triangle t1 = Triangle(Vec3(-1, -1, -1), Vec3(1, -1, -10), Vec3(0, 0.75, -1));
	//Vec3 dir = Vec3(0.0,-1.0,0.0);
	//dir.normalize();
	//Plane p1 = Plane(Vec3(0.0,6.0,0.0), dir);
	Material m1 = Material();
	m1.setAmbientColor(black());
	m1.setDiffuseColor(white());
	//p1.setMaterial(m1);
	Sphere s1 = Sphere(Vec3(0, 0, -20), 6.0, 1);
	s1.setMaterial(m1);
	Sphere s2 = Sphere(Vec3(0, 0, -10), 1.0, 2);
	std::vector<Object*> objects;
	//objects.push_back(&s1);
	//objects.push_back(&s2);

	//BVH* yo = new BVH(objects);
	//Vec3 dir = Vec3(0, 0, 1);
	//dir.normalize();
	//Ray ray = Ray(Vec3(0, 0, 0), dir);
	//std::cout << s2.createBoundingBox()->hasIntersect(ray,t) << std::endl;
	//Object* w = yo->getIntersectedObject(ray);
	//std::cout << w << std::endl;


	//Mesh test = Mesh("Carved pumpkin.obj", Vec3(0.0f,0.0f,-15.0f), 0.05f);
	Mesh test2 = Mesh("bunny.obj", Vec3(0.0f, 10.0f, -20.0f), 100.0f);
	//std::cout << "Here!" << std::endl;
	//Mesh test3 = Mesh("cube.obj", Vec3(-4.0f, 4.0f, -5.0f), 1.0f);
	//std::vector<Object*> objects;
	std::vector<LightSource*> lights;
	//objects.push_back(&p1);
	//objects.push_back(&s1);
	//test.getObjects(objects);
	test2.getObjects(objects);
	//test3.getObjects(objects);
	LightSource* lightSources_ = new PointLight(Vec3(0.0, -20.0, 0));
	lights.push_back(lightSources_);
	//LightSource* lightSource2 = new PointLight(Vec3(10.0, 0.0, 0));
	//lights.push_back(lightSource2);
	//LightSource* lightSource3 = new PointLight(Vec3(-5.0, 1.0, 3));
	//lights.push_back(lightSource3);
	Scene scene = Scene(objects, lights);
	scene.renderScene();



	return EXIT_SUCCESS;
}
