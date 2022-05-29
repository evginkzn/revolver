#include "modbus_receiver.hpp"

static int32_t hold_reg_read_(uint16_t address);
static bool hold_reg_write_(uint16_t address, uint16_t value);

static uint16_t address = 0;

static uint16_t map_[ModbusReceiver::AddressMap::Length];

static ModbusReceiver* instance_ = 0;

ModbusReceiver::ModbusReceiver()
    : modbus_slave_instance_(Serial, buffer_, BufferSize)
{
    ::instance_ = this;
    address = DefaultAddress;
}

void ModbusReceiver::init()
{
    Serial.begin(BaudRate);
    modbus_slave_instance_.begin(address, BaudRate);

    modbus_slave_instance_.configureHoldingRegisters(AddressMap::Length, hold_reg_read_, hold_reg_write_);
}

void ModbusReceiver::poll()
{
    modbus_slave_instance_.poll();
}

void ModbusReceiver::updateErrorHandler(uint16_t error_code)
{
    map_[Error] = error_code;
}

void ModbusReceiver::updateStateHandler(uint16_t state)
{
    map_[State] = state;
}

uint16_t ModbusReceiver::selectedTube()
{
    return map_[TubeNumber];
}

static int32_t hold_reg_read_(uint16_t address)
{
    if (address < ModbusReceiver::AddressMap::Length)
    {
        ++map_[address];
        
        return map_[address];
    }
    else return 0;
}

static bool hold_reg_write_(uint16_t address, uint16_t value)
{
    switch (address)
    {
        case ModbusReceiver::AddressMap::State:
        break;

        case ModbusReceiver::AddressMap::Error:
            if (value == 0)
            {
                map_[ModbusReceiver::AddressMap::TubeNumber] = 0;
            }
        break;

        case ModbusReceiver::AddressMap::TubeNumber:
            map_[ModbusReceiver::AddressMap::TubeNumber] = value;
        break;

        case ModbusReceiver::AddressMap::Command:
            ::instance_->onCommandReceived_.fire(value);
        break;
        
        default:
        break;
    }
    return true;
}