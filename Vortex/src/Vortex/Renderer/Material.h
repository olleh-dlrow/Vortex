#pragma once

#include<unordered_map>
#include"Shader.h"
#include"Vortex/Core/Core.h"
#include"Texture.h"

namespace Vortex
{
	class Material
	{
	public:
		Material();
		Material(const std::string& name);

		void ApplyProperties();

		// add texture to map, not set shader
		void AddTexture(const std::string& name, const Ref<Texture>& tex);
		
		void SetTexture(const std::string& name, const Ref<Texture>& tex);
		Texture* GetTexture(const std::string& name);

		// bind all texture to slots
		void BindTextures();

		void SetFloat(const std::string& name, float val);
		float GetFloat(const std::string& name);

		void SetFloat3(const std::string& name, const glm::vec3& val);
		glm::vec3 GetFloat3(const std::string& name);

		void SetFloat4(const std::string& name, const glm::vec4& val);
		glm::vec4 GetFloat4(const std::string& name);

		void SetInt(const std::string& name, int val);
		int GetInt(const std::string& name);

		void SetMat4(const std::string& name, const glm::mat4& val);
		glm::mat4 GetMat4(const std::string& name);

		// warning!!!
		// can't appear the same name label in window, or they will change values together
		void RenderConfigGUI();

		Ref<Shader>									m_Shader;
	private:
		std::string									m_Name = "";
		// key: name, value: TextureMeta, Texture ptr
		std::unordered_map<std::string, TexPair>	m_Textures;

		// properties
		std::unordered_map<std::string, float>		m_FloatProps;
		std::unordered_map<std::string, int>		m_IntProps;
		std::unordered_map<std::string, glm::vec3>	m_Float3Props;
		std::unordered_map<std::string, glm::vec4>	m_Float4Props;
		std::unordered_map<std::string, glm::mat4>	m_Mat4Props;
	};
}