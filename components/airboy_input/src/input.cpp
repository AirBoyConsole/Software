#include "input.hpp"

namespace airboy 
{
    TaskHandle_t Input::input_handle;

    Input::Input()
    {
        this->input_semaphore = xSemaphoreCreateMutex();
        assert(this->input_semaphore);
    }

    gpio_num_t Input::get_interrupt_pin()
    {
        return this->interrupt;
    }

    void Input::block_input()
    {
        xSemaphoreTake(this->input_semaphore, portMAX_DELAY);
    }

    void Input::unblock_input()
    {
        xSemaphoreGive(this->input_semaphore);
    }

    void Input::input_task(void *arg)
    {
        uint32_t natified_value;
        Input *input = reinterpret_cast<Input*>(arg);

        //check if input was passed corectly
        if (input != nullptr)
        {
            //task main loop
            while(true)
            {
                //wait for notification
                natified_value = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
                
                //if notified read input
                if (natified_value > 0)
                {
                    input->block_input();
                        do{
                            input->read_input();
                        }while(!gpio_get_level(input->get_interrupt_pin()));
                    input->unblock_input();
                }    
                else
                {
                    ESP_LOGI(INPUT_TAG, "notified value 0");
                }                  
            }
        }

        ESP_LOGE(INPUT_TAG, "Pointer error!");
        ESP_LOGE(INPUT_TAG, "task is deleted");
        vTaskDelete(NULL);
    }

    void IRAM_ATTR Input::input_interrupt(void* args)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(input_handle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    void Input::read_input()
    {
        //read input register value 
        uint8_t button_state[2];
        ESP_ERROR_CHECK(register_read(this->chip_0_address, 0x09, &button_state[0]));
        ESP_ERROR_CHECK(register_read(this->chip_1_address, 0x09, &button_state[1]));

        //this->prev_buttons_value = this->buttons_value;
        this->buttons_value = button_state[0];
        this->buttons_value += button_state[1] << 8;
    }

    uint16_t Input::get_raw()
    {
        return this->buttons_value;
    }

    void Input::reset_previous()
    {
        this->prev_buttons_value = this->buttons_value;
    }

    bool Input::is_pressed(Buttons button)
    {
        return this->buttons_value & button;
    }

    bool Input::is_just_pressed(Buttons button)
    {
        return (this->buttons_value & button) && !(this->prev_buttons_value & button);
    }   

    bool Input::is_just_released(Buttons button)
    {
        return !(this->buttons_value & button) && (this->prev_buttons_value & button);
    }
}
