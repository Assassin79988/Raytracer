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

	//Triangle t1 = Triangle(Vec3(2, 2, -2), Vec3(1, -1, -1), Vec3(1, 2, -6));
	//Triangle t1 = Triangle(Vec3(-1, -1, -1), Vec3(1, -1, -10), Vec3(0, 0.75, -20));
	Vec3 dir = Vec3(0.0,-1.0,0.0);
	dir.normalize();
	Plane p1 = Plane(Vec3(1.0,1.0,0.0), dir);
	Sphere s1 = Sphere(Vec3(0, -1.01, -5), 2.0);
	Sphere s2 = Sphere(Vec3(-80, -40, -100), 4.0);
	Objects* s = new Objects[2];
	s[0].object = &s1;
	s[1].object = &p1;

	Scene scene = Scene(s, 2);
	scene.renderScene();


	return EXIT_SUCCESS;
}
