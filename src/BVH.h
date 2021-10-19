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

struct raytracer::Node {
public:
	Node(std::vector<Object*> objects);
	Node* left = nullptr;
	Node* right = nullptr;
	BoundingBox* bounds;
	std::vector<Object*> objects_;
	bool isLeaf;
};

class raytracer::BVH {
private:
	Node* root_;
	std::vector<Object*> objects_;

	static bool box_compare(const Object* obj1, const Object* obj2, int axis);
	static bool box_compare_x(const Object* obj1, const Object* obj2);
	static bool box_compare_y(const Object* obj1, const Object* obj2);
	static bool box_compare_z(const Object* obj1, const Object* obj2);

	Node* bulidBVH(std::vector<Object*> objects);
	Node* getClosestObjectNode(Node* parnetNode, const Ray& ray, std::vector<Object*>& nodes);
public:
	BVH(std::vector<Object*> objects);
	~BVH() { delete root_; }
	

	static BoundingBox* createBoundingBox(std::vector<Object*> objects);
	static BoundingBox* mergeBoundingBox(BoundingBox* boxOne, BoundingBox* boxTwo);
	std::vector<raytracer::Object*> getIntersectedObject(const Ray& ray);
};

raytracer::Node::Node(std::vector<Object*> objects) {
	objects_ = objects;
	bounds = BVH::createBoundingBox(objects);
	isLeaf = objects.size() == 1;

}

std::vector<raytracer::Object*> raytracer::BVH::getIntersectedObject(const Ray& ray) {
	std::vector<Object*> objects;
	Node* temp = getClosestObjectNode(root_, ray, objects);

	return objects;
}

raytracer::Node* raytracer::BVH::getClosestObjectNode(Node* parnetNode, const Ray& ray, std::vector<Object*>& objects) {
	if (parnetNode != nullptr) {
		if (parnetNode->bounds->hasIntersect(ray)) {
			if (parnetNode->isLeaf) {
				objects.push_back(parnetNode->objects_[0]);
				return parnetNode;
			}
			else {
				Node* left = this->getClosestObjectNode(parnetNode->left, ray, objects);
				Node* right = this->getClosestObjectNode(parnetNode->right, ray, objects);

				if (left != nullptr) return left;
				if (right != nullptr) return right;
			}
		}
		else  return nullptr;
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
