#include <iostream>
#include "geo/IOUtils.h"


int main(int argc, char** argv)
{
	//geo::io::GeometryDumpData meshData = geo::io::LoadOBJ(RESOURCES_PATH"models/cube/cube.obj");
	//std::cout << meshData.ToString() << '\n';

	geo::io::GeometryDumpData meshData = geo::io::LoadOBJ(RESOURCES_PATH"models/bunny/bunny.obj");
	std::cout << meshData.ToString() << '\n';

	geo::io::MeshData m = geo::io::MakeMeshData(meshData);
	std::cout << "Mesh cube | vertixes: " << m.vertexBuffer.size() << " triangles: " << m.indexBuffer.size() << "\n";

	geo::io::PointCloud c = geo::io::MakePointCloud(meshData);
	std::cout << "Pointcloud cube |" <<
		" points: "  << c.points.size() <<
		" normals: " << c.points.size() <<
		" collos: "  << c.colors.size() << '\n';

	return 0;
}
