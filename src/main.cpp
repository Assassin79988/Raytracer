#include <cstdlib>
#include <vector>
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "PointLight.h"
#include "Scene.h"
#include "Mesh.h"
#include "Material.h"

using namespace raytracer;

/*
* This File Is Just For Testing.
*/
int main()
{

	//Triangle t1 = Triangle(Vec3(2, 2, -2), Vec3(1, -1, -1), Vec3(1, 2, -6));
	//Triangle t1 = Triangle(Vec3(-1, -1, -1), Vec3(1, -1, -10), Vec3(0, 0.75, -1));
	Vec3 dir = Vec3(0.0,-1.0,0.0);
	dir.normalize();
	Plane p1 = Plane(Vec3(0.0,6.0,0.0), dir);
	Material m1 = Material();
	m1.setAmbientColor(black());
	m1.setDiffuseColor(white());
	p1.setMaterial(m1);
	Sphere s1 = Sphere(Vec3(0, 0, -10), 3.0);
	//Sphere s2 = Sphere(Vec3(-80, -40, -100), 4.0);
	Mesh test = Mesh("Carved pumpkin.obj", Vec3(0.0f,0.0f,-15.0f), 0.05f);
	Mesh test2 = Mesh("bunny.obj", Vec3(0.0f, 10.0f, -20.0f), 100.0f);
	//std::cout << "Here!" << std::endl;
	Mesh test3 = Mesh("cube.obj", Vec3(-4.0f, 4.0f, -5.0f), 1.0f);
	std::vector<Object*> objects;
	std::vector<LightSource*> lights;
	objects.push_back(&p1);
	objects.push_back(&s1);
	//test.getObjects(objects);
	//test2.getObjects(objects);
	//test3.getObjects(objects);
	LightSource* lightSources_ = new PointLight(Vec3(0.0, -20.0, 0));
	lights.push_back(lightSources_);
	LightSource* lightSource2 = new PointLight(Vec3(10.0, 0.0, 0));
	lights.push_back(lightSource2);
	LightSource* lightSource3 = new PointLight(Vec3(-5.0, 1.0, 3));
	lights.push_back(lightSource3);
	Scene scene = Scene(objects, lights);
	scene.renderScene();



	return EXIT_SUCCESS;
}
