#include "vendomat_mode_main.hpp"

static VendomatModeMain* instance = 0;

VendomatModeMain::VendomatModeMain(Revolver* revolver, Pusher* pusher, Cap* cap)
    : revolver_(revolver)
    , pusher_(pusher)
    , cap_(cap)
{
    ::instance = this;
}

void VendomatModeMain::init()
{
    MethodSlot<VendomatModeMain, uint8_t> selected_event_slot(this, &VendomatModeMain::cell_selecting_done);
    revolver_->attachOnSelectEvent(selected_event_slot);

    MethodSlot<VendomatModeMain, bool> opened_event_slot(this, &VendomatModeMain::cap_opening_done);
    cap_->attachOnOpenedEvent(opened_event_slot);

    MethodSlot<VendomatModeMain, bool> closed_event_slot(this, &VendomatModeMain::cap_closing_done);
    cap_->attachOnClosedEvent(closed_event_slot);

    MethodSlot<VendomatModeMain, bool> pushed_event_slot(this, &VendomatModeMain::pushing_done);
    pusher_->attachOnPushedEvent(pushed_event_slot);

    stage_ = StageStandBy;
}

void VendomatModeMain::deinit()
{
    MethodSlot<VendomatModeMain, uint8_t> selected_event_slot(this, &VendomatModeMain::cell_selecting_done);
    revolver_->detachOnSelectEvent(selected_event_slot);

    MethodSlot<VendomatModeMain, bool> opened_event_slot(this, &VendomatModeMain::cap_opening_done);
    cap_->detachOnOpenedEvent(opened_event_slot);

    MethodSlot<VendomatModeMain, bool> closed_event_slot(this, &VendomatModeMain::cap_closing_done);
    cap_->detachOnClosedEvent(closed_event_slot);

    MethodSlot<VendomatModeMain, bool> pushed_event_slot(this, &VendomatModeMain::pushing_done);
    pusher_->detachOnPushedEvent(pushed_event_slot);

    stage_ = StageInactive;
}

void VendomatModeMain::tick()
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
            if (millis() - stage_delay_counter_ > 500 && ! cap_->in_action())
            {
                cap_->open();
            }
        }
        break;

        case StagePushing:
        {
            if (millis() - stage_delay_counter_ > 500 && ! pusher_->in_action())
            {
                pusher_->make_push();
            }
        }
        break;

        case StageWaitingEndOperation:
        {
            if (millis() - stage_delay_counter_ > 500 && ! pusher_->in_action())
            {
                stage_ = StageClosingCap;
            }
        }
        break;

        case StageClosingCap:
        {
            if (millis() - stage_delay_counter_ > 500 && ! pusher_->in_action())
            {
                cap_->close();
            }
        }
        break;
    
        default: break;
    }
}

void VendomatModeMain::select_cell(uint8_t cell)
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

    if (revolver_->state() != Revolver::StateIdle)
    {
        #ifdef DEBUG
        Serial.println("Error revolver not ready!");
        Serial.println("");
        #endif // ! DEBUG
        return;
    }

    if (revolver_->select_tube(cell))
    {
        stage_ = StageSelectingTube;
    }
}

void VendomatModeMain::cell_selecting_done(uint8_t cell)
{
    #ifdef DEBUG
    Serial.print("Tube ");
    Serial.print(cell);
    Serial.println(" selected");
    #endif // ! DEBUG

    stage_delay_counter_ = millis();
    stage_ = StageWaitingOwenOpening;
}

void VendomatModeMain::cap_opening_done(bool done)
{
    #ifdef DEBUG
    Serial.println("Cap opened");
    #endif // ! DEBUG

    stage_delay_counter_ = millis();
    stage_ = StagePushing;
}

void VendomatModeMain::cap_closing_done(bool done)
{
    #ifdef DEBUG
    Serial.println("Cap closed");
    #endif // ! DEBUG

    stage_ = StageStandBy;
}

void VendomatModeMain::pushing_done(bool done)
{
    #ifdef DEBUG
    Serial.println("Pushing closed");
    #endif // ! DEBUG

    stage_delay_counter_ = millis();
    stage_ = StageWaitingEndOperation;
}