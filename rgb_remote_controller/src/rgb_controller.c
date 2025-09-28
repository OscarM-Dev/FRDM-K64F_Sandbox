/**
 * @file rgb_controller.c
 * @author Oscar Mercado omercadorico@gmail.com
 * @brief This file contains the definition of functions related to the RGB remote controller driver.
 * 
 */
#include "rgb_controller.h"

/*******************************************************************************
 * Static global data.
 ******************************************************************************/
static RGB_control RGB_LED = 
{
    .RGB_pins_conf = 
    {
        //Red channel
        { 
            .Gpio = BOARD_LED_RED_GPIO,
            .Pin = BOARD_LED_RED_GPIO_PIN,
            .Pin_config =
            {   
                .pinDirection = kGPIO_DigitalOutput,
                .outputLogic = RGB_CHANNEL_ENABLED
            }
        },
        //Green channel
        { 
            .Gpio = BOARD_LED_GREEN_GPIO,
            .Pin = BOARD_LED_GREEN_GPIO_PIN,
            .Pin_config =
            {
                .pinDirection = kGPIO_DigitalOutput,
                .outputLogic = RGB_CHANNEL_DISABLED
            }
        },
        //Blue channel.
        { 
            .Gpio = BOARD_LED_BLUE_GPIO,
            .Pin = BOARD_LED_BLUE_GPIO_PIN,
            .Pin_config =
            {
                .pinDirection = kGPIO_DigitalOutput,
                .outputLogic = RGB_CHANNEL_DISABLED
            }
        }        
    },
    .RGB_color = RED
};

/*******************************************************************************
 * RGB related functions.
 ******************************************************************************/
/**
 * @brief This function initialices the RGB pins.
 * @note The RGB pins have negative logic. 
 * 
 * @retval result of operation.
 */
bool RGB_Init( void )
{   
    bool result = true;

    for ( uint8_t i = 0; i < 3; i++ )
    {
        GPIO_PinInit( RGB_LED.RGB_pins_conf[i].Gpio, RGB_LED.RGB_pins_conf[i].Pin, &RGB_LED.RGB_pins_conf[i].Pin_config );
    }

    return result;
}