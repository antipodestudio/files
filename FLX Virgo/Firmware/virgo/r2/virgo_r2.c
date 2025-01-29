#include "virgo_r2.h"
#include "eeconfig.h"

// PERSISTENT MEMORY CONFIGURATION

typedef struct _indicator_config_t {
    uint8_t h;
    uint8_t s;
    uint8_t v;
} indicator_config ;

typedef struct _keyboard_indicators_t {
    indicator_config sclk ;
    indicator_config caps ;
} keyboard_indicators ;

_Static_assert(sizeof(keyboard_indicators) == EECONFIG_KB_DATA_SIZE, "Mismatch in keyboard indicators stored data");

keyboard_indicators indicators;

void eeconfig_init_kb(void) {
    indicators.sclk.h = 0;
    indicators.sclk.s = 0;
    indicators.sclk.v = 255;

    indicators.caps.h = 0;
    indicators.caps.s = 0;
    indicators.caps.v = 255;

    eeconfig_update_kb_datablock(&indicators);
}

// At the keyboard start, retrieves PMEM stored configs
void keyboard_post_init_kb(void) {
    eeconfig_read_kb_datablock(&indicators);
    rgb_matrix_indicators_kb();
}

bool rgb_matrix_indicators_kb(void) {
    if (!rgb_matrix_indicators_user()) {
        return false;
    }
        HSV hsv_sclk_indicator_color = {indicators.sclk.h, indicators.sclk.s, indicators.sclk.v};
        RGB rgb_sclk_indicator_color = hsv_to_rgb(hsv_sclk_indicator_color);
        if (host_keyboard_led_state().scroll_lock) rgb_matrix_set_color(SCLK_INDICATOR_INDEX, rgb_sclk_indicator_color.r, rgb_sclk_indicator_color.g, rgb_sclk_indicator_color.b);
        else rgb_matrix_set_color(SCLK_INDICATOR_INDEX, 0, 0, 0);

        HSV hsv_caps_indicator_color = {indicators.caps.h, indicators.caps.s, indicators.caps.v};
        RGB rgb_caps_indicator_color = hsv_to_rgb(hsv_caps_indicator_color);
        if (host_keyboard_led_state().caps_lock) rgb_matrix_set_color(CAPS_INDICATOR_INDEX, rgb_caps_indicator_color.r, rgb_caps_indicator_color.g, rgb_caps_indicator_color.b);
        else rgb_matrix_set_color(CAPS_INDICATOR_INDEX, 0, 0, 0);

    return true;
}

enum via_indicator_color {
    id_sclk_indicator_brightness = 1,
    id_sclk_indicator_color = 2,
    //
    id_caps_indicator_brightness = 3,
    id_caps_indicator_color = 4,
};

void indicator_config_set_value( uint8_t *data){
    uint8_t *value_id = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch ( *value_id ){
        case id_sclk_indicator_brightness:{
            indicators.sclk.v = value_data[0];
            break;
        }
        case id_sclk_indicator_color:{
            indicators.sclk.h = value_data[0];
            indicators.sclk.s = value_data[1];
            break;
        }
        //
        case id_caps_indicator_brightness:{
            indicators.caps.v = value_data[0];
            break;
        }
        case id_caps_indicator_color:{
            indicators.caps.h = value_data[0];
            indicators.caps.s = value_data[1];
            break;
        }
    }
}

void indicator_config_get_value( uint8_t *data ){
    uint8_t *value_id = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch (*value_id){
        case id_sclk_indicator_brightness: {
            value_data[0] = indicators.sclk.v;
            break;
        }
        case id_sclk_indicator_color: {
            value_data[0] = indicators.sclk.h;
            value_data[1] = indicators.sclk.s;
            break;
        }
        //
        case id_caps_indicator_brightness: {
            value_data[0] = indicators.caps.v;
            break;
        }
        case id_caps_indicator_color: {
            value_data[0] = indicators.caps.h;
            value_data[1] = indicators.caps.s;
            break;
        }
    }
}

void indicator_config_save(void) {
    eeconfig_update_kb_datablock(&indicators);
}

void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    uint8_t *command_id = &(data[0]);
    uint8_t *channel_id = &(data[1]);
    uint8_t *value_id_and_data = &(data[2]);

    if ( *channel_id == id_custom_channel ) {
        switch ( *command_id ) {
            case id_custom_set_value:
            {
                indicator_config_set_value(value_id_and_data);
                break;
            }
            case id_custom_get_value:
            {
                indicator_config_get_value(value_id_and_data);
                break;
            }
            case id_custom_save:
            {
                indicator_config_save();
                break;
            }
            default:
            {
                *command_id = id_unhandled;
                break;
            }
        }
        return;
    }
    *command_id = id_unhandled;
}
