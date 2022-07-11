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

#include "vendomat_mode_base.hpp"
#include "vendomat_mode_main.hpp"
#include "vendomat_mode_service.hpp"

class Vendomat
{
public:
  enum Mode
  {
    ModeMain = 0,
    ModeService,
    
    ModesCount
  };

public:
  Vendomat(Revolver* revolver, Pusher* pusher, Cap* cap);
  ~Vendomat(){}

public:
  void init();
  void tick();

  bool select_cell(uint8_t cell);

  Mode mode() const { return mode_; }

  bool set_mode(Mode mode);

  uint8_t stage() const;

private:
  Mode mode_;
};

#endif // ! VENDOMAT_HPP