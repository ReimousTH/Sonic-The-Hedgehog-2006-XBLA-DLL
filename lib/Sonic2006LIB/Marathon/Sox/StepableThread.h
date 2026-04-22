#pragma once


#include <Marathon.inl>
#include <Sox/Thread.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>




namespace Sonicteam{
	namespace SoX{

	    ////////////////////////EXAMPLE///////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		/* 
		boost::function<int(int, int)> fnc1 = boost::bind(&MyClass::multiply, &obj, _1, _2); Hoow To use For Example :) (where _a1,_a2 custom args) , yes place inside class(this yea too)

		*/
		///////////////////////////////////////////////////////////////////////////


		
		class StepableThread:public Thread {
        public:

			boost::function<int(double)> func1;
			boost::function<void*(void)> func2;          

			unsigned int StepableThreadFlag;
        
			

        public:
			StepableThread(const char* ThreadName,boost::function<void*(double)>& fnc1,boost::function<void*(void)>& fnc2,int Priority);

			MARATHON_DESTRUCTION_H(StepableThread);
  
		};
	};
};