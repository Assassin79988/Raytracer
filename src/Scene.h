#ifndef SCENE_H_
#define SCENE_H_

#include "DataTypes.h"
#include "Object.h"
#include "Camera.h"
#include "Image.h"
#include "ImagePlane.h"
#include "Ray.h"
#include "PointLight.h"
#include "BVH.h"

#include <iostream>
#include <vector>

namespace raytracer {
	class Scene;
	struct Objects;
}

class raytracer::Scene {
private:
	BVH* bvh;
	Image image_ = Image(500, 500);
	std::vector<Object*> objects_;
	std::vector<Object*> noBoundingBoxobjects_;
	std::vector<Object*> boundingBoxobjects_;
	std::vector<LightSource*> lights_;
	Camera camera_ = Camera(Vec3(0.0, 0.0, 0.0));
	ImagePlane imagePlane_ = ImagePlane(image_.getCols(), image_.getRows(), Vec3(-1, -1, -1), Vec3(1, 1, -1));

	uchar clamp(int color) const;
	bool hasShadow(Ray ray, float lightDirLength) const;
	Colour reflections(Ray ray, int depth) const;
	Colour phong(Ray ray, Object* closestObj, Vec3 normal, int depth, float t) const;
	Colour blinnPhong(Ray ray, Object* closestObj, Vec3 normal, float t) const;
	Object* findClosestObject(Ray ray, std::vector<Object*> objects) const;
public:
	Scene() {}
	Scene(std::vector<Object*> objects, std::vector<LightSource*> lights);

	void renderScene();
};

raytracer::Scene::Scene(std::vector<Object*> objects, std::vector<LightSource*> lights) : objects_(objects), lights_(lights) {
	// Checks for any objects that don't have a bounding box i.e. infinte planes
	for (int i = 0; i < objects_.size(); ++i) {
		if (!objects_[i]->hasBoundingBox()) {
			noBoundingBoxobjects_.push_back(objects_[i]);
		}
		else {
			boundingBoxobjects_.push_back(objects_[i]);
		}
	}
	
	bvh = new BVH(boundingBoxobjects_);
}

uchar raytracer::Scene::clamp(int color) const {
	if (color < 0) return 0;
	if (color >= 255) return 255;
	return color;
}


bool raytracer::Scene::hasShadow(Ray ray, float lightDirLength) const {
	bool hasShadow = false;
	float dist = 0.0;
	std::vector<Object*> objects = bvh->getIntersectedObject(ray);
	for (int i = 0; i < objects.size(); ++i) {
		bool hasIntersect = objects[i]->hasIntersect(ray, dist);
		if (dist > 0.0001 && dist < lightDirLength && hasIntersect) {
			hasShadow = true;
			break;
		}
	}

	for (int i = 0; i < noBoundingBoxobjects_.size(); ++i) {
		bool hasIntersect = noBoundingBoxobjects_[i]->hasIntersect(ray, dist);
		if (dist > 0.0001 && dist < lightDirLength && hasIntersect) {
			hasShadow = true;
			break;
		}
	}

	return hasShadow;
}

Colour raytracer::Scene::reflections(Ray ray, int depth) const{
	float t;
	Colour colour = black();
	std::vector<Object*> objects = bvh->getIntersectedObject(ray);
	Object* closestObj = findClosestObject(ray, objects);
	if (closestObj != nullptr) {
		if (closestObj->hasIntersect(ray, t)) {
			Vec3 normal = closestObj->getNormal(ray.compute(t));
			colour = phong(ray, closestObj, normal, depth, t);
		}
	}
	return colour;
}

