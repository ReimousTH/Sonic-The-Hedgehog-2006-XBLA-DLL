#pragma once

#include <Player/State/Object2.h>
#include "IMachine.h"
#include "IContext.h"
#include "CommonContext.h"
#include "../../Sox/StateMachine.h"
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <functional>
#include <Marathon.inl>
#include <deque>

namespace Sonicteam
{
	namespace Player
	{
		namespace State
		{

			typedef boost::shared_ptr<Sonicteam::Player::State::Object2>* (*ConstructFunction)(Sonicteam::Player::State::IMachine*);
	
			//8200BAD8
			class Machine2:public SoX::AI::StateMashine<IContext>,IMachine
			{
			public:
				Machine2(void);
				~Machine2(void);


				MARATHON_DESTRUCTION_H(Machine2);
				virtual void Update(float) override;
				virtual boost::shared_ptr<IContext> GetStateContext() override;
				virtual unsigned int ChangeState(int) override;
				virtual unsigned int ChangeStateQueue(int) override;
				virtual void EnableQueueState() override;
				virtual void EnableQueueStatePop() override;
				virtual unsigned int ChangeStatePop(int) override;
				virtual unsigned int GetStateID() override;

				std::vector<std::pair<uint32_t, ConstructFunction>> m_vInitialStates; //0x40
				unsigned int m_PreState; // -1 (at constuctor) //0x50
				unsigned int m_PostState; //0x54
						 byte m_IsNewStateQueue; //0x58
						 byte m_DoPendingState; //0x59
						 byte m_DoPendingStatePop; //0x5A
						 byte m_Flag4; //0x5B
				std::deque<uint32_t> m_dPendingStates; //0x5C //std::queue?
				boost::shared_ptr<IContext> m_spContext; //0x70

			};
		}
	}
}

