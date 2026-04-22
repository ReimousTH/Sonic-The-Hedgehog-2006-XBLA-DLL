#include "UpdateResourceInFrustumThread.h"

using namespace Sonicteam;

Sonicteam::UpdateResourceInFrustumThread::UpdateResourceInFrustumThread(uint32_t Processor) : Thread("UpdateResourceInFrustumThread", Processor, 1)
{
}

Sonicteam::UpdateResourceInFrustumThread::~UpdateResourceInFrustumThread(void)
{
}

void Sonicteam::UpdateResourceInFrustumThread::OnStep(double)
{
    for (std::vector<boost::shared_ptr<SoX::Scenery::World>>::iterator it = m_vspWorlds.begin(); it != m_vspWorlds.end(); it++)
    {
        it->get()->WorldProcessUnk13();
    }
}
