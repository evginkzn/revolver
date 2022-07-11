#include "vendomat_mode_service.hpp"

static VendomatModeService* instance = 0;

VendomatModeService::VendomatModeService(Revolver* revolver)
    : revolver_(revolver)
{
    ::instance = this;
}

void VendomatModeService::init()
{

}

void VendomatModeService::deinit()
{

}

void VendomatModeService::tick()
{

}