#include "engine.hpp"

namespace airboy 
{

    Engine::Engine() {}
    Engine::~Engine() {}

    bool Engine::construct(int fps_target)
    {
        //cerate internal i2c bus
        internal_bus = new InternalBus();
        if (internal_bus == nullptr) return false;
        ESP_ERROR_CHECK(internal_bus->init_bus());

        eeprom = new InternalEEprom(internal_bus->get_port(), 0x50);

        airboy::input_bus_cfg_t input_conf;
        airboy::display_bus_cfg_t display_conf;

        eeprom->get_display_config(&display_conf);
        eeprom->get_input_config(&input_conf);

        switch (eeprom->get_display_type())
        {
            case 0:
                display = new ILI9341Display(&display_conf);
                break;
            default:
                ESP_LOGE(ENGINE_TAG, "Unknow display");
                ESP_LOGE(ENGINE_TAG, "Pleas update game on cartridge to the latest library version");
        }
        if (display == nullptr) return false;

        renderer = new Renderer(*display, Vector2i(320, 240));

        switch (eeprom->get_input_type())
        {
            case 0:
                input = new MCP23008Input(internal_bus->get_port(), &input_conf);
                break;
            default:
                ESP_LOGE(ENGINE_TAG, "Unknow input");
                ESP_LOGE(ENGINE_TAG, "Pleas update game opn cartridge to the latest library version");
        }
        if (input == nullptr) return false;

        this->set_fps_target(fps_target);

        return true;
    }

    void Engine::run()
    {
        float delta = 0;
        uint64_t time = 0, oldtime = 0;
        setup();

        fps_target_last_time = xTaskGetTickCount();
        while(true)
        {
            if (input->is_just_pressed(airboy::Buttons::BUTTON_AIRBOY)) menu();

            oldtime = time;
            time = esp_timer_get_time();
            delta = (time - oldtime) / 1000000.0;

            renderer->camera->update();

            input->block_input();

            update(delta);

            input->reset_previous();

            input->unblock_input();

            display->draw_frame();

            if(this->fps_target != 0) vTaskDelayUntil(&fps_target_last_time, this->fps_target);
        }
    }

    void Engine::set_fps_target(int target)
    {
        //avoid devide by 0
        if (target == 0)
        {
            this->fps_target = 0;
            return;
        } 

        this->fps_target = pdMS_TO_TICKS(1000 / target);
    }

    void Engine::menu()
    {
        //darken frame buffer
        for (int row = 0; row < 320; row++)
            for (int col = 0; col < 240; col++)
                this->display->set_pixel_fast(row, col, (display->get_pixel(row, col) >> 1) & 0xEF7B);

        renderer->draw_fill_rect(Vector2i(60, 0), Vector2i(200, 180), 0xEB5A);
        renderer->draw_fill_rect(Vector2i(60, 0), Vector2i(200, 40), 0x8631);

        input->reset_previous();

        while(true)
        {
            ESP_LOGI(ENGINE_TAG, "value %d", input->is_just_pressed(airboy::Buttons::BUTTON_ACTION_A));
            if (input->is_just_pressed(airboy::Buttons::BUTTON_AIRBOY))
            {
                return;
            } 

            input->reset_previous();

            display->draw_frame();
        }
    }
}
