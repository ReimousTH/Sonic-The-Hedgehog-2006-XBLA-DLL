#pragma once

#include <Sox/IResource.h>

namespace Sonicteam
{
	namespace SplinePath
	{

		class Scene : public Sonicteam::SoX::IResource
		{

		public:
			Scene();
			~Scene();

			MARATHON_DESTRUCTION_H(Scene);
		};

	}
}