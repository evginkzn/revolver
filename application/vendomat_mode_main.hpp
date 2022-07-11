#ifndef VENDOMAT_MODE_MAIN_HPP
#define VENDOMAT_MODE_MAIN_HPP

#include "vendomat_mode_base.hpp"

class VendomatModeMain : public VendomatModeBase
{
public:
  enum Stage
  {
    StageStandBy = 0,
    StageSelectingTube,
    StageWaitingOwenOpening,
    StageOpeningCap,
    StagePushing,
    StageWaitingEndOperation,
    StageClosingCap,
    StageInactive,

    StageRevolverCalibrating,
  };

public:
    VendomatModeMain(Revolver* revolver, Pusher* pusher, Cap* cap);
    VendomatModeMain(){}
    ~VendomatModeMain(){};

public:
    void init() override;
    void deinit() override;

    void tick() override;

    Stage stage() const { return stage_; }

    void select_cell(uint8_t cell);


private:
  void cell_selecting_done(uint8_t cell);
  void cap_opening_done(bool done);
  void cap_closing_done(bool done);
  void pushing_done(bool done);

private:
    Revolver* revolver_;
    Pusher* pusher_;
    Cap* cap_;

    Stage stage_;

    unsigned long stage_delay_counter_;
};

#endif // ! VENDOMAT_MODE_MAIN_HPP