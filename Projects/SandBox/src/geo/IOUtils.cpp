#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <geo/utils/logging/LogMacros.h>

#include "IOUtils.h"


namespace geo
{
	// ============================================================
	// GeometryDumpData
	// ============================================================

	std::string GeometryDumpData::ToString() const
	{
		std::stringstream ss;

		auto GeometryTypeToString = [](GeometryType type)
			{
				switch (type)
				{
				case GeometryType::POINT_CLOUD:   return "PointCloud";
				case GeometryType::TRIANGLE_MESH: return "TriangleMesh";
				default:                          return "Unknown";
				}
			};

		ss << "GeometryDumpData\n";
		ss << "----------------\n";

		ss << "File:      " << filePath.filename().string() << '\n';
		ss << "Geometry:  " << GeometryTypeToString(geometryType) << '\n';
						  
		ss << "Vertices:  " << points.size()       << '\n';
		ss << "Normals:   " << normals.size()      << '\n';
		ss << "TexCoords: " << texcoords.size()    << '\n';

		ss << "Triangles: " << indexBuffer.size()  << '\n';
		ss << "Materials: " << materialsMap.size() << '\n';

		if (bbox.IsValid())
		{
			const glm::vec3 bmin = bbox.Min();
			const glm::vec3 bmax = bbox.Max();
			const glm::vec3 size = bbox.Size();

			ss << '\n';
			ss << std::fixed << std::setprecision(4) << "BBox Min:    (" << bmin.x << ", " << bmin.y << ", " << bmin.z << ")\n";
			ss << std::fixed << std::setprecision(4) << "BBox Max:    (" << bmax.x << ", " << bmax.y << ", " << bmax.z << ")\n";
			ss << std::fixed << std::setprecision(4) << "BBox Size:   (" << size.x << ", " << size.y << ", " << size.z << ")\n";
			ss << std::fixed << std::setprecision(4) << "BBox Radius: " << bbox.Radius() << '\n';
		}

		return ss.str();
	}

	// ============================================================
	// Utilities
	// ============================================================

	std::string ReadFile(const std::string& filepath)
	{
		std::ifstream input_file(filepath, std::ios::binary);

		if (!input_file.is_open())
		{
			GEOLOGERROR("Failed to open file at " << filepath.c_str());
			return std::string();
		}

		return std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
	}

	bool FileExists(const std::filesystem::path& path)
	{
		return std::filesystem::exists(path);
	}

	FileType GetFileType(const std::filesystem::path& path)
	{
		if (!path.has_extension())
		{
			return FileType::UNKNOWN;
		}

		std::string ext = path.extension().string();

		std::transform(ext.begin(), ext.end(), ext.begin(),
			[](unsigned char c)
			{
				return static_cast<char>(std::tolower(c));
			});

		if (ext == ".obj")
		{
			return FileType::OBJ;
		}

		if (ext == ".ply")
		{
			return FileType::PLY;
		}

		return FileType::UNKNOWN;
	}

	std::string GetFileName(const std::filesystem::path& path)
	{
		return path.filename().string();
	}

	std::filesystem::path GetParentFolder(const std::filesystem::path& filePath)
	{
		return filePath.parent_path();
	}

	// ============================================================
	// OBJ Loader
	// ============================================================

