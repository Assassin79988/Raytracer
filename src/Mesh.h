#ifndef MESH_H_
#define MESH_H_

#include <stdio.h>
#include <iostream>
#include <vector>
#include "Object.h"
#include "Triangle.h"

namespace raytracer{
	class Mesh;
	struct Face;
}

struct raytracer::Face {
	unsigned int vertexIndex[3];
	unsigned int normalIndex[3];
	unsigned int uvIndex[3];
};

class raytracer::Mesh {
private:
	char* path_ = "bunny.obj";
	std::vector<Vec3> vertices_;
	std::vector<Vec2> uvs_;
	std::vector<Vec3> normals_;
	std::vector<Face> faces_;
	std::vector<Triangle> triangles_;

	Mat4 affineTransform_;

	bool objectFileParser();
	void getAffineTransform(Vec3 position, Vec3 rotation, Vec3 scale);
public:
	Mesh() {
		objectFileParser();
	}
	Mesh(char* path, Vec3 position, Vec3 rotation, Vec3 scale) {
		path_ = path;
		getAffineTransform(position, rotation, scale);
		objectFileParser(); 
	}

	void getObjects(std::vector<Object*> &objects);
};

void raytracer::Mesh::getAffineTransform(Vec3 position, Vec3 rotation, Vec3 scale) {
	Mat4 translationMatrix = Mat4::Identity();
	translationMatrix(0, 3) = position[0];
	translationMatrix(1, 3) = position[1];
	translationMatrix(2, 3) = position[2];

	Mat4 scaleMatrix = Mat4::Identity();
	scaleMatrix(0, 0) = scale[0];
	scaleMatrix(1, 1) = -scale[1];
	scaleMatrix(2, 2) = scale[2];

	Mat4 xRotMatrix = Mat4::Identity();
	xRotMatrix(1, 1) = cos(rotation[0]);
	xRotMatrix(2, 1) = sin(rotation[0]);
	xRotMatrix(1, 2) = -sin(rotation[0]);
	xRotMatrix(2, 2) = cos(rotation[0]);

	Mat4 yRotMatrix = Mat4::Identity();
	yRotMatrix(0, 0) = cos(rotation[1]);
	yRotMatrix(0, 2) = sin(rotation[1]);
	yRotMatrix(2, 0) = -sin(rotation[1]);
	yRotMatrix(2, 2) = cos(rotation[1]);

	Mat4 zRotMatrix = Mat4::Identity();
	zRotMatrix(0, 0) = cos(rotation[2]);
	zRotMatrix(0, 1) = -sin(rotation[2]);
	zRotMatrix(1, 0) = sin(rotation[2]);
	zRotMatrix(1, 1) = cos(rotation[2]);

	//TODO: Add shear matrix.
	affineTransform_ = translationMatrix * scaleMatrix * xRotMatrix * yRotMatrix * zRotMatrix;
}

void raytracer::Mesh::getObjects(std::vector<Object*>& objects) {
	for (int i = 0; i < triangles_.size(); ++i) {
		objects.push_back(&triangles_[i]);
	}
}

