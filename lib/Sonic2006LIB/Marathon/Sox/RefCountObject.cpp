#include "RefCountObject.h"


using namespace Sonicteam::SoX;


void Sonicteam::SoX::RefCountObject::AddReference()
{
	++m_ReferenceCount;
}

void Sonicteam::SoX::RefCountObject::Release()
{
	if (--m_ReferenceCount == 0) {
		this->DestroyObject(1);
	}
}

uint32_t Sonicteam::SoX::RefCountObject::GetReferenceCount()
{
	return this->m_ReferenceCount;
}


