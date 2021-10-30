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
#include "PerspectiveCamera.h"

#include <iostream>
#include <vector>

namespace raytracer {
	class Scene;
}

class raytracer::Scene {
private:
	// BVH for the scene
	BVH* bvh_;
	// Image the scene will be rendered on
	Image* image_;
	// A list of objects in the scene
	std::vector<Object*> objects_;
	// Objects included in the BVH
	std::vector<Object*> noBoundingBoxobjects_;
	// Objects not included in the BVH
	std::vector<Object*> boundingBoxobjects_;
	// Lights in the scene
	std::vector<LightSource*> lights_;
	// Camera in the scnene
	Camera* camera_;
	// The image plane the scene will be rendered on
	ImagePlane* imagePlane_; 

	// Clamps a colour between 0 and 255
	uchar clamp(int color) const;
	// Generate a BVH with the current light of objects
	void createBVH();
	// See if a shadow is cased or not
	bool hasShadow(Ray ray, float lightDirLength) const;
	// Casted the secondary rays
	Colour castSecondaryRays(Ray ray, int depth) const;
	// Computed the current pixel colour
	Colour shading(Ray ray, Object* closestObj, Vec3 normal, int depth, float t) const;
	// Finds the cloest intersected object given a list of objects
	Object* findClosestObject(Ray ray, std::vector<Object*> objects) const;
public:
	// Constructors
	Scene() {}
	Scene(Camera* camera, ImagePlane* imagePlane, Image* image, std::vector<Object*> objects, std::vector<LightSource*> lights);
	Scene(std::vector<Object*> objects, std::vector<LightSource*> lights);
	// Deconstructors
	~Scene();

	/* Getters/Setters */
	void setCamera(Camera* camera) { camera_ = camera; }
	void setImagePlane(ImagePlane* imagePlane) { imagePlane_ = imagePlane; }
	void setImage(Image* image) { image_ = image; }
	void setObjects(std::vector<Object*> objects);
	void setLights(std::vector<LightSource*>);
	Camera* getCamera() { return camera_; }
	ImagePlane* getImagePlane() { return imagePlane_; }
	Image* getImage() { return image_; }
	std::vector<Object*> getObjects() { return objects_; }
	std::vector<LightSource*> getLights() { return lights_; }

	// Renders the scene
	void renderScene();
};

raytracer::Scene::Scene(Camera* camera, ImagePlane* imagePlane, Image* image, std::vector<Object*> objects, std::vector<LightSource*> lights) {
	camera_ = camera;
	imagePlane_ = imagePlane;
	image_ = image;
	objects_ = objects;
	lights_ = lights;

	createBVH();
}

raytracer::Scene::Scene(std::vector<Object*> objects, std::vector<LightSource*> lights) : objects_(objects), lights_(lights) {
	createBVH();
}

raytracer::Scene::~Scene() {
	// Free's all pointers 
	if (bvh_ != nullptr) free(bvh_);
	if (camera_ != nullptr) free(camera_);
	if (imagePlane_ != nullptr) free(imagePlane_);
	if (image_ != nullptr) free(image_);

	for (int i = 0; i < objects_.size(); ++i) {
		if (objects_[i] != nullptr) 	free(objects_[i]);
	}

	for (int i = 0; i < noBoundingBoxobjects_.size(); ++i) {
		if (noBoundingBoxobjects_[i] != nullptr) free(noBoundingBoxobjects_[i]);
	}

	for (int i = 0; i < boundingBoxobjects_.size(); ++i) {
		if (boundingBoxobjects_[i] != nullptr) free(boundingBoxobjects_[i]);
	}

	for (int i = 0; i < lights_.size(); ++i) {
		if (lights_[i] != nullptr) free(lights_[i]);
	}
}

void raytracer::Scene::setObjects(std::vector<Object*> objects) {
	for (int i = 0; i < objects_.size(); ++i) {
		if (objects_[i] != nullptr) 	free(objects_[i]);
	}

	for (int i = 0; i < noBoundingBoxobjects_.size(); ++i) {
		if (noBoundingBoxobjects_[i] != nullptr) free(noBoundingBoxobjects_[i]);
	}

	for (int i = 0; i < boundingBoxobjects_.size(); ++i) {
		if (boundingBoxobjects_[i] != nullptr) free(boundingBoxobjects_[i]);
	}
	objects_ = objects;
	createBVH();
}

void raytracer::Scene::createBVH() {
	// Checks for any objects that don't have a bounding box i.e. infinte plane
	for (int i = 0; i < objects_.size(); ++i) {
		if (!objects_[i]->hasBoundingBox()) {
			noBoundingBoxobjects_.push_back(objects_[i]);
		}
		else {
			boundingBoxobjects_.push_back(objects_[i]);
		}
	}

	bvh_ = new BVH(boundingBoxobjects_);
}

// Clmaps a colour value between 0 and 255
uchar raytracer::Scene::clamp(int color) const {
	if (color < 0) return 0;
	if (color >= 255) return 255;
	return color;
}


