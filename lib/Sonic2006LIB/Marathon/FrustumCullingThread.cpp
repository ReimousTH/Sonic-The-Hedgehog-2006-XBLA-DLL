#include "FrustumCullingThread.h"

using namespace Sonicteam;

Sonicteam::FrustumCullingThread::FrustumCullingThread(uint32_t Processor) : Thread("FrustumCullingThread", Processor, 1)
{
}

Sonicteam::FrustumCullingThread::~FrustumCullingThread(void)
{
}

void Sonicteam::FrustumCullingThread::OnStep(double)
{
    for (std::vector<boost::shared_ptr<SoX::Scenery::World>>::iterator it = m_vspWorlds.begin(); it != m_vspWorlds.end(); it++)
    {
        it->get()->WorldProcessUnk13();
    }

    // i'll think that its reset at least
    for (std::vector<boost::shared_ptr<SoX::Scenery::World>>::iterator it = m_vspWorlds.begin(); it != m_vspWorlds.end(); it++)
    {
        it->reset();
    }
}

uint32_t Sonicteam::FrustumCullingThread::ms_FrustumCullingThreadProcessor[] =
{
    0,1,2,3,5,5
};