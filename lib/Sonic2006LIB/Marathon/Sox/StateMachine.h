#pragma once

#include <Marathon.inl>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <Sox/AI/State.h>
#include <deque>

namespace Sonicteam
{
	namespace SoX
	{
		namespace AI
		{
			
			template<typename T>
			class StateMashine
			{
			public:
				StateMashine(void) {};
				MARATHON_DESTRUCTION_H(StateMashine);
				virtual void OnStateChange(boost::shared_ptr<Sonicteam::SoX::AI::State<T>>& NextState,T* context); // For now nothing
				virtual void OnStateUnk(boost::shared_ptr<Sonicteam::SoX::AI::State<T>>& NextState,T* context); // For now nothing
				virtual unsigned int Unk03(T* context) { return 0;}; // For now nothing
				virtual unsigned int Unk04() { return 0;}; // OnMessageRecieved, -_- argument is message, also bool return type (:8200BAC8 here for info)
				virtual unsigned int Unk05() { return 0;}; // For now nothing
				virtual unsigned int Unk06() { return 0;}; // For now nothing

			public:
				boost::shared_ptr<Sonicteam::SoX::AI::State<T>> CurrentState; //0x4
				std::deque<boost::shared_ptr<Sonicteam::SoX::AI::State<T>>> StateDQueue; //0xC or queue cant figure 

			};

			template<typename T>
			void Sonicteam::SoX::AI::StateMashine<T>::OnStateUnk(boost::shared_ptr<Sonicteam::SoX::AI::State<T>>& NextState,T* context)
			{

			}

			template<typename T>
			void Sonicteam::SoX::AI::StateMashine<T>::OnStateChange(boost::shared_ptr<Sonicteam::SoX::AI::State<T>>& NextState,T* context) //IContext
			{
				boost::shared_ptr<Sonicteam::SoX::AI::State<T>> _next = NextState;
				if (NextState){
					if (this->CurrentState) this->CurrentState->OnStateEnd();
					_next->OnStateStart(0);
				}
				this->CurrentState = _next;
			}
		}
	
	}
}
