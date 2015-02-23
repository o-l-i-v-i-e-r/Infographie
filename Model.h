#include <vector>
#include "geometry.h"

class Model {
	public: 
		Model(const char* file);
		int nbFaces();
		int nbVertices();
		std::vector<Vec3f> vertices;
		std::vector<Vec3i> faces;
		std::vector<Vec3f> nVectorList; 
		std::vector<Vec3f> vtList;
		std::vector<Vec3i> indexNv;
		std::vector<Vec3i> indexVt;
		std::vector<Vec3f> getVertices();
		std::vector<Vec3i> getFaces();
		std::vector<Vec3f> getNVectorList();
		std::vector<Vec3f> getVtList();
		std::vector<Vec3i> getIndexNv();
		std::vector<Vec3i> getIndexVt();
};
