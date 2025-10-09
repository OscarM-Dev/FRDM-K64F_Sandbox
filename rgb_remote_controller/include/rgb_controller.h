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
#include "timers.h"

/*******************************************************************************
 * Macros.
 ******************************************************************************/
#define RGB_CHANNEL_ENABLED     0
#define RGB_CHANNEL_DISABLED    1
#define RGB_RED_BLUE_DISABLED   0x00600000
#define RGB_GREEN_DISABLED      0x04000000
#define RGB_TOGGLE_CHANNEL      1 << RGB_LED.RGB_pins_conf[RGB_LED.RGB_color].Pin
#define RGB_TOGGLE_LEVEL_1_COUNTS   10
#define RGB_TOGGLE_LEVEL_2_COUNTS   5
#define RGB_TOGGLE_LEVEL_3_COUNTS   2
#define RGB_TOGGLE_LEVEL_4_COUNTS   1

/*******************************************************************************
 * Enums.
 ******************************************************************************/
/**
 * @brief RGB colors.
 * 
 */
typedef enum
{
    RED,
    GREEN,
    BLUE
} RGB_colors;

/**
 * @brief RGB updates.
 * 
 */
typedef enum
{
    COLOR_UP,
    COLOR_DOWN
} RGB_color_update;

/**
 * @brief RGB toggle levels.
 * 
 */
typedef enum
{
    LEVEL_1_1000_MS,
    LEVEL_2_500_MS,
    LEVEL_3_200_MS,
    LEVEL_4_100_MS
} RGB_toggle_levels;

/**
 * @brief Publish Topics IDs.
 * 
 */
typedef enum
{
    TOPIC_STATUS,
    TOPIC_BTN1,
    TOPIC_BTN2,
    TOPIC_POT
} RGB_MQTT_publisher_topics_IDs;

/**
 * @brief Publish Message IDs.
 * 
 */
typedef enum
{
    MESSAGE_1_STATUS_COLOR_RED,
    MESSAGE_2_STATUS_COLOR_GREEN,
    MESSAGE_3_STATUS_COLOR_BLUE,
    MESSAGE_4_BTN_1,
    MESSAGE_5_BTN_2,
    MESSAGE_6_POT_LEVEL_1,
    MESSAGE_7_POT_LEVEL_2,
    MESSAGE_8_POT_LEVEL_3,
    MESSAGE_9_POT_LEVEL_4
} RGB_MQTT_publisher_message_IDs;

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
    RGB_toggle_levels RGB_toggle_level;
    uint8_t RGB_toggle_timer_counts;
} RGB_control;

/**
 * @brief Control structure for RGB related publisher.
 * 
 */
typedef struct 
{
    uint8_t Num_Messages;   //Number of messages to publish.
} RGB_MQTT_publisher;


/*******************************************************************************
 * RGB related functions.
 ******************************************************************************/
bool RGB_Init( void );
bool RGB_Set_Color_Cb( RGB_color_update Color_update );
bool RGB_Set_Toggle_Delay_Cb( RGB_toggle_levels level );
bool RGB_Get_Status_Cb( RGB_colors *color, RGB_toggle_levels *level );
void vRGB_Timer_Cb( TimerHandle_t xTimer );
extern void vInput_Timer_Cb( TimerHandle_t xTimer );

/*******************************************************************************
 * Buttons related functions.
 ******************************************************************************/


/*******************************************************************************
 * Potentiometer related functions.
 ******************************************************************************/

#endif