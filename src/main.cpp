#include <cstdlib>
#include "Sphere.h"
#include "PointLight.h"
#include "Scene.h"

using namespace raytracer;

/*
* This File Is Just For Testing.
*/
int main()
{
	Sphere s1 = Sphere(Vec3(0, 0, -5), 2.0);
	Sphere s2 = Sphere(Vec3(-40, -40, -100), 4.0);
	Sphere* s = new Sphere[2]; 
	s[0] = s1;
	s[1] = s2;

	Scene scene = Scene(s, 2);
	scene.renderScene();


	return EXIT_SUCCESS;
}
