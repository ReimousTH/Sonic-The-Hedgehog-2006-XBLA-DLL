#pragma once

#include <Marathon.inl>
#include <Sox/IResource2.h>

namespace Sonicteam
{
	class TextBookMgr;

	class TextBook : public SoX::IResource2<TextBook, TextBookMgr>
	{
	public:
		TextBook(void);
		~TextBook(void);

		MARATHON_DESTRUCTION_H(TextBook); //0x0 + 0x10
	};
};
