#ifndef MODBUS_RECEIVER_HPP
#define MODBUS_RECEIVER_HPP

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include "ModbusRTUSlave.h"
#include <Callback.h>

class ModbusReceiver
{
private:
    enum
    {
        BufferSize = 256,
        BaudRate = 9600,
        DefaultAddress = 2
    };

public:
    enum AddressMap
    {
        State = 0,
        Error,
        TubeNumber,
        Command,

        Length
    };

public:
    ModbusReceiver();
    ~ModbusReceiver(){}

public:
    void init();

    void poll();

    void updateErrorHandler(uint16_t error_code);
    void updateStateHandler(uint16_t state);

    void attachOnCommandReceivedEvent(const Slot<uint16_t>& slot)
    {
        onCommandReceived_.attach(slot);
    }

    uint16_t selectedTube();

public:
    Signal<uint16_t> onCommandReceived_;

private:
    uint8_t buffer_[BufferSize];

    ModbusRTUSlave modbus_slave_instance_;

};

#endif // ! MODBUS_RECEIVER_HPP