#pragma once

#include <vector>
#include <string>
#include "Vortex/Core/Core.h"
#include "Component.h"

namespace Vortex
{
    class Scene;

	class Entity
	{
	public:
		Entity()
            :m_Name("Entity")
		{

		}

        Entity(const std::string& name)
            :m_Name(name)
        {

        }

        template<typename T>
        T* GetComponent()
        {
            for (Ref<Component>& c : m_Components)
            {
                if (typeid(*c) == typeid(T))
                {
                    return (T*)c.get();
                }
            }
            return nullptr;
        }

        template<typename T>
        T* AddComponent()
        {
            if (GetComponent<T>() != nullptr)
                return nullptr;

            Ref<Component> c = CreateRef<T>();
            c->SetEntity(this);
            c->Init();

            m_Components.emplace_back(c);
            return (T*)c.get();
        }

        void Init()
        {

        }

        void OnUpdate(Timestep ts)
        {
            for (auto& c : m_Components)
            {
                c->OnUpdate(ts);
            }
        }

        void AfterUpdate(Timestep ts)
        {
            for (auto& c : m_Components)
            {
                c->AfterUpdate(ts);
            }
        }

        std::string& GetName() { return m_Name; }

        void SetScene(Scene* scene)
        {
            m_Scene = scene;
        }

        Scene* GetScene() { return m_Scene; }
	protected:
        const static uint64_t AVAIL_GUID = 256;
        static uint64_t GetGUID()
        {
            static uint64_t guid = AVAIL_GUID;
            return guid++;
        }

        const uint64_t                  m_GUID = GetGUID();
        std::string                     m_Name;
		std::vector< Ref<Component> >	m_Components;
        Scene*                          m_Scene;
	};
}