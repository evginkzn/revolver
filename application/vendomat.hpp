#ifndef VENDOMAT_HPP
#define VENDOMAT_HPP

#if ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include "revolver.hpp"
#include "pusher.hpp"
#include "cap.hpp"

class Vendomat
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

    StageRevolverCalibrating,
  };

  enum Mode
  {
    ModeMain = 0,
    ModeService,
    
    ModesCount
  };

public:
  Vendomat(Revolver& revolver, Pusher& pusher, Cap& cap);
  ~Vendomat(){}

public:
  void init();
  void tick();

  void select_cell(uint8_t cell);

  Stage stage() const { return stage_; }
  Mode mode() const { return mode_; }

  bool set_mode(Mode mode);

private:
  void cell_selecting_done(uint8_t cell);
  void cap_opening_done(bool done);
  void cap_closing_done(bool done);
  void pushing_done(bool done);

private:
    Revolver& revolver_;
    Pusher& pusher_;
    Cap& cap_;

    Stage stage_;

    unsigned long stage_delay_counter_;

    Mode mode_;
};

#endif // ! VENDOMAT_HPP