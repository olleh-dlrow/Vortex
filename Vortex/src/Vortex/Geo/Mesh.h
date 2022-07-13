#pragma once

#include <vector>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "Vortex/Core/Core.h"
#include "Vortex/Renderer/Vertex.h"

namespace Vortex
{
	class Mesh
	{
	public:
		Mesh();

		std::vector<MeshVertex> m_Vertices;
		std::vector<int>		m_Indices;
	};

	class MeshImporter
	{
	public:
		MeshImporter();

		bool ImportFromFile(const char* filename, std::vector< Ref<Mesh> >& meshes);

		bool ParseAssimpMesh(const aiMesh* inMesh, Ref<Mesh>& outMesh);

	private:
		Assimp::Importer m_Importer;
		const aiScene* m_Scene;

	};
}
