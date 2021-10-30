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
#include "Box.h"
#include "Quad.h"
#include "Cone.h"
#include "PerspectiveCamera.h"
#include <random>

using namespace raytracer;


void CornellBox() {
	/* Conrell Box Scene */

	std::vector<LightSource*> lights;
	std::vector<Object*> objects;

	// Defining Lights
	LightSource* lightSources_ = new PointLight(Vec3(-5.0, -9.0f, -10.0f));

	lights.push_back(lightSources_);

	// Defining Materials
	Material* back_wall_m = new Material();
	back_wall_m->setAmbientColor(white());
	back_wall_m->setDiffuseColor(Colour(128, 128, 128));
	back_wall_m->setKa(0.1);
	back_wall_m->setKd(0.7);
	back_wall_m->setKs(0.7);
	back_wall_m->setSpecularExp(15);


	Material* left_wall_m = new Material();
	left_wall_m->setAmbientColor(red());
	left_wall_m->setDiffuseColor(Colour(0, 0, 139));
	left_wall_m->setKa(0.1);
	left_wall_m->setKd(0.7);
	left_wall_m->setKs(0.7);
	left_wall_m->setSpecularExp(15);

	Material* right_wall_m = new Material();
	right_wall_m->setAmbientColor(green());
	right_wall_m->setDiffuseColor(Colour(0, 100, 0));
	right_wall_m->setKa(0.1);
	right_wall_m->setKd(0.7);
	right_wall_m->setKs(0.7);
	right_wall_m->setSpecularExp(15);


	Material* floor_m = new Material();
	floor_m->setAmbientColor(white());
	floor_m->setDiffuseColor(Colour(128, 128, 128));
	floor_m->setKa(0.1);
	floor_m->setKd(0.7);
	floor_m->setKs(0.7);
	floor_m->setSpecularExp(15);


	Material* roof_m = new Material();
	roof_m->setAmbientColor(white());
	roof_m->setDiffuseColor(Colour(128, 128, 128));
	roof_m->setKa(0.1);
	roof_m->setKd(0.7);
	roof_m->setKs(0.7);
	roof_m->setSpecularExp(15);

	Material* sphere_m = new Material();
	sphere_m->setAmbientColor(green());
	sphere_m->setDiffuseColor(Colour(0, 100, 0));
	sphere_m->setKa(0.1);
	sphere_m->setKd(0.7);
	sphere_m->setKs(0.7);
	sphere_m->setRefraction(1.0);
	sphere_m->setSpecularExp(15);

	Material* sphere2_m = new Material();
	sphere2_m->setAmbientColor(red());
	sphere2_m->setDiffuseColor(Colour(0, 100, 0));
	sphere2_m->setKa(0.1);
	sphere2_m->setKd(0.7);
	sphere2_m->setKs(0.7);
	sphere2_m->setReflection(1.0);
	sphere2_m->setSpecularExp(15);

	// Defining Objects
	Quad front_wall = Quad(Vec3(10, 10, 0), Vec3(10, -10, 0), Vec3(-10, -10, 0), Vec3(-10, 10, 0));
	front_wall.setMaterial(back_wall_m);

	Quad back_wall = Quad(Vec3(10, 10, -20), Vec3(10, -10, -20), Vec3(-10, -10, -20), Vec3(-10, 10, -20));
	back_wall.setMaterial(back_wall_m);

	Quad left_wall = Quad(Vec3(-10, 10, -20), Vec3(-10, -10, -20), Vec3(-10, -10, 0), Vec3(-10, 10, 0));
	left_wall.setMaterial(left_wall_m);

	Quad right_wall = Quad(Vec3(10, -10, -20), Vec3(10, 10, -20), Vec3(10, 10, 0), Vec3(10, -10, 0));
	right_wall.setMaterial(right_wall_m);

	Quad roof = Quad(Vec3(-10, -10, -20), Vec3(10, -10, -20), Vec3(10, -10, 0), Vec3(-10, -10, 0));
	roof.setMaterial(roof_m);

	Quad floor = Quad(Vec3(10, 10, -20), Vec3(-10, 10, -20), Vec3(-10, 10, 0), Vec3(10, 10, 0));
	floor.setMaterial(floor_m);

	Quad tallBoxFront = Quad(Vec3(-4, 10, -15), Vec3(1, 10, -16), Vec3(1, -3, -16), Vec3(-4, -3, -15));
	tallBoxFront.setMaterial(back_wall_m);
	Quad tallBoxLeftSide = Quad(Vec3(-7, -3, -19), Vec3(-7, 10, -19), Vec3(-4, 10, -15), Vec3(-4, -3, -15));
	tallBoxLeftSide.setMaterial(left_wall_m);
	Quad tallBoxRightSide = Quad(Vec3(-2, -3, -19), Vec3(-2, 10, -19), Vec3(1, -3, -16), Vec3(1, -3, -16));
	Quad tallBoxBack = Quad(Vec3(-2, -3, -19), Vec3(-2, 10, -19), Vec3(-7, 10, -19), Vec3(-7, -3, -19));

	Box smallBox = Box(Vec3(-1, 5, -15), Vec3(4, 10, -10));
	smallBox.setMaterial(floor_m);

	Sphere s1 = Sphere(Vec3(1.5, 2, -12.5), 3);
	s1.setMaterial(sphere_m);

	Sphere s2 = Sphere(Vec3(-3, -6, -16.5), 3);
	s2.setMaterial(sphere2_m);

	// Loading Scene + Defining image, imagePlane, Camera
	objects.push_back(&front_wall);
	objects.push_back(&back_wall);
	objects.push_back(&left_wall);
	objects.push_back(&right_wall);
	objects.push_back(&roof);
	objects.push_back(&floor);
	objects.push_back(&tallBoxFront);
	objects.push_back(&tallBoxLeftSide);
	objects.push_back(&tallBoxRightSide);
	objects.push_back(&tallBoxBack);
	objects.push_back(&smallBox);
	objects.push_back(&s1);
	objects.push_back(&s2);

	Camera* camera = new PerspectiveCamera(Vec3(0.0, 0.0, 0), Vec3(0, 0, -1), Vec3(0, -1, 0));
	Image* image = new Image(500, 500);
	ImagePlane* imagePlane = new ImagePlane(image->getCols(), image->getRows(), Vec3(-1, -1, -1), Vec3(1, 1, -1));
	Scene scene = Scene(camera, imagePlane, image, objects, lights);

	scene.renderScene();
}

