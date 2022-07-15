#include "vtpch.h"
#include "Material.h"
#include "glad/glad.h"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

namespace Vortex
{
	Material::Material()
	{

	}
	Material::Material(const std::string& name)
		:m_Name(name)
	{

	}
	void Material::AddTexture(const std::string& name, const Ref<Texture>& tex)
	{
		m_Textures[name] = TexPair{ TextureMeta{false}, tex };
	}
	void Material::SetTexture(const std::string& name, const Ref<Texture>& tex)
	{
		m_Textures[name] = TexPair{ TextureMeta{false}, tex };
	}
	Texture* Material::GetTexture(const std::string& name)
	{
		if (m_Textures.count(name) == 0)return nullptr;
		return m_Textures[name].second.get();
	}
	void Material::BindTextures()
	{
		int slotIdx = 0;
		for (auto& p : m_Textures)
		{
			Texture* tex = p.second.second.get();
			tex->Bind(slotIdx);
			m_Shader->SetInt(p.first, slotIdx);
			slotIdx++;
		}
	}
	void Material::SetFloat(const std::string& name, float val)
	{
		m_FloatProps[name] = val;
		m_Shader->SetFloat(name, val);
	}
	float Material::GetFloat(const std::string& name)
	{
		if (m_FloatProps.count(name) == 0)
		{
			VT_CORE_WARN("Can't find property {}", name);
			return 0.0f;
		}
		return m_FloatProps[name];
	}

	void Material::SetFloat3(const std::string& name, const glm::vec3& val)
	{
		m_Float3Props[name] = val;
		m_Shader->SetFloat3(name, val);
	}

	glm::vec3 Material::GetFloat3(const std::string& name)
	{
		if (m_Float3Props.count(name) == 0)
		{
			VT_CORE_WARN("Can't find property {}", name);
			return glm::vec3(0);
		}
		return m_Float3Props[name];
	}

	void Material::SetFloat4(const std::string& name, const glm::vec4& val)
	{
		m_Float4Props[name] = val;
		m_Shader->SetFloat4(name, val);
	}

	glm::vec4 Material::GetFloat4(const std::string& name)
	{
		if (m_Float4Props.count(name) == 0)
		{
			VT_CORE_WARN("Can't find property {}", name);
			return glm::vec4(0);
		}
		return m_Float4Props[name];
	}

	void Material::SetInt(const std::string& name, int val)
	{
		m_IntProps[name] = val;
		m_Shader->SetInt(name, val);
	}

	int Material::GetInt(const std::string& name)
	{
		if (m_IntProps.count(name) == 0)
		{
			VT_CORE_WARN("Can't find property {}", name);
			return 0;
		}
		return m_IntProps[name];
	}

	void Material::SetMat4(const std::string& name, const glm::mat4& val)
	{
		m_Mat4Props[name] = val;
		m_Shader->SetMat4(name, val);
	}

	glm::mat4 Material::GetMat4(const std::string& name)
	{
		if (m_Mat4Props.count(name) == 0)
		{
			VT_CORE_WARN("Can't find property {}", name);
			return glm::mat4(0);
		}
		return m_Mat4Props[name];
	}

	void Material::RenderConfigGUI()
	{
		ImGui::Text("Material Name: %s", m_Name.c_str());
		// display properties
		// float, float3, float4, int, texture
		float v_speed = 0.05f;
		m_Shader->Bind();
		for (auto& pf : m_FloatProps)
		{
			float v = GetFloat(pf.first);
			ImGui::DragFloat(pf.first.c_str(), &v, v_speed);
			SetFloat(pf.first, v);
		}
		for (auto& pf3 : m_Float3Props)
		{
			glm::vec3 v = GetFloat3(pf3.first);
			ImGui::DragFloat3(pf3.first.c_str(), glm::value_ptr(v), v_speed);
			SetFloat3(pf3.first, v);
		}
		for (auto& pf4 : m_Float4Props)
		{
			glm::vec4 v = GetFloat4(pf4.first);
			ImGui::DragFloat4(pf4.first.c_str(), glm::value_ptr(v), v_speed);
			SetFloat4(pf4.first, v);
		}
		for (auto& pi : m_IntProps)
		{
			int v = GetInt(pi.first);
			ImGui::DragInt(pi.first.c_str(), &v);
			SetInt(pi.first, v);
		}
		for (auto& tex : m_Textures)
		{
			ImGui::Text("Texture: %s", tex.first.c_str());
		}
		m_Shader->Unbind();
	}
}