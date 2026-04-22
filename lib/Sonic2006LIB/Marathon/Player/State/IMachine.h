#pragma once


#include "..\IPlugIn.h"
#include "IContext.h"
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>


namespace Sonicteam
{
	namespace Player
	{
		namespace State
		{

			//8200BAD8
			class IMachine:IPlugIn
			{
			public:
				IMachine(void);
				~IMachine(void);


				MARATHON_DESTRUCTION_H(IMachine);;
				virtual void Update(float) = 0; //delta (origin = 8221EBE8)
				virtual boost::shared_ptr<IContext> GetStateContext() = 0;
				virtual unsigned int ChangeState(int) = 0; // StateI
				virtual unsigned int ChangeStateQueue(int) = 0; // StateI
				virtual void EnableQueueState() = 0; //Not Sure about names
				virtual void EnableQueueStatePop() = 0; //Not Sure about names
				virtual unsigned int ChangeStatePop(int) = 0; // With All Flags
				virtual unsigned int GetStateID() = 0;

			};

		}

	}

}