void testScene2() {
	/* Test Scene 2*/
	std::vector<LightSource*> lights;
	std::vector<Object*> objects;

	LightSource* lightSources_ = new PointLight(Vec3(-5.0, -9.0f, -10.0f));

	lights.push_back(lightSources_);

	// Defining Materials
	Material* back_wall_m = new Material();
	back_wall_m->setAmbientColor(white());
	back_wall_m->setDiffuseColor(Colour(128, 128, 128));
	back_wall_m->setKa(0.1);
	back_wall_m->setKd(0.7);
	back_wall_m->setKs(0.7);
	back_wall_m->setSpecularExp(15);


	Material* left_wall_m = new Material();
	left_wall_m->setAmbientColor(red());
	left_wall_m->setDiffuseColor(Colour(0, 0, 139));
	left_wall_m->setKa(0.1);
	left_wall_m->setKd(0.7);
	left_wall_m->setKs(0.7);
	left_wall_m->setSpecularExp(15);

	Material* right_wall_m = new Material();
	right_wall_m->setAmbientColor(green());
	right_wall_m->setDiffuseColor(Colour(0, 100, 0));
	right_wall_m->setKa(0.1);
	right_wall_m->setKd(0.7);
	right_wall_m->setKs(0.7);
	right_wall_m->setSpecularExp(15);


	Material* floor_m = new Material();
	floor_m->setAmbientColor(white());
	floor_m->setDiffuseColor(Colour(128, 128, 128));
	floor_m->setKa(0.1);
	floor_m->setKd(0.7);
	floor_m->setKs(0.7);
	floor_m->setSpecularExp(15);


	Material* roof_m = new Material();
	roof_m->setAmbientColor(white());
	roof_m->setDiffuseColor(Colour(128, 128, 128));
	roof_m->setKa(0.1);
	roof_m->setKd(0.7);
	roof_m->setKs(0.7);
	roof_m->setSpecularExp(15);

	Material* sphere_m = new Material();
	sphere_m->setAmbientColor(green());
	sphere_m->setDiffuseColor(Colour(0, 100, 0));
	sphere_m->setKa(0.1);
	sphere_m->setKd(0.7);
	sphere_m->setKs(0.7);
	sphere_m->setRefraction(1.0);
	sphere_m->setSpecularExp(15);

	Material* sphere2_m = new Material();
	sphere2_m->setAmbientColor(red());
	sphere2_m->setDiffuseColor(Colour(0, 100, 0));
	sphere2_m->setKa(0.1);
	sphere2_m->setKd(0.7);
	sphere2_m->setKs(0.7);
	sphere2_m->setReflection(1.0);
	sphere2_m->setSpecularExp(15);

	// Defining Objects
	Quad front_wall = Quad(Vec3(10, 10, 0), Vec3(10, -10, 0), Vec3(-10, -10, 0), Vec3(-10, 10, 0));
	front_wall.setMaterial(back_wall_m);

	Quad back_wall = Quad(Vec3(10, 10, -20), Vec3(10, -10, -20), Vec3(-10, -10, -20), Vec3(-10, 10, -20));
	back_wall.setMaterial(back_wall_m);

	Quad left_wall = Quad(Vec3(-10, 10, -20), Vec3(-10, -10, -20), Vec3(-10, -10, 0), Vec3(-10, 10, 0));
	left_wall.setMaterial(left_wall_m);

	Quad right_wall = Quad(Vec3(10, -10, -20), Vec3(10, 10, -20), Vec3(10, 10, 0), Vec3(10, -10, 0));
	right_wall.setMaterial(right_wall_m);

	Quad roof = Quad(Vec3(-10, -10, -20), Vec3(10, -10, -20), Vec3(10, -10, 0), Vec3(-10, -10, 0));
	roof.setMaterial(roof_m);

	Quad floor = Quad(Vec3(10, 10, -20), Vec3(-10, 10, -20), Vec3(-10, 10, 0), Vec3(10, 10, 0));
	floor.setMaterial(floor_m);

	Mesh bunny = Mesh("bunny.obj", Vec3(0.0f, 10.0f, -15.0f), Vec3(0, 0, 0), Vec3(50.0f, 50.0f, 50.0f));
	
	Cone cone = Cone(Vec3(0,1,0), Vec3(7,5,-15) , 4.0, M_PI / 8.0f);
	Cone cone2 = Cone(Vec3(0, 1, 0), Vec3(-7, 5, -15), 5.0, M_PI / 8.0f);
	bunny.getObjects(objects);
	objects.push_back(&front_wall);
	objects.push_back(&back_wall);
	objects.push_back(&left_wall);
	objects.push_back(&right_wall);
	objects.push_back(&roof);
	objects.push_back(&floor);
	objects.push_back(&cone);
	objects.push_back(&cone2);

	Camera* camera = new PerspectiveCamera(Vec3(0.0, 0.0, 0), Vec3(0, 0, -1), Vec3(0, -1, 0));
	Image* image = new Image(500, 500);
	ImagePlane* imagePlane = new ImagePlane(image->getCols(), image->getRows(), Vec3(-1, -1, -1), Vec3(1, 1, -1));
	Scene scene = Scene(camera, imagePlane, image, objects, lights);

	scene.renderScene();
}

