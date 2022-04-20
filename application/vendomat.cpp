#include "vendomat.hpp"

#include "Callback.h"

#define DEBUG

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

Vendomat::Vendomat(Revolver& revolver, Pusher& pusher, Cap& cap)
    : revolver_(revolver)
    , pusher_(pusher)
    , cap_(cap)
{

}

void Vendomat::init()
{
    MethodSlot<Vendomat, uint8_t> selected_event_slot(this, &Vendomat::cell_selecting_done);
    revolver_.attachOnSelectEvent(selected_event_slot);

    MethodSlot<Vendomat, bool> opened_event_slot(this, &Vendomat::cap_opening_done);
    cap_.attachOnOpenedEvent(opened_event_slot);

    MethodSlot<Vendomat, bool> closed_event_slot(this, &Vendomat::cap_closing_done);
    cap_.attachOnClosedEvent(closed_event_slot);

    MethodSlot<Vendomat, bool> pushed_event_slot(this, &Vendomat::pushing_done);
    pusher_.attachOnPushedEvent(pushed_event_slot);

    stage_ = StageStandBy;
}

void Vendomat::tick()
{
    switch (stage_)
    {
        case StageStandBy:
        {
            
        }
        break;

        case StageSelectingTube:
        {
            
        }
        break;

        case StageWaitingOwenOpening:
        {
            if (millis() - stage_delay_counter_ > 500)
            {
                stage_delay_counter_ = millis();

                stage_ = StageOpeningCap;
            }
        }
        break;

        case StageOpeningCap:
        {
            if (millis() - stage_delay_counter_ > 500 && ! cap_.in_action())
            {
                cap_.open();
            }
        }
        break;

        case StagePushing:
        {
            if (millis() - stage_delay_counter_ > 500 && ! pusher_.in_action())
            {
                pusher_.make_push();
            }
        }
        break;

        case StageWaitingEndOperation:
        {
            if (millis() - stage_delay_counter_ > 500 && ! pusher_.in_action())
            {
                stage_ = StageClosingCap;
            }
        }
        break;

        case StageClosingCap:
        {
            if (millis() - stage_delay_counter_ > 500 && ! pusher_.in_action())
            {
                cap_.close();
            }
        }
        break;
    
        default: break;
    }
}

void Vendomat::select_cell(uint8_t cell)
{
    if (stage_ != StageStandBy)
    {
        #ifdef DEBUG
        Serial.println("Vendomat is busy");
        Serial.println("Current stage: ");
        Serial.print(stages_strings[stage_]);
        Serial.println("");
        #endif // ! DEBUG

        return;
    }

    if (revolver_.state() != Revolver::StateIdle)
    {
        #ifdef DEBUG
        Serial.println("Error revolver not ready!");
        Serial.println("");
        #endif // ! DEBUG
        return;
    }

    if (revolver_.select_tube(cell))
    {
        stage_ = StageSelectingTube;
    }
}

void Vendomat::cell_selecting_done(uint8_t cell)
{
    #ifdef DEBUG
    Serial.print("Tube ");
    Serial.print(cell);
    Serial.println(" selected");
    #endif // ! DEBUG

    stage_delay_counter_ = millis();
    stage_ = StageWaitingOwenOpening;
}

void Vendomat::cap_opening_done(bool done)
{
    #ifdef DEBUG
    Serial.println("Cap opened");
    #endif // ! DEBUG

    stage_delay_counter_ = millis();
    stage_ = StagePushing;
}

void Vendomat::cap_closing_done(bool done)
{
    #ifdef DEBUG
    Serial.println("Cap closed");
    #endif // ! DEBUG

    stage_ = StageStandBy;
}

void Vendomat::pushing_done(bool done)
{
    #ifdef DEBUG
    Serial.println("Pushing closed");
    #endif // ! DEBUG

    stage_delay_counter_ = millis();
    stage_ = StageWaitingEndOperation;
}