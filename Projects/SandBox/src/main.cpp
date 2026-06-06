#include <iostream>
#include "geo/IOUtils.h"


int main(int argc, char** argv)
{
	geo::io::GeometryDumpData cube = geo::io::LoadOBJ(RESOURCES_PATH"models/cube/cube.obj");
	std::cout << cube.ToString() << '\n';

	geo::io::GeometryDumpData bunny = geo::io::LoadOBJ(RESOURCES_PATH"models/bunny/bunny.obj");
	std::cout << bunny.ToString() << '\n';

	return 0;
}
