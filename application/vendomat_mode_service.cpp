#include "vendomat_mode_service.hpp"

static Revolver* revolver_;
static Button2* left_button_;
static Button2* right_button_;

static int step_count_ = 0;

static VendomatModeService* instance = 0;

static void service_left_button_click_handler(Button2& btn);
static void service_right_button_click_handler(Button2& btn);

uint8_t current_tube = 0;

VendomatModeService::VendomatModeService(Revolver* revolver, Button2* left_button, Button2* right_button)
{
    ::instance = this;
    revolver_ = revolver;
    left_button_ = left_button;
    right_button_ = right_button;
}

void VendomatModeService::init()
{
    stage_ = StageStandBy;
    left_button_->setClickHandler(service_left_button_click_handler);
    right_button_->setClickHandler(service_right_button_click_handler);
    left_button_->setLongClickHandler(service_left_button_click_handler);
    right_button_->setLongClickHandler(service_right_button_click_handler);

    MethodSlot<VendomatModeService, uint8_t> selected_event_slot(this, &VendomatModeService::cell_selecting_done);
    revolver_->attachOnSelectEvent(selected_event_slot);

    current_tube = revolver_->get_current_tube();
}

void VendomatModeService::deinit()
{
    MethodSlot<VendomatModeService, uint8_t> selected_event_slot(this, &VendomatModeService::cell_selecting_done);
    revolver_->detachOnSelectEvent(selected_event_slot);

    left_button_->setClickHandler(0);
    right_button_->setClickHandler(0);
    left_button_->setLongClickHandler(0);
    right_button_->setLongClickHandler(0);
}

void VendomatModeService::tick()
{
    switch (stage_)
    {
        case StageStandBy:
        {
            if (step_count_ != 0)
            {
                if (step_count_ < 0)
                {
                    ++step_count_;

                    if (current_tube == 11)
                    {
                        current_tube = 0;
                        revolver_->select_tube(current_tube);
                    }
                    else
                    {
                        current_tube += 1;
                        revolver_->select_tube(current_tube);
                    }
                }
                else
                {
                    --step_count_;

                    if (current_tube == 0)
                    {
                        current_tube = 11;
                        revolver_->select_tube(current_tube);
                    }
                    else
                    {
                        current_tube -= 1;
                        revolver_->select_tube(current_tube);
                    }
                }

                /*Serial.print("Current tube ");
                Serial.print(current_tube);*/
                stage_ = StageSelectingTube;
                break;
            }

            /*if (left_button_->isPressed())
            {
                if (current_tube == 0)
                {
                    current_tube = 11;
                    revolver_->select_tube(current_tube);
                }
                else
                {
                    current_tube -= 1;
                    revolver_->select_tube(current_tube);
                }
                stage_ = StageSelectingTube;
            }
            else if (right_button_->isPressed())
            {
                if (current_tube == 11)
                {
                    current_tube = 0;
                    revolver_->select_tube(current_tube);
                }
                else
                {
                    current_tube += 1;
                    revolver_->select_tube(current_tube);
                }
                stage_ = StageSelectingTube;
            }*/
        }
        break;

        case StageSelectingTube:
        {
            
        }
        break;
    
        default: break;
    }
}

void VendomatModeService::cell_selecting_done(uint8_t cell)
{
    stage_ = StageStandBy;
}

static void service_left_button_click_handler(Button2& btn)
{
    --step_count_;

                /*Serial.print("Step count ");
            Serial.print(step_count_);
            Serial.println("");*/
}

static void service_right_button_click_handler(Button2& btn)
{
    ++step_count_;

                /*Serial.print("Step count ");
            Serial.print(step_count_);
            Serial.println("");*/
}