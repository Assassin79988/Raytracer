#ifndef BVH_H_
#define BVH_H_

#include "Object.h"
#include "BoundingBox.h"
#include "Ray.h"
#include <vector>
#include <algorithm>
#include <random>

namespace raytracer {
	class BVH;
	struct Node;
}

// Information foreach node on the BVH
struct raytracer::Node {
public:
	Node(std::vector<Object*> objects); // objects stored in ndoe
	Node* left = nullptr;			    // left ndoe 
	Node* right = nullptr;			    // right node
	BoundingBox* bounds;				// bounding box for the group of objects
	std::vector<Object*> objects_;		// objects stored in node
	bool isLeaf;						// Store weather this node is a leaf or not
};

class raytracer::BVH {
private:
	// root node
	Node* root_;
	// list of all objects
	std::vector<Object*> objects_;
	// compares two bounding boxes on a axis
	static bool box_compare(const Object* obj1, const Object* obj2, int axis);
	// compares two bounding boxes on the x axis
	static bool box_compare_x(const Object* obj1, const Object* obj2);
	// compares two bounding boxes on the y axis
	static bool box_compare_y(const Object* obj1, const Object* obj2);
	// compares two bounding boxes on the z axis
	static bool box_compare_z(const Object* obj1, const Object* obj2);

	// builds the BVH from a list of objects
	Node* bulidBVH(std::vector<Object*> objects);
	// Finds a list of objects that the ray could intersect
	Node* FindPossiableIntersections(Node* parnetNode, const Ray& ray, std::vector<Object*>& nodes);
public:
	// Constructures
	BVH(std::vector<Object*> objects);
	~BVH() { delete root_; }
	
	// Creates a bounding box for a vector of objects
	static BoundingBox* createBoundingBox(std::vector<Object*> objects);
	// Merge two bounding boxes
	static BoundingBox* mergeBoundingBox(BoundingBox* boxOne, BoundingBox* boxTwo);
	// Finds objects whos bounding box was intersected
	std::vector<raytracer::Object*> getIntersectedObject(const Ray& ray);
};

raytracer::Node::Node(std::vector<Object*> objects) {
	objects_ = objects;
	bounds = BVH::createBoundingBox(objects);
	isLeaf = objects.size() == 1;

}

std::vector<raytracer::Object*> raytracer::BVH::getIntersectedObject(const Ray& ray) {
	std::vector<Object*> objects;
	Node* temp = FindPossiableIntersections(root_, ray, objects);

	return objects;
}

raytracer::Node* raytracer::BVH::FindPossiableIntersections(Node* parnetNode, const Ray& ray, std::vector<Object*>& objects) {
	if (parnetNode != nullptr) {
		if (parnetNode->bounds->hasIntersect(ray)) {
			if (parnetNode->isLeaf) {
				objects.push_back(parnetNode->objects_[0]);
				return parnetNode;
			}
			else {
				Node* left = this->FindPossiableIntersections(parnetNode->left, ray, objects);
				Node* right = this->FindPossiableIntersections(parnetNode->right, ray, objects);

				if (left != nullptr) return left;
				if (right != nullptr) return right;
			}
		}
		else return nullptr;
	}

	return nullptr;
}

raytracer::BVH::BVH(std::vector<Object*> objects) : objects_(objects) {
	root_ = this->bulidBVH(objects_);
}

bool raytracer::BVH::box_compare(const Object* a, const Object* b, int axis) {
	BoundingBox* box1 = a->createBoundingBox();
	BoundingBox* box2 = b->createBoundingBox();

	bool result = box1->getMin()[axis] < box2->getMin()[axis];

	free(box1);
	free(box2);

	return result;
}

bool raytracer::BVH::box_compare_x(const Object* obj1, const Object* obj2) {
	return box_compare(obj1, obj2,0);
}

bool raytracer::BVH::box_compare_y(const Object* obj1, const Object* obj2) {
	return box_compare(obj1, obj2, 1);
}

bool raytracer::BVH::box_compare_z(const Object* obj1, const Object* obj2) {
	return box_compare(obj1, obj2, 2);
}

raytracer::Node* raytracer::BVH::bulidBVH(std::vector<Object*> objects) {
	//std::cout << "Objects ID: " << objects[0][0] << std::endl;
	Node* parnetNode = nullptr;
	if (objects.size() == 0) {
		parnetNode = nullptr;
	}
	else if (objects.size() == 1) {
		parnetNode = new Node(objects);
	}
	else {
		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> randAxis(0, 2);
		int axis = randAxis(rng);

		auto comparator = (axis == 0) ? box_compare_x : ((axis == 1) ? box_compare_y : box_compare_z);

		std::sort(objects.begin(), objects.end(), comparator);

		int mid = objects.size() / 2;
		std::vector<Object*> leftObjects(objects.begin(), objects.begin() + mid);
		std::vector<Object*> rightObjects(objects.begin() + mid, objects.end());

		parnetNode = new Node(objects);
		parnetNode->left = this->bulidBVH(leftObjects);
		parnetNode->right = this->bulidBVH(rightObjects);
		//std::cout << "Left Objects ID: " << parnetNode->left->objects_[0][0] << std::endl;
	}

	return parnetNode;
}

raytracer::BoundingBox* raytracer::BVH::mergeBoundingBox(BoundingBox* boxOne, BoundingBox* boxTwo) {
	

	Vec3 min = Vec3(fmin(boxOne->getMin()[0], boxTwo->getMin()[0]), fmin(boxOne->getMin()[1], boxTwo->getMin()[1]), fmin(boxOne->getMin()[2], boxTwo->getMin()[2]));
	Vec3 max = Vec3(fmax(boxOne->getMax()[0], boxTwo->getMax()[0]), fmax(boxOne->getMax()[1], boxTwo->getMax()[1]), fmax(boxOne->getMax()[2], boxTwo->getMax()[2]));

	BoundingBox* boundingBox = new BoundingBox(min, max);

	return boundingBox;
}

raytracer::BoundingBox* raytracer::BVH::createBoundingBox(std::vector<Object*> objects) {
	
	BoundingBox* finalBox = nullptr;
	BoundingBox* temp;
	bool atFirstBox = true;
	for (int i = 0; i < objects.size(); ++i) {
		temp = objects[i]->createBoundingBox();
		BoundingBox* memeoryManger = finalBox;
		finalBox = atFirstBox ? temp : mergeBoundingBox(temp, finalBox);
		free(memeoryManger);
		if (!atFirstBox) {
			free(temp);
		}
		else {
			atFirstBox = false;
		}
	}

	return finalBox;
}

#endif