	GeometryDumpData LoadOBJ(const std::filesystem::path& path)
	{
		GeometryDumpData data;
		data.filePath = path;
		data.fileType = FileType::OBJ;
		data.geometryType = GeometryType::TRIANGLE_MESH;

		tinyobj::ObjReaderConfig cfg;
		cfg.mtl_search_path = path.parent_path().string();
		cfg.mtl_search_path = "";
		cfg.vertex_color = false;
		cfg.triangulate = true;  // force triangles

		tinyobj::ObjReader reader;
		if (!reader.ParseFromFile(path.string(), cfg)) {
			GEOLOGERROR("Failed to load OBJ: " << path << "\n" << reader.Error());
			return data;
		}

		if (!reader.Warning().empty()) {
			GEOLOGWARN(reader.Warning());
		}

		const auto& attrib = reader.GetAttrib();
		const auto& shapes = reader.GetShapes();
		const auto& materials = reader.GetMaterials();

		// --------------------------------------------------------------
		// Points
		// --------------------------------------------------------------

		data.points.resize(attrib.vertices.size() / 3);
		for (size_t i = 0, v = 0; i < attrib.vertices.size(); i += 3, v++)
		{
			data.points[v] = glm::vec3(attrib.vertices[i + 0], attrib.vertices[i + 1], attrib.vertices[i + 2]);
		}

		// --------------------------------------------------------------
		// Normals
		// --------------------------------------------------------------

		if (!attrib.normals.empty())
		{
			data.normals.resize(attrib.normals.size() / 3);
			for (size_t i = 0, n = 0; i < attrib.normals.size(); i += 3, n++)
			{
				data.normals[n] = glm::vec3(attrib.normals[i + 0], attrib.normals[i + 1], attrib.normals[i + 2]);
			}
		}

		// --------------------------------------------------------------
		// TexCoord
		// --------------------------------------------------------------
		if (!attrib.texcoords.empty())
		{
			data.texcoords.resize(attrib.texcoords.size() / 2);
			for (size_t i = 0, n = 0; i < attrib.texcoords.size(); i += 2, n++)
			{
				data.texcoords[n] = glm::vec2(attrib.texcoords[i + 0], attrib.texcoords[i + 1]);
			}
		}

		// --------------------------------------------------------
		// Triangles
		// --------------------------------------------------------

		for (const auto& shape : shapes)
		{
			size_t indexOffset = 0;
			for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
			{
				const int fv = shape.mesh.num_face_vertices[f];

				if (fv != 3)
				{
					GEOLOGWARN("Non-triangle face encountered after triangulation (unexpected)");
					indexOffset += fv;
					continue;
				}

				TriangleIndex tri = {};

				for (int v = 0; v < 3; v++)
				{
					tri.vertexIndex[v] = (u32)shape.mesh.indices[indexOffset + v].vertex_index;
					tri.normalIndex[v] = (u32)shape.mesh.indices[indexOffset + v].normal_index;
					tri.coordsIndex[v] = (u32)shape.mesh.indices[indexOffset + v].texcoord_index;
				}

				data.indexBuffer.emplace_back(tri);
				indexOffset += 3;
			}
		}

		// --------------------------------------------------------------
		// materials
		// --------------------------------------------------------------

		for (const auto& m : materials)
		{
			Material mat;
			mat.name = m.name;
			mat.base_color = { m.diffuse[0], m.diffuse[1], m.diffuse[2] };

			// Phong -> PBR approximation
			float shininess = std::clamp(m.shininess, 0.0f, 1000.0f);
			mat.roughness = std::clamp(std::sqrt(1.0f - shininess / 1000.0f), 0.0f, 1.0f);
			float specBright = 0.2126f * m.specular[0] + 0.7152f * m.specular[1] + 0.0722f * m.specular[2];
			mat.metallic = std::clamp(specBright, 0.0f, 1.0f);
			mat.reflectance = 0.05f;  // dielectric default

			mat.texture_file_color = m.diffuse_texname;
			mat.texture_file_normal = m.normal_texname.empty() ? m.bump_texname: m.normal_texname;
			mat.texture_file_mask = m.specular_highlight_texname;

			data.materialsMap[m.name] = std::move(mat);
		}

		// --------------------------------------------------------
		// Bounding box
		// --------------------------------------------------------
		if (!data.points.empty())
		{
			data.bbox.MakeEmpty();

			for (const auto& p : data.points)
			{
				data.bbox.ExpandBy(p);
			}
		}

		GEOLOGINFO("Loaded OBJ: " << path
			<< " | vertices: " << data.points.size()
			<< " | triangles: " << data.indexBuffer.size()
			<< " | normals: " << (data.HasNormals() ? "yes" : "no"));

		return data;
	}

	// ============================================================
	// Obj Saving
	// ============================================================

	bool SaveOBJ(const std::filesystem::path& path, const GeometryDumpData& data)
	{
		return false;
	}



}
