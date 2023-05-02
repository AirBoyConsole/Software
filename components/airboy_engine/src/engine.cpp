#include "engine.hpp"

namespace airboy 
{

    Engine::Engine() {}
    Engine::~Engine() {}

    bool Engine::construct()
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
                ESP_LOGE(ENGINE_TAG, "Pleas update game opn cartridge to the latest library version");
        }
        if (display == nullptr) return false;

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

        return true;
    }

    void Engine::run()
    {
        float delta = 0;
        setup();

        while(true)
        {
            input->block_input();

            update(delta);

            input->reset_previous();

            input->unblock_input();

            display->drawFrame();

            vTaskDelay(10);
        }
    }
}
