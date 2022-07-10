#ifndef VENDOMAT_MODE_SERVICE_HPP
#define VENDOMAT_MODE_SERVICE_HPP

#include "vendomat_mode_base.hpp"

class VendomatModeService : public VendomatModeBase
{
public:
    VendomatModeService();
    ~VendomatModeService(){};

public:
    void init();
    void deinit();

    void tick();

};

#endif // ! VENDOMAT_MODE_SERVICE_HPP