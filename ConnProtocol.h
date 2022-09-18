#ifndef CONNPROTOCOL_H
#define CONNPROTOCOL_H

#pragma once
namespace ConnProtocol
{
    class ConnProtocol
    {
    public:
        virtual bool canSend() = 0;
        virtual bool canReceive() = 0;

        ConnProtocol();
        ~ConnProtocol();
    };

    class _tcpClass : public ConnProtocol
    {
    public:
        _tcpClass() {}
        ~_tcpClass() {}
        bool canSend() override
        {
            return true;
        }
        bool canReceive() override
        {
            return true;
        }
    };
}
#endif
