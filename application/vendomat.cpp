#include "vendomat.hpp"

#include "Callback.h"
//#define DEBUG

#ifdef DEBUG
static const char stages_strings[7][22] = 
                                    {
                                        "Stand By             ",
                                        "Selecting Tube       ",
                                        "Waiting Owen Opening ",
                                        "Opening Cap          ",
                                        "Pushing              ",
                                        "Waiting End Operation",
                                        "Closing Cap          "
                                    };
#endif

static VendomatModeMain mode_main_;
static VendomatModeService mode_service_;

static VendomatModeBase* mode_instance_[Vendomat::ModesCount];

Vendomat::Vendomat(Revolver* revolver, Pusher* pusher, Cap* cap)
    : mode_(ModeMain)
{
    mode_main_ = VendomatModeMain(revolver, pusher, cap);
    mode_instance_[ModeMain] = &mode_main_;
    mode_instance_[ModeService] = &mode_service_;
}

void Vendomat::init()
{
    mode_instance_[mode_]->init();
}

void Vendomat::tick()
{
    mode_instance_[mode_]->tick();
}

bool Vendomat::select_cell(uint8_t cell)
{
    if (mode_ != ModeMain)
    {
        return false;
    }

    mode_main_.select_cell(cell);
    return true;
}

bool Vendomat::set_mode(Mode mode)
{
    /*if (stage_ != StageStandBy)
    {
        return false;
    }*/

    if (mode_ == mode)
    {
        return true;
    }

    mode_instance_[mode_]->deinit();
    mode_ = mode;
    mode_instance_[mode_]->init();
}

uint8_t Vendomat::stage() const
{
    if (mode_ == ModeMain)
    {
        return mode_main_.stage();
    }
    else
    {
        return VendomatModeMain::StageInactive;
    }
}