void testScene3() {
	/* Test Scene 2*/
	std::vector<LightSource*> lights;
	std::vector<Object*> objects;

	LightSource* lightSources_ = new PointLight(Vec3(-5.0, -9.0f, -10.0f));

	lights.push_back(lightSources_);

	// Defining Materials
	Material* plane_m = new Material();
	plane_m->setAmbientColor(black());
	plane_m->setDiffuseColor(white());
	plane_m->setKa(0.1);
	plane_m->setKd(0.7);
	plane_m->setKs(0.7);
	plane_m->setSpecularExp(15);


	Material* s1_m = new Material();
	s1_m->setAmbientColor(red());
	s1_m->setDiffuseColor(Colour(0, 0, 139));
	s1_m->setReflection(1.0f);
	s1_m->setKa(0.1);
	s1_m->setKd(0.7);
	s1_m->setKs(0.7);
	s1_m->setSpecularExp(15);

	Material* s2_m = new Material();
	s2_m->setAmbientColor(green());
	s2_m->setDiffuseColor(Colour(0, 100, 0));
	s2_m->setReflection(1.0f);
	s2_m->setFuzz(0.1f);
	s2_m->setKa(0.1);
	s2_m->setKd(0.7);
	s2_m->setKs(0.7);
	s2_m->setSpecularExp(15);


	Material* s3_m = new Material();
	s3_m->setAmbientColor(red());
	s3_m->setDiffuseColor(Colour(0, 255, 255));
	s3_m->setKa(0.1);
	s3_m->setKd(0.7);
	s3_m->setKs(0.7);
	s3_m->setSpecularExp(15);


	Material* s4_m = new Material();
	s4_m->setAmbientColor(blue());
	s4_m->setDiffuseColor(Colour(255, 255, 0));
	s4_m->setKa(0.1);
	s4_m->setKd(0.7);
	s4_m->setKs(0.7);
	s4_m->setSpecularExp(15);

	Material* bunny_m = new Material();
	bunny_m->setAmbientColor(red());
	bunny_m->setDiffuseColor(Colour(135,0, 0));
	bunny_m->setKa(0.1);
	bunny_m->setKd(0.7);
	bunny_m->setKs(0.7);
	bunny_m->setReflection(1.0);
	bunny_m->setSpecularExp(15);

	Mesh bunny = Mesh("bunny.obj", Vec3(0.0f, 10.0f, -25.0f), Vec3(0, M_PI / 4.0f, 0), Vec3(100.0f, 100.0f, 100.0f));
	bunny.setMaterial(bunny_m);
	bunny.getObjects(objects);

	Plane p = Plane(Vec3(0,6,0), Vec3(0, -1, 0));
	Sphere s1 = Sphere(Vec3(6.0f, 3.0f, -10.0f), 3.0);
	s1.setMaterial(s1_m);
	Sphere s2 = Sphere(Vec3(-6.0f, 3.0f, -10.0f), 3.0);
	s2.setMaterial(s2_m);
	Sphere s3 = Sphere(Vec3(0.0f, 3.0f, 10.0f), 3.0);
	s3.setMaterial(s3_m);
	Sphere s4 = Sphere(Vec3(-6.0f, 3.0f, 10.0f), 3.0);
	s4.setMaterial(s4_m);

	objects.push_back(&p);
	objects.push_back(&s1);
	objects.push_back(&s2);
	objects.push_back(&s3);
	objects.push_back(&s4);
	//objects.push_back(&);
	//objects.push_back(&);

	Camera* camera = new PerspectiveCamera(Vec3(0.0, 0.0, 0), Vec3(0, 0, -1), Vec3(0, -1, 0));
	Image* image = new Image(500, 500);
	ImagePlane* imagePlane = new ImagePlane(image->getCols(), image->getRows(), Vec3(-1, -1, -1), Vec3(1, 1, -1));
	Scene scene = Scene(camera, imagePlane, image, objects, lights);

	scene.renderScene();
}

