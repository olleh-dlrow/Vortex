#pragma once

#include "Vortex/Core/Timestep.h"

namespace Vortex
{
	class Entity;

	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;

		virtual void Init() {};
		virtual void OnUpdate(Timestep ts) {}
		virtual void PostUpdate(Timestep ts) {}

		void SetEntity(Entity* e) 
		{
			m_Entity = e;
		}

		Entity* GetEntity() { return m_Entity; }
	protected:
		const static uint64_t AVAIL_GUID = 256;
		static uint64_t GetGUID()
		{
			static uint64_t guid = AVAIL_GUID;
			return guid++;
		}

		const uint64_t                m_GUID = GetGUID();
		Entity*						  m_Entity;
	};

	class DerivedComponent: public Component
	{
	public:
		virtual void Init() override;
		virtual void OnUpdate(Timestep ts) override;
	};
}
