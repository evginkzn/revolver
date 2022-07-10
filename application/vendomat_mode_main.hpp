#ifndef VENDOMAT_MODE_MAIN_HPP
#define VENDOMAT_MODE_MAIN_HPP

#include "vendomat_mode_base.hpp"

class VendomatModeMain : public VendomatModeBase
{
public:
    VendomatModeMain();
    ~VendomatModeMain(){};

public:
    void init();
    void deinit();

    void tick() override;

};

#endif // ! VENDOMAT_MODE_MAIN_HPP