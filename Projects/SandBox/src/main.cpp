#include <iostream>
#include "geo/IOUtils.h"


int main(int argc, char** argv)
{
	geo::GeometryDumpData cube = geo::LoadOBJ(RESOURCES_PATH"models/cube/cube.obj");
	std::cout << cube.ToString() << '\n';

	geo::GeometryDumpData bunny = geo::LoadOBJ(RESOURCES_PATH"models/bunny/bunny.obj");
	std::cout << bunny.ToString() << '\n';

	return 0;
}
