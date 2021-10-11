#include <cstdlib>
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "PointLight.h"
#include "Scene.h"

using namespace raytracer;

/*
* This File Is Just For Testing.
*/
int main()
{

	Triangle t1 = Triangle(Vec3(-1, -1, -1), Vec3(1, -1, -1), Vec3(0, 0.75, -1));
	Vec3 dir = Vec3(1.0,1.0,0.0);
	dir.normalize();
	Plane p1 = Plane(Vec3(1.0,1.0,0.0), dir);
	Sphere s1 = Sphere(Vec3(0, 0, -5), 2.0);
	Sphere s2 = Sphere(Vec3(-40, -40, -100), 4.0);
	Triangle* s = new Triangle[1];
	s[0] = t1;

	Scene scene = Scene(s, 1);
	scene.renderScene();


	return EXIT_SUCCESS;
}
