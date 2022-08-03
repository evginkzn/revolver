#ifndef VENDOMAT_MODE_SERVICE_HPP
#define VENDOMAT_MODE_SERVICE_HPP

#include "Button2.h"

#include "vendomat_mode_base.hpp"

#include "service_door.hpp"

class VendomatModeService : public VendomatModeBase
{
public:
    enum Stage
    {
        StageStandBy = 0,
        StageSelectingTube,
    };

public:
    VendomatModeService(Revolver* revolver, Button2* left_button, Button2* right_button);
    VendomatModeService(){}
    ~VendomatModeService(){};

public:
    void init() override;
    void deinit() override;

    void tick() override;

private:
    void cell_selecting_done(uint8_t cell);

private:
    Stage stage_;
};

#endif // ! VENDOMAT_MODE_SERVICE_HPP