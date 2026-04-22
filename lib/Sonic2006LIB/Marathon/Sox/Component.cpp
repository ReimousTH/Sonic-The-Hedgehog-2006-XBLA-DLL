#include "Component.h"

using namespace Sonicteam::SoX;

Component::Component()
{

}
Component::Component(Component* component)
{
	m_pParent = component;
	m_lnComponent.InitializeLink(this);
	m_llComponents.InitializeList();

	if (component)
	{
		component->m_llComponents.InsertFast(&m_lnComponent);
	}
}

void __ComponentKernelDestroy(Component* pComponent)
{
	pComponent->DestroyObject(1);
}

Sonicteam::SoX::Component::~Component(void)
{
	this->m_llComponents.ForEach(__ComponentKernelDestroy);
	this->m_llComponents.ResetListFast();
	this->m_lnComponent.ResetQuick();
}

char* Sonicteam::SoX::Component::GetType()
{
	return "Object";
}

DESTRUCTION_CPP(Component);