bool raytracer::Scene::hasShadow(Ray ray, float lightDirLength) const {
	bool hasShadow = false;
	float dist = 0.0;
	std::vector<Object*> objects = bvh_->getIntersectedObject(ray);
	// Loops through each object from the BVH to see if a shadow is present
	for (int i = 0; i < objects.size(); ++i) {
		bool hasIntersect = objects[i]->hasIntersect(ray, dist);
		if (dist > EPSILON && dist < lightDirLength && hasIntersect) {
			hasShadow = true;
			break;
		}
	}

	// Loops through each object noit included in the BVH to see if a shadow is present
	for (int i = 0; i < noBoundingBoxobjects_.size(); ++i) {
		bool hasIntersect = noBoundingBoxobjects_[i]->hasIntersect(ray, dist);
		if (dist > EPSILON && dist < lightDirLength && hasIntersect) {
			hasShadow = true;
			break;
		}
	}

	return hasShadow;
}

Colour raytracer::Scene::castSecondaryRays(Ray ray, int depth) const{
	float t;
	// Set the inital colour to the background colour
	Colour colour = BACKGROUND_COLOUR;
	// Finds all objects that the ray could intersect
	std::vector<Object*> objects = bvh_->getIntersectedObject(ray);
	Object* closestObj = findClosestObject(ray, objects);
	// Finds the colour of the pixel from the casted ray
	if (closestObj != nullptr) {
		if (closestObj->hasIntersect(ray, t)) {
			Vec3 normal = closestObj->getNormal(ray.compute(t));
			colour = shading(ray, closestObj, normal, depth, t);
		}
	}
	return colour;
}

Colour raytracer::Scene::shading(Ray ray, Object* closestObj, Vec3 normal, int depth, float t) const {
	// Gets the closest objects material
	Material* m = closestObj->getMaterial(); 
	Vec3 intersection = ray.compute(t);
	Vec3 diffuseAndSpecColour = Vec3(0.0, 0.0, 0.0);
	Colour reflectionColour = Colour(0.0, 0.0, 0.0);
	Colour refractionColour = Colour(0.0, 0.0, 0.0);

	// For testing 
	bool hasRefraction = false;

	// Reflection and Refractions
	if (depth > 1) {
		if (m->getReflection() > 0.0f) {
			Ray reflectedRay;
			// Gets the reflected ray if one exist
			if (m->getReflectedRay(ray.getDirection(), intersection, normal, reflectedRay)) {
				// Cast the reflected ray and find it's correspounding colour
				reflectionColour += castSecondaryRays(reflectedRay, depth - 1);
				// Weights the reflection colour
				reflectionColour *= m->getReflection();
			}
		}
		//std::cout << m->getRefraction() << std::endl;
		if (m->getRefraction() > 0.0f) {
			Ray refractedRay;
			// Gets the refracted ray if one exist
			if (m->getRefractedRay(ray.getDirection(), intersection, normal, refractedRay)) {
				// Cast the refracted ray and find it's correspounding colour
				hasRefraction = true;
				refractionColour += castSecondaryRays(refractedRay, depth - 1);
				// Weights the refracted colour
				refractionColour *= m->getRefraction();
			}
		}
	}

	// Loop through each light source
	for (int i = 0; i < lights_.size(); ++i) {
		Vec3 lightVector = lights_[i]->computeLightVector(intersection);
		Vec3 viewer = ray.getOrigin() - intersection;
		float lightDirLength = lightVector.norm();

		lightVector.normalize();
		normal.normalize();
		viewer.normalize();
		float diffuseTerm = 0.0f;
		float specularTerm = 0.0f;

		// Computes the diffuse and specular terms if a shadow is not present
		Ray lightRay = Ray(intersection, lightVector);
		if (!hasShadow(lightRay, lightDirLength)) {
			diffuseTerm = fmax(lightVector.dot(normal), 0);
			if (PHONG) {
				Vec3 relfectedLight = 2 * lightVector.dot(normal) * normal - lightVector;
				relfectedLight.normalize();
				specularTerm = pow(fmax(relfectedLight.dot(viewer), 0), m->getSpecularExp());
			}
			else if (BILNN_PHONG) {
				// computed the bisector between the viewer and the lightVector
				Vec3 halfVector = viewer + lightVector;
				halfVector.normalize();
				specularTerm = pow(fmax(normal.dot(halfVector), 0), m->getSpecularExp());
			}

			// Issues with refrations and specular highlights currently
			specularTerm *= (!hasRefraction);
		}

		// FInd the total colour from the diffuse and specular terms
		Colour diffuseColour = m->getDiffuseColor();
		Colour specularColor = lights_[i]->getColour();
		float kd = m->getKd();
		float ks = m->getKs();

		diffuseAndSpecColour[0] += kd * diffuseColour[0] * diffuseTerm + ks * specularColor[0] * specularTerm;
		diffuseAndSpecColour[1] += kd * diffuseColour[1] * diffuseTerm + ks * specularColor[1] * specularTerm;
		diffuseAndSpecColour[2] += kd * diffuseColour[2] * diffuseTerm + ks * specularColor[2] * specularTerm;
	}

	// Gets the ambient colour and coefficent (ka)
	Colour colour(0, 0, 0);
	float ka = m->getKa();
	Colour ambientColour = m->getAmbientColor();
	//Colour textureColor = Colour(0, 0, 0);
	//if (closestObj->getTexture() != nullptr) {
	//	textureColor = closestObj->getTextureAt(ray.compute(t));
	//}
	// computes the total pixel colour (the diffuse and specular terms are averaged for each light source in the scene)
	colour[0] = clamp(ka * ambientColour[0] + diffuseAndSpecColour[0] / (float)lights_.size() + reflectionColour[0] + refractionColour[0]);
	colour[1] = clamp(ka * ambientColour[1] + diffuseAndSpecColour[1] / (float)lights_.size() + reflectionColour[1] + refractionColour[1]);
	colour[2] = clamp(ka * ambientColour[2] + diffuseAndSpecColour[2] / (float)lights_.size() + reflectionColour[2] + refractionColour[2]);
	return colour;
}

