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
//RGB control structure.
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
    .RGB_color = RED,
    .RGB_toggle_level = LEVEL_1_1000_MS,
    .RGB_toggle_timer_counts = RGB_TOGGLE_LEVEL_1_COUNTS
};

//RGB timer handle
static TimerHandle_t RGB_timer;

/*******************************************************************************
 * RGB related functions.
 ******************************************************************************/
/**
 * @brief This function initialices the RGB pins and the RGB timer.
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

    RGB_timer = xTimerCreate( "RGB Timer", pdMS_TO_TICKS( 100 ), true, NULL, vRGB_Timer_Cb );
    xTimerStart( RGB_timer, pdMS_TO_TICKS( 10 ) );

    return result;
}

/**
 * @brief This function updates the actual RGB color.
 * 
 * @param Color_update Indicates an increment or decremnet in the RGB color.
 * @retval result of operation.
 */
bool RGB_Set_Color_Cb( RGB_color_update Color_update )
{
    bool result = false;

    switch ( Color_update )
    {
        case COLOR_UP:
            RGB_LED.RGB_color++;
            
            if ( RGB_LED.RGB_color > BLUE )
            {
                RGB_LED.RGB_color = RED;
            }
            
            result = true;
        break;

        case COLOR_DOWN:
            RGB_LED.RGB_color--;

            if ( RGB_LED.RGB_color > BLUE )
            {
                RGB_LED.RGB_color = BLUE;
            }

            result = true;
        break;

        default:
        break;
    }

    return result;
}

/**
 * @brief This function updates the actual RGB toggle delay level.
 * 
 * @param level Desired toggle level.
 * @retval result of operation.
 */
bool RGB_Set_Toggle_Delay_Cb( RGB_toggle_levels level )
{
    bool result = false;

    if ( level >= LEVEL_1_1000_MS && level <= LEVEL_4_100_MS )
    {
        RGB_LED.RGB_toggle_level = level;

        //Calculating timer counts for level
        switch ( level )
        {
            case LEVEL_1_1000_MS:
                RGB_LED.RGB_toggle_timer_counts = RGB_TOGGLE_LEVEL_1_COUNTS;
            break;

            case LEVEL_2_500_MS:
                RGB_LED.RGB_toggle_timer_counts = RGB_TOGGLE_LEVEL_2_COUNTS;
            break;

            case LEVEL_3_200_MS:
                RGB_LED.RGB_toggle_timer_counts = RGB_TOGGLE_LEVEL_3_COUNTS;
            break;

            case LEVEL_4_100_MS:
                RGB_LED.RGB_toggle_timer_counts = RGB_TOGGLE_LEVEL_4_COUNTS;
            break;
        }

        result = true;
    }

    return result;
}

/**
 * @brief This function is the RGB timer callback, which is called every 100ms.
 * 
 * @param xTimer RGB timer control structure.
 */
void vRGB_Timer_Cb( TimerHandle_t xTimer )
{
    static uint8_t count = 0;
    static bool enabled = true;

    count++;

    //Set corresponding RGB color, onlty if LED was previously active.
    if ( enabled )
    {
        GPIO_PortSet( RGB_LED.RGB_pins_conf[RED].Gpio, RGB_RED_BLUE_DISABLED );
        GPIO_PortSet( RGB_LED.RGB_pins_conf[GREEN].Gpio, RGB_GREEN_DISABLED );
        GPIO_PinWrite( RGB_LED.RGB_pins_conf[RGB_LED.RGB_color].Gpio, RGB_LED.RGB_pins_conf[RGB_LED.RGB_color].Pin, RGB_CHANNEL_ENABLED );
    }

    //Check if toggle level was accomplished.
    if ( count >= RGB_LED.RGB_toggle_timer_counts )
    {
        GPIO_PortToggle( RGB_LED.RGB_pins_conf[RGB_LED.RGB_color].Gpio, RGB_TOGGLE_CHANNEL );
        count = 0;
        enabled = !enabled;
    }

}

/*******************************************************************************
 * Buttons related functions.
 ******************************************************************************/


/*******************************************************************************
 * Potentiometer related functions.
 ******************************************************************************/