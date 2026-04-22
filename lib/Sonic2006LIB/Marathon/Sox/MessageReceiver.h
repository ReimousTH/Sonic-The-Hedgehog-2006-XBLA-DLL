#ifndef SONICTEAM__SOX__MESSAGE_RECEIVER
#define SONICTEAM__SOX__MESSAGE_RECEIVER
#include <Marathon.inl>
#include <Marathon.inl>
#include <Sox/Message.h>

#define SoXMessageType void*


namespace Sonicteam{
	namespace SoX{


		
		template <unsigned int T>
		struct MessageTemplate {
			char MessageData[T];
			MessageTemplate(){};
			MessageTemplate(unsigned int value){this->SetValueAt(0,0x1300C);};

			template <typename Y>
			inline void SetValueAt(unsigned int pos, const Y& Value) {
				*reinterpret_cast<Y*>(&MessageData[pos]) = Value; // 
			}
			template <typename Y>
			inline Y GetValueAt(unsigned int pos) const {
				return *(Y*)(&MessageData[pos]); //
			}
			template <typename Y>
			inline Y* GetValueAtRef(unsigned int pos) const {
				return (Y*)(&MessageData[pos]); //
			}

		};


		class MessageReceiver
		{
		public:
			MessageReceiver(void);
			~MessageReceiver(void);

			MARATHON_DESTRUCTION_H(MessageReceiver);
			virtual int OnMessageRecieved(SoXMessageType) = 0;
		};
	};
};

#endif
