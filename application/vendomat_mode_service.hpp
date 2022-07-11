#ifndef VENDOMAT_MODE_SERVICE_HPP
#define VENDOMAT_MODE_SERVICE_HPP

#include "vendomat_mode_base.hpp"

class VendomatModeService : public VendomatModeBase
{
public:
    VendomatModeService(Revolver* revolver);
    VendomatModeService(){}
    ~VendomatModeService(){};

public:
    void init() override;
    void deinit() override;

    void tick() override;

private:
    Revolver* revolver_;
};

#endif // ! VENDOMAT_MODE_SERVICE_HPP