/**
 * @file rgb_controller.h
 * @author Oscar Mercado omercadorico@gmail.com
 * @brief This header contains the declaration of macros, control structures and functions related to the RGB remote controller driver.
 * 
 */
#ifndef RGB_CONTROLLER_H
#define RGB_CONTROLLER_H

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Macros.
 ******************************************************************************/
#define RGB_CHANNEL_ENABLED     0
#define RGB_CHANNEL_DISABLED    1

/*******************************************************************************
 * Enums.
 ******************************************************************************/
typedef enum
{
    RED,
    GREEN,
    BLUE,
    F
} RGB_colors;

typedef enum
{
    COLOR_UP,
    COLOR_DOWN
} RGB_color_update;

/*******************************************************************************
 * Structs.
 ******************************************************************************/
/**
 * @brief Configuration structure for each RGB pin. 
 * 
 */
typedef struct
{
    GPIO_Type *Gpio;                //Ptr to GPIO base add.
    uint8_t Pin;                    //Pin number.
    gpio_pin_config_t Pin_config;   //Pin initial configuration.
} RGB_config;

/**
 * @brief Control structure for RGB LED.
 * 
 */
typedef struct
{
    RGB_config RGB_pins_conf[3];
    RGB_colors RGB_color;
} RGB_control;

/*******************************************************************************
 * RGB related functions.
 ******************************************************************************/
bool RGB_Init( void );
bool RGB_Set_Color_Cb( RGB_color_update Color_update );
bool RGB_Set_Toggle_Delay_Cb( void );

/*******************************************************************************
 * Buttons related functions.
 ******************************************************************************/


/*******************************************************************************
 * Potentiometer related functions.
 ******************************************************************************/

#endif