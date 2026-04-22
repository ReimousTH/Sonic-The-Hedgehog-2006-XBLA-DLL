#pragma once

#include <Marathon.inl>

namespace Sonicteam
{
    namespace SoX
    {
        struct IMessage
        {
            uint32_t ID;

            IMessage() {}
            IMessage(const uint32_t id) : ID(id) {}
        };

        template <const uint32_t id>
        struct Message : IMessage
        {
        public:
            Message()
            {
                ID = id;
            }

            static const uint32_t GetID()
            {
                return id;
            }

            static Message<id>* AsMessage(IMessage* msg)
            {
                if (msg && msg->ID == GetID()) {
                    return (Message<id>*)(msg);
                }
                return nullptr;
            }
        };
    }
}