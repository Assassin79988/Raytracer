#include <cstdlib>
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
	//Triangle t1 = Triangle(Vec3(-1, -1, -1), Vec3(1, -1, -10), Vec3(0, 0.75, -20));
	Vec3 dir = Vec3(0.0,-1.0,0.0);
	dir.normalize();
	Plane p1 = Plane(Vec3(1.0,6.0,0.0), dir);
	Material m1 = Material();
	m1.setAmbientColor(Colour(192, 192, 192));
	m1.setDiffuseColor(Colour(105, 105, 105));
	p1.setMaterial(m1);
	Sphere s1 = Sphere(Vec3(-5, -1, -10), 2.0);
	//Sphere s2 = Sphere(Vec3(-80, -40, -100), 4.0);
	Mesh test = Mesh("bunny.obj", Vec3(5.0f,5.0f,-30.0f), 100.0f);
	Mesh test2 = Mesh("teapot.obj", Vec3(5.0f, 1.0f, -20.0f), 0.1f);
	std::cout << "Here!" << std::endl;
	Mesh test3 = Mesh("cube.obj", Vec3(2.0f, 2.0f, -5.0f), 1.0f);
	std::vector<Object*> objects;
	objects.push_back(&p1);
	objects.push_back(&s1);
	test.getObjects(objects);
	test2.getObjects(objects);
	test3.getObjects(objects);
	Scene scene = Scene(objects);
	scene.renderScene();


	return EXIT_SUCCESS;
}
