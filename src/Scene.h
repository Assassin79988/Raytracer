#ifndef SCENE_H_
#define SCENE_H_

#include "Object.h"
#include "Camera.h"
#include "Image.h"
#include "ImagePlane.h"
#include "Ray.h"
#include "PointLight.h"

#include <iostream>

namespace raytracer {
	class Scene;
}

class raytracer::Scene {
private:
	Image image_ = Image(500, 500);
	Triangle* objects_;
	int numObjects_;
	LightSource* lightSources_ = new PointLight(Vec3(2.0,3.0,0));
	//int numLights_ = 1;
	Camera camera_ = Camera(Vec3(0.0, 0.0, 0.0));
	ImagePlane imagePlane_ = ImagePlane(image_.getCols(), image_.getRows(), Vec3(-1, -1, -1), Vec3(1, 1, -1));

public:
	Scene(Triangle* objects, int numObjects) {
		objects_ = objects;
		numObjects_ = numObjects;
	}

	uchar clamp(int color);
	void renderScene();
};


uchar raytracer::Scene::clamp(int color) {
	if (color < 0) return 0;
	if (color >= 255) return 255;
	return color;
}

void raytracer::Scene::renderScene() {
	Colour ambient(0, 0, 0);
	Colour diffuse(0, 255, 100);

	for (int row = 0; row < image_.getRows(); ++row) {
		for (int col = 0; col < image_.getCols(); ++col) {
			Ray ray = camera_.generateRay(imagePlane_.generatePixelPos(col, row));
			float tSmall = INT_MAX;
			for (int obj = 0; obj < numObjects_; ++obj) {
				float t;
				if (objects_[obj].hasIntersect(ray, t)) {
					//std::cout << t << std::endl;
					if (t > 0 && t < tSmall) {
						Vec3 lightVector = lightSources_->computeLightVector(ray.compute(t));
						Vec3 Normal = objects_[obj].getNormal(ray.compute(t));
						float diffuseTerm = lightVector.dot(Normal);
						if (diffuseTerm < 0) diffuseTerm = 0;
						Colour colour(0, 0, 0); //The ambient base
						colour[0] = clamp(ambient[0] + diffuse[0] * diffuseTerm);
						colour[1] = clamp(ambient[1] + diffuse[1] * diffuseTerm);
						colour[2] = clamp(ambient[2] + diffuse[2] * diffuseTerm);
						image_(row, col) = colour;
						tSmall = t;

					}
					else if (t > 0) {

					}
					else {
						image_(row, col) = white();
					}
				}
			}
		}
	}

	image_.save("./result.png");
	image_.display();
}
#endif