#include "vtpch.h"
#include "SceneImporter.h"

namespace Vortex
{
	SceneImporter::SceneImporter(const char* filepath)
		:m_Filepath(filepath)
	{
		std::string fp(filepath);
		size_t idx = fp.find_last_of("/\\");
		m_Dirpath = fp.substr(0, idx);

		m_Scene = m_Importer.ReadFile(filepath,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);

		// If the import failed, report it
		if (nullptr == m_Scene) {
			VT_CORE_ERROR(m_Importer.GetErrorString());
			return;
		}
	}

	bool SceneImporter::ImportMeshAndMat(std::vector< Ref<Mesh> >& meshes,
		std::vector< Ref<Material> >& materials)
	{
		if (!m_Scene->HasMeshes())
		{
			VT_CORE_ERROR("Can't find mesh in {}", m_Filepath);
			return false;
		}

		for (int i = 0; i < m_Scene->mNumMeshes; i++)
		{
			Ref<Mesh> mesh;
			Ref<Material> mat;
			if (ParseAssimpMesh(m_Scene->mMeshes[i], mesh, mat))
			{
				meshes.push_back(mesh);
				materials.push_back(mat);
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	std::string SceneImporter::GetMaterialInfo(const aiMaterial* mat)
	{
		std::ostringstream builder;

		builder << "> Material Name: " << mat->GetName().C_Str() << "\n";
		std::vector< Pair<std::string, aiTextureType> > types = {
			{"Ambient", aiTextureType_AMBIENT},
			{"Diffuse", aiTextureType_DIFFUSE},
			{"Specular", aiTextureType_SPECULAR},
			{"Reflection", aiTextureType_REFLECTION},
			{"Normal", aiTextureType_NORMALS},
			{"Height", aiTextureType_HEIGHT}
		};
		// textures
		for (auto& t : types)
		{
			int cnt = mat->GetTextureCount(t.second);
			if (cnt > 0)
			{
				builder << t.first << " Texture Count: " << cnt << "\n";
				for (int i = 0; i < cnt; i++)
				{
					aiString str;
					mat->GetTexture(t.second, i, &str);
					builder << "Texture " << i << ": " << str.C_Str() << "\n";
				}
			}
		}

		// properties
		if (mat->mNumProperties > 0)
		{
			builder << "Property Count: " << mat->mNumProperties << "\n";
			for (int i = 0; i < mat->mNumProperties; i++)
			{
				aiMaterialProperty* prop = mat->mProperties[i];
				aiString key = prop->mKey;
				aiPropertyTypeInfo tInfo = prop->mType;
				builder << key.C_Str() << ": ";
				switch (tInfo)
				{
				case aiPropertyTypeInfo::aiPTI_Integer:
				{
					unsigned len = prop->mDataLength / sizeof(int);
					int* arrInt = new int[len];
					if (AI_SUCCESS
						== mat->Get(key.C_Str(), -1, -1, arrInt, &len))
					{
						for(int j = 0; j < len; j++)
							builder << arrInt[j] << " ";
					}
					delete[] arrInt;
					break;
				}
				// https://github.com/assimp/assimp/issues/2504
				case aiPropertyTypeInfo::aiPTI_Float:
				{
					unsigned len = prop->mDataLength / sizeof(float);
					float* arrFloat = new float[len];
					if (AI_SUCCESS
						== mat->Get(key.C_Str(), -1, -1, arrFloat, &len))
					{
						for (int j = 0; j < len; j++)
							builder << arrFloat[j] << " ";
					}
					delete[] arrFloat;
					break;
				}
				case aiPropertyTypeInfo::aiPTI_Double:
				{
					unsigned len = prop->mDataLength / sizeof(double);
					double* arrDouble = new double[len];
					if (AI_SUCCESS
						== mat->Get(key.C_Str(), -1, -1, arrDouble, &len))
					{
						for (int j = 0; j < len; j++)
							builder << arrDouble[j] << " ";
					}
					delete[] arrDouble;
					break;
				}
				case aiPropertyTypeInfo::aiPTI_String:
				{
					aiString valStr;
					if (AI_SUCCESS
						== mat->Get(key.C_Str(), -1, -1, valStr))
					{
						builder << valStr.C_Str();
					}
					break;
				}
				case aiPropertyTypeInfo::aiPTI_Buffer:
				{
					builder << "Buffer Length: " << prop->mDataLength;
					break;
				}
				default:
				{
					if (prop->mIndex > 0)
					{
						builder << "Texture Index: " << prop->mIndex;
					}
					break;
				}
				}
				builder << "\n";
			}
		}

		return builder.str();
	}

	std::string SceneImporter::GetMeshesInfo()
	{
		std::ostringstream builder;
		
		builder << "Filepath: " << m_Filepath << "\n";
		builder << "Mesh Count: " << m_Scene->mNumMeshes << "\n";
		for (int i = 0; i < m_Scene->mNumMeshes; i++)
		{
			auto mesh = m_Scene->mMeshes[i];
			builder << "> Mesh Name: " << mesh->mName.C_Str() << "\n";
			builder << "Vertices Count: " << mesh->mNumVertices << "\n";
			if(mesh->HasNormals())
				builder << "Normals Count: " << mesh->mNumVertices << "\n";
			if (mesh->HasFaces())
				builder << "Faces Count: " << mesh->mNumFaces << "\n";
			
			int texIdx = 0;
			while (mesh->HasTextureCoords(texIdx))
			{
				if (texIdx == 0)
					builder << "Has TextureCoords\n";
				builder << "Index " << texIdx;
				if (mesh->HasTextureCoordsName(texIdx))
					builder << ": " << mesh->mTextureCoordsNames[texIdx]->C_Str();
				builder << "\n";
				texIdx++;
			}
			if (mesh->HasTangentsAndBitangents())
				builder << "Tangent Bitangent Pair Count: " << mesh->mNumVertices << "\n";
			
			if (m_Scene->HasMaterials())
			{
				auto mat = m_Scene->mMaterials[mesh->mMaterialIndex];
				
				builder << "\n" << GetMaterialInfo(mat) << "\n";
			}
			builder << "\n";
		}

		return builder.str();
	}

	std::string SceneImporter::GetNodesInfo()
	{
		std::ostringstream builder;
		builder << "Filepath: " << m_Filepath << "\n";
		builder << _GetNodesInfo(m_Scene->mRootNode, 0);
		return builder.str();
	}

	std::string SceneImporter::_GetNodesInfo(const aiNode* root, int depth)
	{
		if (root == nullptr)return "";
		std::ostringstream builder;
		
		auto fmt = [](int d)->std::string{
			std::ostringstream ss;
			for (int i = 0; i < d; i++)
			{
				ss << "\t";
			}
			return ss.str();
		};
		builder << fmt(depth) << "> Node Name: " << root->mName.C_Str() << "\n";
		builder << fmt(depth) << "  Mesh Count: " << root->mNumMeshes << "\n";
		for (int i = 0; i < root->mNumChildren; i++)
		{
			builder << _GetNodesInfo(root->mChildren[i], depth + 1);
		}

		return builder.str();
	}

	bool SceneImporter::ParseAssimpMesh(const aiMesh* inMesh, Ref<Mesh>& outMesh, Ref<Material>& outMat)
	{
		VT_CORE_ASSERT(outMesh == nullptr, "output mesh is not null, may loss data");
		VT_CORE_ASSERT(outMat == nullptr, "output material is not null, may loss data");

		outMesh = CreateRef<Mesh>(std::string(inMesh->mName.C_Str()));
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
		if (m_Scene->HasMaterials())
		{
			aiMaterial* aiMat = m_Scene->mMaterials[inMesh->mMaterialIndex];

			outMat = CreateRef<Material>();
			// diffuse
			ParseAssimpTextures(aiMat, aiTextureType_DIFFUSE, outMat);
			// specular
			ParseAssimpTextures(aiMat, aiTextureType_SPECULAR, outMat);
			// normal
			ParseAssimpTextures(aiMat, aiTextureType_NORMALS, outMat);
			// height
			ParseAssimpTextures(aiMat, aiTextureType_HEIGHT, outMat);
		}

		return true;
	}
	bool SceneImporter::ParseAssimpTextures(const aiMaterial* inMat, aiTextureType texType, Ref<Material>& mat)
	{
		std::unordered_map<aiTextureType, std::string> slotNames = {
			{aiTextureType_DIFFUSE, "texture_diffuse"},
			{aiTextureType_SPECULAR, "texture_specular"},
			{aiTextureType_AMBIENT, "texture_ambient"},
			{aiTextureType_NORMALS, "texture_normal"},
			{aiTextureType_HEIGHT, "texture_height"}
		};

		for (int i = 0; i < inMat->GetTextureCount(texType); i++)
		{
			// texture path relative to the path of model
			aiString relPath;
			inMat->GetTexture(texType, i, &relPath);
			std::string filepath = m_Dirpath + "/" + std::string(relPath.C_Str());
			// load tex
			Ref<Texture> tex;
			TextureLibrary& ins = TextureLibrary::Instance();
			if (ins.Exists(filepath))
			{
				tex = ins.Get(filepath);
			}
			else
			{
				tex = ins.Load(filepath);
			}

			// texture_xxxN = tex
			mat->AddTexture(slotNames[texType] + std::to_string(i + 1), tex);
		}
		return true;
	}
}