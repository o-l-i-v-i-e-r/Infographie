#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "Model.h"

Model::Model(const char* file) {
  std::ifstream stream;
  stream.open(file, std::ios::in);
  std::string content;
  while(!stream.eof()) {
    std::getline(stream, content);
    if(!content.compare(0,2,"v ")) {
      float x,y,z;
           
     const char* chh=content.c_str();
    sscanf (chh, "v %f %f %f",&x,&y,&z);
      Vec3f v(x,y,z) ; 
      vertices.push_back(v);
      
    }
    if(!content.compare(0,1,"f")) {
		  
	int p0,p1,p2; 
    int vn0,vn1,vn2; 
    int vt0, vt1, vt2;
  const char* chh=content.c_str();
    sscanf (chh, "f %i/%i/%i %i/%i/%i %i/%i/%i",&p0,&vt0,&vn0,&p1,&vt1,&vn1,&p2,&vt2,&vn2);
  
		p0--;p1--;p2--;
		vn0--;vn1--;vn2--;
		vt0--; vt1--;vt2--;
		
	
	Vec3i f(p0, p1, p2);
	Vec3i vn(vn0, vn1, vn2);
	Vec3i vt(vt0, vt1, vt2);
	
     faces.push_back(f);
     indexNv.push_back(vn);
     indexVt.push_back(vt);
    }
    if(!content.compare(0,2,"vn")) {
		float x,y,z;
		 const char* chh=content.c_str();
		sscanf (chh, "vn %f %f %f",&x,&y,&z);	
		Vec3f nv(x,y,z);
		nVectorList.push_back(nv);
	}
	if(!content.compare(0,2,"vt")) {
		float x,y,z;
		 const char* chh=content.c_str();
		sscanf (chh, "vt %f %f %f",&x,&y,&z);	
		Vec3f vt(x,y,z);
		vtList.push_back(vt);
	}
  }
 
  stream.close();
}

int Model::nbFaces() {
  return (int)faces.size();
}

int Model::nbVertices() {
  return (int)vertices.size();
}

std::vector<Vec3f> Model::getVertices() {
	return vertices;
}

std::vector<Vec3i> Model::getFaces() {
	return faces;
}
 
std::vector<Vec3f> Model::getNVectorList() {
	return nVectorList;
}

std::vector<Vec3i> Model::getIndexNv() {
	return indexNv; 
}

std::vector<Vec3i> Model::getIndexVt() {
	return indexVt;
}

std::vector<Vec3f> Model::getVtList() {
	return vtList;
}