bool raytracer::Mesh::objectFileParser() {

	FILE* file = fopen(path_, "r");
	if (file == NULL) {
		std::cout << "Failed to open file."<< std::endl;
		return false;
	}

	bool hasNormal = false;
	bool hasNormalIndex = false;
	bool hasUV = false;
	bool hasUVIndex = false;

	while (true) {
		char lineHeader[128];

		int pointer = fscanf(file, "%s", lineHeader);

		if (pointer == EOF) {
			break;
		}
		else if (strcmp(lineHeader, "v") == 0) {
			float vx, vy, vz;
			(void) fscanf(file, "%f %f %f\n", &vx, &vy, &vz);
			Vec4 temp = Vec4(vx, vy, vz, 1);
			temp = affineTransform_ * temp;
			//std::cout << temp << std::endl << std::endl;
			vertices_.push_back(Vec3(temp[0], temp[1], temp[2]));
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			float uvx, uvy;
			(void) fscanf(file, "%f %f\n", &uvx, &uvy);
			uvs_.push_back(Vec2(uvx, uvy));
			hasUV = true;
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			float nx, ny, nz;
			(void) fscanf(file, "%f %f %f\n", &nx, &ny, &nz);
			normals_.push_back(Vec3(nx, ny, nz));
			hasNormal = true;
		}
		else if (strcmp(lineHeader, "f") == 0) {
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
			bool supportedFormat = false;
			bool isQuad = false;
			Face f;
			Face f2;
			char line[128];
			fgets(line, 128, file);
			int matches = sscanf(line, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]);
			if (matches == 9) {
				supportedFormat = true;
				
				f.vertexIndex[0] = vertexIndex[0];
				f.vertexIndex[1] = vertexIndex[1];
				f.vertexIndex[2] = vertexIndex[2];

				hasNormalIndex = true;
				f.normalIndex[0] = normalIndex[0];
				f.normalIndex[1] = normalIndex[1];
				f.normalIndex[2] = normalIndex[2];

				hasUVIndex = true;
				f.uvIndex[0] = uvIndex[0];
				f.uvIndex[1] = uvIndex[1];
				f.uvIndex[2] = uvIndex[2];
			}
			else if (matches == 12) {
				supportedFormat = true;
				isQuad = true;

				f.vertexIndex[0] = vertexIndex[0];
				f.vertexIndex[1] = vertexIndex[1];
				f.vertexIndex[2] = vertexIndex[2];

				f2.vertexIndex[0] = vertexIndex[0];
				f2.vertexIndex[1] = vertexIndex[2];
				f2.vertexIndex[2] = vertexIndex[3];

				hasUVIndex = true;
				f.uvIndex[0] = uvIndex[0];
				f.uvIndex[1] = uvIndex[1];
				f.uvIndex[2] = uvIndex[2];

				f2.uvIndex[0] = uvIndex[0];
				f2.uvIndex[1] = uvIndex[2];
				f2.uvIndex[2] = uvIndex[3];

				hasNormalIndex = true;
				f.normalIndex[0] = normalIndex[0];
				f.normalIndex[1] = normalIndex[1];
				f.normalIndex[2] = normalIndex[2];

				f2.normalIndex[0] = normalIndex[0];
				f2.normalIndex[1] = normalIndex[2];
				f2.normalIndex[2] = normalIndex[3];
			}
			else {
				matches = sscanf(line, "%d//%d %d//%d %d//%d %d//%d", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2], &vertexIndex[3], &normalIndex[3]);
				if (matches == 6) {
					supportedFormat = true;
					
					f.vertexIndex[0] = vertexIndex[0];
					f.vertexIndex[1] = vertexIndex[1];
					f.vertexIndex[2] = vertexIndex[2];

					hasNormalIndex = true;
					f.normalIndex[0] = normalIndex[0];
					f.normalIndex[1] = normalIndex[1];
					f.normalIndex[2] = normalIndex[2];
				}
				else if (matches == 8) {
					supportedFormat = true;
					isQuad = true;

					f.vertexIndex[0] = vertexIndex[0];
					f.vertexIndex[1] = vertexIndex[1];
					f.vertexIndex[2] = vertexIndex[2];

					f2.vertexIndex[0] = vertexIndex[0];
					f2.vertexIndex[1] = vertexIndex[2];
					f2.vertexIndex[2] = vertexIndex[3];

					hasNormalIndex = true;
					f.normalIndex[0] = normalIndex[0];
					f.normalIndex[1] = normalIndex[1];
					f.normalIndex[2] = normalIndex[2];

					f2.normalIndex[0] = normalIndex[0];
					f2.normalIndex[1] = normalIndex[2];
					f2.normalIndex[2] = normalIndex[3];
				}
				else {
					matches = sscanf(line, "%d/%d %d/%d %d/%d %d/%d", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2], &vertexIndex[3], &uvIndex[3]);
					if (matches == 6) {
						supportedFormat = true;
						
						f.vertexIndex[0] = vertexIndex[0];
						f.vertexIndex[1] = vertexIndex[1];
						f.vertexIndex[2] = vertexIndex[2];

						hasUVIndex = true;
						f.uvIndex[0] = uvIndex[0];
						f.uvIndex[1] = uvIndex[1];
						f.uvIndex[2] = uvIndex[2];
					}
					else if (matches == 8) {
						supportedFormat = true;
						isQuad = true;

						f.vertexIndex[0] = vertexIndex[0];
						f.vertexIndex[1] = vertexIndex[1];
						f.vertexIndex[2] = vertexIndex[2];

						f2.vertexIndex[0] = vertexIndex[0];
						f2.vertexIndex[1] = vertexIndex[2];
						f2.vertexIndex[2] = vertexIndex[3];

						hasUVIndex = true;
						f.uvIndex[0] = uvIndex[0];
						f.uvIndex[1] = uvIndex[1];
						f.uvIndex[2] = uvIndex[2];

						f2.uvIndex[0] = uvIndex[0];
						f2.uvIndex[1] = uvIndex[2];
						f2.uvIndex[2] = uvIndex[3];
					}
					else {
						matches = sscanf(line, "%d %d %d %d", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2], &vertexIndex[3]);
						if (matches == 3) {
							supportedFormat = true;
							f.vertexIndex[0] = vertexIndex[0];
							f.vertexIndex[1] = vertexIndex[1];
							f.vertexIndex[2] = vertexIndex[2];
						}
						else if (matches == 4) {
							supportedFormat = true;
							isQuad = true;

							f.vertexIndex[0] = vertexIndex[0];
							f.vertexIndex[1] = vertexIndex[1];
							f.vertexIndex[2] = vertexIndex[2];

							f2.vertexIndex[0] = vertexIndex[0];
							f2.vertexIndex[1] = vertexIndex[2];
							f2.vertexIndex[2] = vertexIndex[3];
						}
					}
				}
			}

			if (!supportedFormat) {
				std::cout << "File can't be read by this parser (Try exporting with other options)" << std::endl;
				return false;
			}

			if (!isQuad) {
				faces_.push_back(f);
			}
			else {
				faces_.push_back(f);
				faces_.push_back(f2);
			}
		
		}
	}

	for (int i = 0; i < faces_.size(); ++i) {
		Vec3 v1 = vertices_[faces_[i].vertexIndex[0] - 1];
		Vec3 v2 = vertices_[faces_[i].vertexIndex[1] - 1];
		Vec3 v3 = vertices_[faces_[i].vertexIndex[2] - 1];
		if (!hasNormal || !hasNormalIndex) {
			triangles_.push_back(Triangle(v1, v2, v3));
		}
		else {
			Vec3 vn1 = normals_[faces_[i].normalIndex[0] - 1];
			Vec3 vn2 = normals_[faces_[i].normalIndex[1] - 1];
			Vec3 vn3 = normals_[faces_[i].normalIndex[2] - 1];
			triangles_.push_back(Triangle(v1, v2, v3, vn1, vn2, vn3));
		}
	}
	
	int v1C = 0;
	int v2C = 0;
	int v3C = 0;

	if (!hasNormal || !hasNormalIndex) {
		for (int v = 0; v < vertices_.size(); ++v) {
			std::vector<int*> data;
			float avgNormalX = 0.0;
			float avgNormalY = 0.0;
			float avgNormalZ = 0.0;
			for (int t = 0; t < triangles_.size(); ++t) {
				if (v == (faces_[t].vertexIndex[0] - 1)) {
					int* d = new int[2];
					d[0] = t;
					d[1] = 0;
					data.push_back(d);
					Vec3 faceNormal = triangles_[t].getFaceNormal();
					avgNormalX += faceNormal[0];
					avgNormalY += faceNormal[1];
					avgNormalZ += faceNormal[2];

				}
				else if (v == (faces_[t].vertexIndex[1] - 1)) {
					int* d = new int[2];
					d[0] = t;
					d[1] = 1;
					data.push_back(d);
					Vec3 faceNormal = triangles_[t].getFaceNormal();
					avgNormalX += faceNormal[0];
					avgNormalY += faceNormal[1];
					avgNormalZ += faceNormal[2];
				}
				else if (v == (faces_[t].vertexIndex[2] - 1)) {
					int* d = new int[2];
					d[0] = t;
					d[1] = 2;
					data.push_back(d);
					Vec3 faceNormal = triangles_[t].getFaceNormal();
					avgNormalX += faceNormal[0];
					avgNormalY += faceNormal[1];
					avgNormalZ += faceNormal[2];
				}
			}

			Vec3 avgNormal = Vec3(avgNormalX, avgNormalY, avgNormalZ);
			avgNormal.normalize();
			//d::cout << avgNormal << std::endl;
			for (int i = 0; i < data.size(); ++i) {
				if (data[i][1] == 0) {
					v1C++;
					triangles_[data[i][0]].setVn1(avgNormal);
					free(data[i]);
				}
				else if (data[i][1] == 1) {
					v2C++;
					triangles_[data[i][0]].setVn2(avgNormal);
					free(data[i]);
				}
				else if (data[i][1] == 2) {
					v3C++;
					triangles_[data[i][0]].setVn3(avgNormal);
					free(data[i]);
				}
			}

		}
	}
	
	return true;
}

#endif
