#pragma once

#include "Vortex/Events/Event.h"
#include <string>
#include <sstream>

namespace Vortex
{
	class GraphicsEvent : public Event
	{
		EVENT_CLASS_CATEGORY(EventCategoryGraphics)
	};

	class MSAAOpenedEvent : public GraphicsEvent
	{
	public:
		MSAAOpenedEvent(int nSamples)
			:m_NSamples(nSamples)
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MSAAOpenedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MSAAOpened)

		int m_NSamples;
	};
	
	class MSAAClosedEvent : public GraphicsEvent
	{
	public:
		MSAAClosedEvent()
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MSAAClosedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MSAAClosed)
	};
	
	class MSAANSamplesChangedEvent : public GraphicsEvent
	{
	public:
		MSAANSamplesChangedEvent(int nSamples)
			:m_NSamples(nSamples)
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MSAANSamplesChangedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MSAANSamplesChanged)

		int m_NSamples;
	};

	// HDROpened, HDRClosed, HDRExposureChanged
	class HDROpenedEvent : public GraphicsEvent
	{
	public:
		HDROpenedEvent()
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "HDROpenedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(HDROpened)
	};

	class HDRClosedEvent : public GraphicsEvent
	{
	public:
		HDRClosedEvent()
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "HDRClosedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(HDRClosed)
	};

	class HDRExposureChangedEvent : public GraphicsEvent
	{
	public:
		HDRExposureChangedEvent(float exposure)
			:m_Exposure(exposure)
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "HDRExposureChangedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(HDRExposureChanged)

		float m_Exposure;
	};
}