Colour raytracer::Scene::phong(Ray ray, Object* closestObj, Vec3 normal, int depth, float t) const {
	Material* m = closestObj->getMaterial();
	Vec3 intersection = ray.compute(t);
	Vec3 diffuseAndSpecColour = Vec3(0.0, 0.0, 0.0);
	Colour reflectionColour = Colour(0.0, 0.0, 0.0);

	if (depth > 1) {
		if (m->getReflection() > 0.0f) {
			Vec3 viewer = ray.getDirection();//ray.getOrigin() - intersection;
			normal.normalize();
			viewer.normalize();
			Ray reflectedRay;
			if (m->getReflectedRay(ray.getDirection(), intersection, normal, reflectedRay)) {//-2 * (viewer.dot(normal)) * (normal) + viewer;
				reflectionColour += reflections(reflectedRay, depth - 1);
				reflectionColour *= m->getReflection();
			}
		}

		if (m->getRefraction() > 0.0f) {
			
		}
	}

	for (int i = 0; i < lights_.size(); ++i) {
		Vec3 lightVector = lights_[i]->computeLightVector(intersection);
		Vec3 viewer = ray.getOrigin() - intersection;
		float lightDirLength = lightVector.norm();

		lightVector.normalize();
		normal.normalize();
		viewer.normalize();
		float diffuseTerm = 0.0f;
		float specularTerm = 0.0f;
		Ray lightRay = Ray(intersection, lightVector);
		if (!hasShadow(lightRay, lightDirLength)) {
			diffuseTerm = fmax(lightVector.dot(normal), 0);

			Vec3 relfectedLight = 2 * lightVector.dot(normal) * normal - lightVector;
			relfectedLight.normalize();
			specularTerm = pow(fmax(relfectedLight.dot(viewer), 0), m->getSpecularExp());
		}

		Colour diffuseColour = m->getDiffuseColor();
		Colour specularColor = m->getSpecularColor();
		float kd = m->getKd();
		float ks = m->getKs();

		diffuseAndSpecColour[0] += kd * diffuseColour[0] * diffuseTerm + ks * specularColor[0] * specularTerm;
		diffuseAndSpecColour[1] += kd * diffuseColour[1] * diffuseTerm + ks * specularColor[1] * specularTerm;
		diffuseAndSpecColour[2] += kd * diffuseColour[2] * diffuseTerm + ks * specularColor[2] * specularTerm;
	}

	Colour colour(0, 0, 0);
	float ka = m->getKa();
	Colour ambientColour = m->getAmbientColor();
	colour[0] = clamp(ka * ambientColour[0] + diffuseAndSpecColour[0] / (float)lights_.size() + reflectionColour[0]);
	colour[1] = clamp(ka * ambientColour[1] + diffuseAndSpecColour[1] / (float)lights_.size() + reflectionColour[1]);
	colour[2] = clamp(ka * ambientColour[2] + diffuseAndSpecColour[2] / (float)lights_.size() + reflectionColour[2]);
	return colour;
}

Colour raytracer::Scene::blinnPhong(Ray ray, Object* closestObj, Vec3 normal, float t) const {

	return black();
}

raytracer::Object* raytracer::Scene::findClosestObject(Ray ray, std::vector<Object*> objects) const {
	float dist = 0.0f;
	float t = std::numeric_limits<float>::max();
	Object* closestObj = nullptr;

	for (int i = 0; i < objects.size(); ++i) {
		bool hasIntersect = objects[i]->hasIntersect(ray, dist);
		if (dist > 0 && dist < MAX_DIST && dist < t && hasIntersect) {
			closestObj = objects[i];
			t = dist;
		}
	}

	for (int i = 0; i < noBoundingBoxobjects_.size(); ++i) {
		bool hasIntersect = noBoundingBoxobjects_[i]->hasIntersect(ray, dist);
		if (dist > 0 && dist < MAX_DIST && dist < t && hasIntersect) {
			closestObj = noBoundingBoxobjects_[i];
			t = dist;
		}
	}

	return closestObj;
}

void raytracer::Scene::renderScene() {

	int h_samples = 2;
	int v_samples = 2;
	for (int row = 0; row < image_.getRows(); ++row) {
		for (int col = 0; col < image_.getCols(); ++col) {
			float pixelColorX = 0.0;
			float pixelColorY = 0.0;
			float pixelColorZ = 0.0;
			for (int p = 0; p < h_samples; ++p) {
				for (int q = 0; q < v_samples; ++q) {
					//std::cout << row << "\t" << col << std::endl;
					//float shfit = rand() / (RAND_MAX + 1.0);
					Ray ray = camera_.generateRay(imagePlane_.generatePixelPos(col + (q + 0.5f) / (float)v_samples, row + (p + 0.5f) / (float)h_samples));
					float t = std::numeric_limits<float>::max();
					std::vector<Object*> objects = bvh->getIntersectedObject(ray);
					Object* closestObj = findClosestObject(ray, objects);

					if (closestObj != nullptr) {
						if (closestObj->hasIntersect(ray, t)) {
							Vec3 normal = closestObj->getNormal(ray.compute(t));
							Colour colour = phong(ray, closestObj, normal, DEPTH, t);
							pixelColorX += colour[0];
							pixelColorY += colour[1];
							pixelColorZ += colour[2];
						}
					}
					else {
						Colour colour = black();
						pixelColorX += colour[0];
						pixelColorY += colour[1];
						pixelColorZ += colour[2];
					}
				}
			}
			image_(row, col) = Colour(pixelColorX / (float)(v_samples * h_samples), pixelColorY / (float)(v_samples * h_samples), pixelColorZ / (float)(v_samples * h_samples));
		}
	}

	image_.save("./result.png");
	image_.display();
}

#endif