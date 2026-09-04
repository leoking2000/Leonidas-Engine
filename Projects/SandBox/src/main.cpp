#include <iostream>
#include "geo/IOUtils.h"

static void PrintSeparator(const std::string& title)
{
    std::cout << "\n=====================================================\n";
    std::cout << title << "\n";
    std::cout << "=====================================================\n";
}

static void TestFile(const std::string& path)
{
    PrintSeparator("Testing: " + path);

    // -----------------------------------------------------
    // Load via automatic loader
    // -----------------------------------------------------
    geo::io::GeometryDumpData dataAuto = geo::io::LoadGeometry(path);
    std::cout << "[LoadGeometry]\n";
    std::cout << dataAuto.ToString() << "\n";

    // -----------------------------------------------------
    // Explicit format loads (if applicable)
    // -----------------------------------------------------
    if (path.find(".obj") != std::string::npos)
    {
        geo::io::GeometryDumpData dataOBJ = geo::io::LoadOBJ(path);
        std::cout << "[LoadOBJ]\n";
        std::cout << dataOBJ.ToString() << "\n";

        geo::io::SaveOBJ("test_roundtrip.obj", dataOBJ);
        std::cout << "Saved OBJ roundtrip\n";
    }

    if (path.find(".ply") != std::string::npos)
    {
        geo::io::GeometryDumpData dataPLY = geo::io::LoadPLY(path);
        std::cout << "[LoadPLY]\n";
        std::cout << dataPLY.ToString() << "\n";

        geo::io::SavePLY("test_roundtrip.ply", dataPLY);
        std::cout << "Saved PLY roundtrip\n";
    }

    // -----------------------------------------------------
    // Mesh conversion test
    // -----------------------------------------------------
    geo::io::MeshData mesh = geo::io::MakeMeshData(dataAuto);

    std::cout << "\n[MeshData]\n";
    std::cout << "Vertices:  " << mesh.vertexBuffer.size() << "\n";
    std::cout << "Triangles: " << mesh.indexBuffer.size() / 3 << "\n";
    std::cout << "Indices:   " << mesh.indexBuffer.size() << "\n";

    // -----------------------------------------------------
    // Point cloud conversion test
    // -----------------------------------------------------
    geo::io::PointCloud cloud = geo::io::MakePointCloud(dataAuto);

    std::cout << "\n[PointCloud]\n";
    std::cout << "Points:  " << cloud.points.size() << "\n";
    std::cout << "Normals: " << cloud.normals.size() << "\n";
    std::cout << "Colors:  " << cloud.colors.size() << "\n";

    // -----------------------------------------------------
    // Consistency checks (VERY IMPORTANT)
    // -----------------------------------------------------
    std::cout << "\n[Checks]\n";

    if (!dataAuto.points.empty())
        std::cout << "Points match dump: " << (cloud.points.size() == dataAuto.points.size()) << "\n";

    if (!dataAuto.normals.empty())
        std::cout << "Normals exist: " << dataAuto.normals.size() << "\n";

    std::cout << "Mesh validity: " << (!mesh.vertexBuffer.empty() && !mesh.indexBuffer.empty()) << "\n";
}

int main(int argc, char** argv)
{
    std::vector<std::string> testFiles =
    {
        RESOURCES_PATH "models/cube/cube.obj",
        RESOURCES_PATH "models/bunny/bunny.obj",
        RESOURCES_PATH "models/owl/owl-decimate10pc-textured.ply",
        RESOURCES_PATH "models/owl/owl-clean.ply"
    };

    for (const auto& file : testFiles)
    {
        TestFile(file);
    }

    return 0;
}