void testScene4() {
	std::vector<LightSource*> lights;
	std::vector<Object*> objects;

	LightSource* lightSources_ = new PointLight(Vec3(-30.0, -19.0f, -20.0f));

	lights.push_back(lightSources_);

	// Defining Materials
	Material* sphere1_m = new Material();
	sphere1_m->setAmbientColor(Colour(0, 0, 255));
	sphere1_m->setDiffuseColor(Colour(0, 0, 250));
	sphere1_m->setKa(0.2);
	sphere1_m->setKd(0.8);
	sphere1_m->setKs(0.8);
	sphere1_m->setReflection(0.1);
	sphere1_m->setSpecularExp(12);

	Material* sphere2_m = new Material();
	sphere2_m->setAmbientColor(Colour(0, 255, 255));
	sphere2_m->setDiffuseColor(Colour(0, 150, 150));

	Material* sphere3_m = new Material();
	sphere3_m->setAmbientColor(green());
	sphere3_m->setDiffuseColor(Colour(0, 100, 0));
	sphere3_m->setKa(0.1);
	sphere3_m->setKd(0.7);
	sphere3_m->setKs(0.7);
	sphere3_m->setRefraction(0.7);
	sphere3_m->setSpecularExp(15);

	Material* sphere4_m = new Material();
	sphere4_m->setAmbientColor(Colour(255, 0, 0));
	sphere4_m->setDiffuseColor(Colour(150, 0, 0));
	sphere4_m->setKa(0.1);
	sphere4_m->setKd(0.2);
	sphere4_m->setKs(0.2);
	sphere4_m->setRefraction(0.5);
	sphere4_m->setSpecularExp(15);

	Plane plane1 = Plane(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f));
	Plane plane2 = Plane(Vec3(0.0f, -20.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f));

	Sphere sphere1 = Sphere(Vec3(0.0f, -5.0f, -10.0f), 6);
	sphere1.setMaterial(sphere2_m);

	Sphere sphere2 = Sphere(Vec3(-10.0f, -5.0f, 0.0f), 6);
	sphere2.setMaterial(sphere1_m);

	Sphere sphere3 = Sphere(Vec3(10.0f, -5.0f, 0.0f), 6);
	sphere3.setMaterial(sphere3_m);

	Sphere sphere4 = Sphere(Vec3(0.0f, -11.0f, 5.0f), 6);
	sphere4.setMaterial(sphere4_m);

	objects.push_back(&sphere1);
	objects.push_back(&sphere2);
	objects.push_back(&sphere3);
	objects.push_back(&sphere4);
	objects.push_back(&plane1);
	objects.push_back(&plane2);

	Camera* camera = new PerspectiveCamera(Vec3(5.0, -16.0, -20.0), Vec3(0, 0, -1), Vec3(0, -1, 0));
	Image* image = new Image(500, 500);
	ImagePlane* imagePlane = new ImagePlane(image->getCols(), image->getRows(), Vec3(-1, -1, -1), Vec3(1, 1, -1));
	Scene scene = Scene(camera, imagePlane, image, objects, lights);

	scene.renderScene();
}

int main()
{
	CornellBox();
	testScene2();
	testScene3();
	testScene4();

	return EXIT_SUCCESS;
}
