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
#include "box.h"
#include "Metal.h"
#include "Dieletric.h"
#include <random>

using namespace raytracer;
# define M_PI 3.14159265358979323846

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
	Vec3 dir = Vec3(0.0,-1.0,0.0);
	dir.normalize();
	Plane p1 = Plane(Vec3(0.0,6.0,0.0), dir);
	Material* m1 = new Dieletric();
	m1->setAmbientColor(black());
	m1->setDiffuseColor(white());
	m1->setReflection(0.0f);
	p1.setMaterial(m1);
	//Sphere s1 = Sphere(Vec3(4, 30, -30), 6.0);
	//s1.setMaterial(m1);
	Sphere s5 = Sphere(Vec3(6.0f, 3.0f, -10.0f), 3.0);
	Material* m2 = new Dieletric();
	m2->setDiffuseColor(blue());
	m2->setReflection(0.0f);
	s5.setMaterial(m2);
	Sphere s2 = Sphere(Vec3(0.0f, 3.0f, -10.0f), 3.0);
	Material* m3 = new Dieletric();
	m3->setReflection(0.0f);
	m3->setAmbientColor(black());
	m3->setDiffuseColor(blue());
	Sphere s1 = Sphere(Vec3(0.0f, 3.0f, 10.0f), 3.0);
	s1.setMaterial(m3);
	Material* m4 = new Dieletric();
	m4->setReflection(0.0f);
	m4->setAmbientColor(black());
	m4->setDiffuseColor(green());
	Sphere s3 = Sphere(Vec3(10.0f, 3.0f, 20.0f), 3.0);
	s3.setMaterial(m4);
	Material* m5 = new Dieletric();
	m5->setReflection(0.0f);
	m5->setAmbientColor(black());
	m5->setDiffuseColor(Colour(0, 255, 255));
	Sphere s4 = Sphere(Vec3(-10.0f, 3.0f, 15.0f), 3.0);
	s4.setMaterial(m5);
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
	//Mesh test2 = Mesh("bunny.obj", Vec3(0.0f, 10.0f,-20.0f), Vec3(0, 0, 0), Vec3(100.0f, 100.0f, 100.0f));
	//Mesh test2 = Mesh("alfa147.obj", Vec3(0.0f, 0.0f, -20.0f), Vec3(-M_PI / 4.0, 0, 0), Vec3(0.1f, 0.1f, 0.1f));
	//std::cout << "Here!" << std::endl;
	//Mesh test3 = Mesh("cube.obj", Vec3(-4.0f, 5.0f, -8.0f), Vec3(0, 0, 0), Vec3(1.0f, 1.0f, 1.0f));
	//std::vector<Object*> objects;
	std::vector<LightSource*> lights;
	objects.push_back(&p1);
	objects.push_back(&s1);
	objects.push_back(&s2);
	objects.push_back(&s3);
	objects.push_back(&s4);
	objects.push_back(&s5);
	//objects.push_back(&s2);
	//test.getObjects(objects);
	//test2.getObjects(objects);
	//std::cout << objects.size() << std::endl;
	//test3.getObjects(objects);
	//Box b1 = Box(Vec3(1, 2, -5), Vec3(-1, -10,-15));
	//objects.push_back(&b1);
	//objects.push_back(&b2);
	LightSource* lightSources_ = new PointLight(Vec3(0.0, -20.0, 0));
	lights.push_back(lightSources_);
	//LightSource* lightSource2 = new PointLight(Vec3(0.0, 0.0, 0));
	//lights.push_back(lightSource2);
	//LightSource* lightSource3 = new PointLight(Vec3(5.0, 3.0, 3));
	//lights.push_back(lightSource3);
	Scene scene = Scene(objects, lights);
	scene.renderScene();



	return EXIT_SUCCESS;
}
