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
		MSAAOpenedEvent()
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MSAAOpenedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MSAAOpened)
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
		MSAANSamplesChangedEvent()
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MSAANSamplesChangedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MSAANSamplesChanged)
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
		HDRExposureChangedEvent()
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "HDRExposureChangedEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(HDRExposureChanged)
	};
}