raytracer::Object* raytracer::Scene::findClosestObject(Ray ray, std::vector<Object*> objects) const {
	// initlized the distacne to zero and the current intersection to max float
	float dist = 0.0f;
	float t = std::numeric_limits<float>::max();
	Object* closestObj = nullptr;

	// Finds the closest object of intersection
	for (int i = 0; i < objects.size(); ++i) {
		bool hasIntersect = objects[i]->hasIntersect(ray, dist);
		if (dist > 0 && dist < MAX_DIST && dist < t && hasIntersect) {
			closestObj = objects[i];
			t = dist;
		}
	}

	// Looks to see if a object without a bounding box is closer (Not included in the BVH)
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

	// Number of samples per pixel
	int h_samples = H_SAMPLES;
	int v_samples = V_SAMPLES;

	// Loop through each pixel
	for (int row = 0; row < image_->getRows(); ++row) {
		for (int col = 0; col < image_->getCols(); ++col) {
			float pixelColorX = 0.0;
			float pixelColorY = 0.0;
			float pixelColorZ = 0.0;
			// loops trhough each sub pixel
			for (int p = 0; p < h_samples; ++p) {
				for (int q = 0; q < v_samples; ++q) {
					Ray ray;

					// Antialiasing 
					if (UNIFORM) {
						// Unifrom supersampling
						ray = camera_->generateRay(imagePlane_->generatePixelPos(col + (q + 0.5f) / (float)v_samples, row + (p + 0.5f) / (float)h_samples));
					}
					else if (RANDOM) {
						// Random supersampling
						float shfit = rand() / (RAND_MAX + 1.0);
						ray = camera_->generateRay(imagePlane_->generatePixelPos(col + shfit, row + shfit));
					}
					else if (JITTERING) {
						// Jittered supersampling
						float shfit = rand() / (RAND_MAX + 1.0);
						ray = camera_->generateRay(imagePlane_->generatePixelPos(col + (q + shfit) / (float)v_samples, row + (p + shfit) / (float)h_samples));
					}
					else {
						// If no supersampling method is selected then antialising is disabled
						h_samples = 1;
						v_samples = 1;
						ray = camera_->generateRay(imagePlane_->generatePixelPos(col + 0.5f, row + 0.5f));
					}

					// initlizes the intersection distance to max float
					float t = std::numeric_limits<float>::max();
					// Finds the object the ray coukld possibly intersect with 
					std::vector<Object*> objects = bvh_->getIntersectedObject(ray);
					// Finds closest intersected object 
					Object* closestObj = findClosestObject(ray, objects);

					if (closestObj != nullptr) {
						if (closestObj->hasIntersect(ray, t)) {
							Vec3 normal = closestObj->getNormal(ray.compute(t));
							// computes the shading for the current sub-pixel
							Colour colour = shading(ray, closestObj, normal, DEPTH, t);
							pixelColorX += colour[0];
							pixelColorY += colour[1];
							pixelColorZ += colour[2];
						}
					}
					else {
						// computes the shading for the current sub-pixel if there is no object intersected 
						Colour colour = BACKGROUND_COLOUR;
						pixelColorX += colour[0];
						pixelColorY += colour[1];
						pixelColorZ += colour[2];
					}
				}
			}
			// Finds the total colour for the current pixel by averaging the sub pixel values
			(*image_)(row, col) = Colour(pixelColorX / (float)(v_samples * h_samples), pixelColorY / (float)(v_samples * h_samples), pixelColorZ / (float)(v_samples * h_samples));
		}
	}

	// Saves and displays the raytraced image
	image_->save("./result.png");
	image_->display();
}

#endif