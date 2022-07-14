#pragma once

#include <vector>
#include <sstream>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "Vortex/Core/Core.h"
#include "Vortex/Renderer/Material.h"
#include "Vortex/Geo/Mesh.h"

namespace Vortex
{
	class SceneImporter
	{
	public:
		SceneImporter(const char* filename);

		bool ImportMeshAndMat(std::vector< Ref<Mesh> >& meshes,
			std::vector< Ref<Material> >& materials);

		std::string GetMaterialInfo(const aiMaterial* mat);

		std::string GetMeshesInfo();

		std::string GetNodesInfo();
	private:
		std::string _GetNodesInfo(const aiNode* root, int depth);

		// set mesh and material, if not find material, ptr of mat will be null
		bool ParseAssimpMesh(const aiMesh* inMesh, Ref<Mesh>& outMesh, Ref<Material>& outMat);

		// parse texture or load from library if exists
		bool ParseAssimpTextures(const aiMaterial* inMat, aiTextureType texType, Ref<Material>& mat);

	private:
		Assimp::Importer	m_Importer;
		const aiScene*		m_Scene;
		const char*			m_Filepath;
		std::string			m_Dirpath;		// start with assets/...
	};
}