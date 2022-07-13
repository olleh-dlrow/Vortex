#include "vtpch.h"
#include "Mesh.h"


namespace Vortex
{
	Mesh::Mesh()
	{

	}

	MeshImporter::MeshImporter()
	{
	}
	bool MeshImporter::ImportFromFile(const char* filename, std::vector< Ref<Mesh> >& meshes)
	{
		m_Scene = m_Importer.ReadFile(filename,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);

		// If the import failed, report it
		if (nullptr == m_Scene) {
			VT_CORE_ERROR(m_Importer.GetErrorString());
			return false;
		}

		if (!m_Scene->HasMeshes())
		{
			VT_CORE_ERROR("Can't find mesh in {}", filename);
			return false;
		}

		for (int i = 0; i < m_Scene->mNumMeshes; i++)
		{
			Ref<Mesh> mesh;
			if (ParseAssimpMesh(m_Scene->mMeshes[i], mesh))
			{
				meshes.push_back(mesh);
			}
			else
			{
				return false;
			}
		}

		return true;
	}

	bool MeshImporter::ParseAssimpMesh(const aiMesh* inMesh, Ref<Mesh>& outMesh)
	{
		VT_CORE_ASSERT(outMesh == nullptr, "output mesh is not null, may loss data");
		
		outMesh = CreateRef<Mesh>();
		// import vertices
		outMesh->m_Vertices.resize(inMesh->mNumVertices);
		if (inMesh->HasPositions())
		{
			for (int i = 0; i < inMesh->mNumVertices; i++)
			{
				aiVector3D& inVert = inMesh->mVertices[i];
				outMesh->m_Vertices[i].position = glm::vec3(inVert.x, inVert.y, inVert.z);
			}
		}
		if (inMesh->HasNormals())
		{
			for (int i = 0; i < inMesh->mNumVertices; i++)
			{
				aiVector3D& inNormal = inMesh->mNormals[i];
				outMesh->m_Vertices[i].normal = glm::vec3(inNormal.x, inNormal.y, inNormal.z);
			}
		}
		// assume we only use one texture coords
		if (inMesh->HasTextureCoords(0))
		{
			for (int i = 0; i < inMesh->mNumVertices; i++)
			{
				aiVector3D& inTC = inMesh->mTextureCoords[0][i];
				outMesh->m_Vertices[i].texCoord = glm::vec2(inTC.x, inTC.y);
			}
		}
		if (inMesh->HasTangentsAndBitangents())
		{
			for (int i = 0; i < inMesh->mNumVertices; i++)
			{
				aiVector3D& inTan = inMesh->mTangents[i];
				aiVector3D& inBiTan = inMesh->mBitangents[i];
				outMesh->m_Vertices[i].tangent = glm::vec3(inTan.x, inTan.y, inTan.z);
				outMesh->m_Vertices[i].biTangent = glm::vec3(inBiTan.x, inBiTan.y, inBiTan.z);
			}
		}

		// import faces
		if (inMesh->HasFaces())
		{
			for (int i = 0; i < inMesh->mNumFaces; i++)
			{
				aiFace* face = inMesh->mFaces + i;
				for (int j = 0; j < face->mNumIndices; j++)
				{
					outMesh->m_Indices.push_back(face->mIndices[j]);
				}
			}
		}

		// import materials
		// mesh -> mat
		//aiMaterial mat;
		//int idx = inMesh->mMaterialIndex;
		//auto m = m_Scene->mMaterials[idx];
		

		return true;
	}
}