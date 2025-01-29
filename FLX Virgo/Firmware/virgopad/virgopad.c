#include "virgopad.h"
#include "eeconfig.h"

// PERSISTENT MEMORY CONFIGURATION

typedef struct _indicator_config_t {
    uint8_t h;
    uint8_t s;
    uint8_t v;
} indicator_config ;

typedef struct _keyboard_indicators_t {
    indicator_config numl ;
    indicator_config layer1 ;
    indicator_config layer2 ;
} keyboard_indicators ;

_Static_assert(sizeof(keyboard_indicators) == EECONFIG_KB_DATA_SIZE, "Mismatch in keyboard indicators stored data");

keyboard_indicators indicators;

void eeconfig_init_kb(void) {
    indicators.numl.h = 0;
    indicators.numl.s = 0;
    indicators.numl.v = 255;

    indicators.layer1.h = 0;
    indicators.layer1.s = 0;
    indicators.layer1.v = 255;

    indicators.layer2.h = 0;
    indicators.layer2.s = 0;
    indicators.layer2.v = 255;

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


        HSV hsv_numl_indicator_color = {indicators.numl.h, indicators.numl.s, indicators.numl.v};
        RGB rgb_numl_indicator_color = hsv_to_rgb(hsv_numl_indicator_color);
        if (host_keyboard_led_state().num_lock) rgb_matrix_set_color(NUML_INDICATOR_INDEX, rgb_numl_indicator_color.r, rgb_numl_indicator_color.g, rgb_numl_indicator_color.b);
        else rgb_matrix_set_color(NUML_INDICATOR_INDEX, 0, 0, 0);

        HSV hsv_layer1_indicator_color = {indicators.layer1.h, indicators.layer1.s, indicators.layer1.v};
        RGB rgb_layer1_indicator_color = hsv_to_rgb(hsv_layer1_indicator_color);
        if (IS_LAYER_ON(1)) rgb_matrix_set_color(LAYER1_INDICATOR_INDEX, rgb_layer1_indicator_color.r, rgb_layer1_indicator_color.g, rgb_layer1_indicator_color.b);
        else rgb_matrix_set_color(LAYER1_INDICATOR_INDEX, 0, 0, 0);

        HSV hsv_layer2_indicator_color = {indicators.layer2.h, indicators.layer2.s, indicators.layer2.v};
        RGB rgb_layer2_indicator_color = hsv_to_rgb(hsv_layer2_indicator_color);
        if (IS_LAYER_ON(2)) rgb_matrix_set_color(LAYER2_INDICATOR_INDEX, rgb_layer2_indicator_color.r, rgb_layer2_indicator_color.g, rgb_layer2_indicator_color.b);
        else rgb_matrix_set_color(LAYER2_INDICATOR_INDEX, 0, 0, 0);

    return true;
}

enum via_indicator_color {
    id_numl_indicator_brightness = 1,
    id_numl_indicator_color = 2,
    //
    id_layer1_indicator_brightness = 3,
    id_layer1_indicator_color = 4,
    //
    id_layer2_indicator_brightness = 5,
    id_layer2_indicator_color = 6,
};

void indicator_config_set_value( uint8_t *data){
    uint8_t *value_id = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch ( *value_id ){
        case id_numl_indicator_brightness:{
            indicators.numl.v = value_data[0];
            break;
        }
        case id_numl_indicator_color:{
            indicators.numl.h = value_data[0];
            indicators.numl.s = value_data[1];
            break;
        }
        //
        case id_layer1_indicator_brightness:{
            indicators.layer1.v = value_data[0];
            break;
        }
        case id_layer1_indicator_color:{
            indicators.layer1.h = value_data[0];
            indicators.layer1.s = value_data[1];
            break;
        }
        //
        case id_layer2_indicator_brightness:{
            indicators.layer2.v = value_data[0];
            break;
        }
        case id_layer2_indicator_color:{
            indicators.layer2.h = value_data[0];
            indicators.layer2.s = value_data[1];
            break;
        }
    }
}

void indicator_config_get_value( uint8_t *data ){
    uint8_t *value_id = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch (*value_id){
        case id_numl_indicator_brightness: {
            value_data[0] = indicators.numl.v;
            break;
        }
        case id_numl_indicator_color: {
            value_data[0] = indicators.numl.h;
            value_data[1] = indicators.numl.s;
            break;
        }
        //
        case id_layer1_indicator_brightness: {
            value_data[0] = indicators.layer1.v;
            break;
        }
        case id_layer1_indicator_color: {
            value_data[0] = indicators.layer1.h;
            value_data[1] = indicators.layer1.s;
            break;
        }
        //
        case id_layer2_indicator_brightness: {
            value_data[0] = indicators.layer2.v;
            break;
        }
        case id_layer2_indicator_color: {
            value_data[0] = indicators.layer2.h;
            value_data[1] = indicators.layer2.s;
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
