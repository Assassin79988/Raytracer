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
	struct Objects;
}

struct raytracer::Objects {
	Object* object = nullptr;
};

class raytracer::Scene {
private:
	Image image_ = Image(500, 500);
	Objects* objects_;
	int numObjects_;
	LightSource* lightSources_ = new PointLight(Vec3(2.0,-3.0,0));
	//int numLights_ = 1;
	Camera camera_ = Camera(Vec3(0.0, 0.0, 0.0));
	ImagePlane imagePlane_ = ImagePlane(image_.getCols(), image_.getRows(), Vec3(-1, -1, -1), Vec3(1, 1, -1));

public:
	Scene(Objects* objects, int numObjects) {
		objects_ = objects;
		numObjects_ = numObjects;
	}

	void renderScene();
};

void raytracer::Scene::renderScene() {

	for (int row = 0; row < image_.getRows(); ++row) {
		for (int col = 0; col < image_.getCols(); ++col) {
			Ray ray = camera_.generateRay(imagePlane_.generatePixelPos(col, row));
			float tSmall = INT_MAX;
			for (int obj = 0; obj < numObjects_; ++obj) {
				float t;
				if (objects_[obj].object->hasIntersect(ray, t)) {
					if (t > 0 && t < tSmall) {
						Vec3 lightVector = lightSources_->computeLightVector(ray.compute(t));
						Vec3 normal = objects_[obj].object->getNormal(ray.compute(t));
						Vec3 viewer = ray.getOrigin() - ray.compute(t);
						lightVector.normalize();
						normal.normalize();
						viewer.normalize();
						Colour colour = objects_[obj].object->getMaterial().blinnPhong(viewer, normal, lightVector);
						image_(row, col) = colour;
						tSmall = t;

					}
					else if (t > 0) {

					}
					else if (tSmall == INT_MAX){
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