/**
 * @file enet_encrypted.h
 * @author Oscar Mercado omercadorico@gmail.com
 * @brief This header contains the declaration of the API related tools.
 * 
 */
#ifndef ENET_ENCRYPTED_H
#define ENET_ENCRYPTED_H

#include <stdint.h>
#include <stdbool.h>
#include "enet_encrypted_cfg.h"
#include "fsl_debug_console.h"
#include "fsl_enet.h"
#include "fsl_phy.h"
#if defined( FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET ) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET
    #include "fsl_memory.h"
#endif
#include "fsl_enet_mdio.h"
#include "fsl_phyksz8081.h"
#include "fsl_common.h"
#include "fsl_sysmpu.h"

/*******************************************************************************
 * AES RELATED
 ******************************************************************************/


/*******************************************************************************
 * ENET RELATED
 ******************************************************************************/
//Macros.

//ENET base address
#define ENET_BASE_ADD   ENET
#define PHY_ADDRESS     0x00U

//MDIO and PHY operations conf.
#define MDIO_OPS        enet_ops
#define PHY_OPS         phyksz8081_ops

//Number of receive and transmit buffers to create.
#define ENET_RXBD_NUM   4    
#define ENET_TXBD_NUM   4

//Buffer sizes.
#define ENET_RXBUFF_SIZE    ENET_FRAME_MAX_FRAMELEN 
#define ENET_TXBUFF_SIZE    ENET_FRAME_MAX_FRAMELEN

#define ENET_DATA_LENGTH    500    //Data payload length ( for TX and RX payloads ).
#define ENET_DATA_MINIM_LENGTH  46  //Minimum data payload length for ethernet frame.

#ifndef APP_ENET_BUFF_ALIGNMENT
    #define APP_ENET_BUFF_ALIGNMENT ENET_BUFF_ALIGNMENT
#endif

//PHY autonegotation timeout
#ifndef PHY_AUTONEGO_TIMEOUT_COUNT
    #define PHY_AUTONEGO_TIMEOUT_COUNT  100000
#endif

#ifndef PHY_STABILITY_DELAY_US
    #define PHY_STABILITY_DELAY_US  0
#endif

//ENET CLK Freq
#define ENET_CLK_FREQ   CLOCK_GetFreq( kCLOCK_CoreSysClk )

/*******************************************************************************
 * API functions
 ******************************************************************************/
typedef enum { E_OK, NOT_OK } ENET_Encrypted_status;

static uint8_t* ENET_Encrypted_Build_Tx_Frame( uint8_t *Tx_data, uint16_t Data_length, uint16_t *Frame_Length );
bool ENET_Encrypted_Init( void );
bool ENET_Encrypted_Send( uint8_t *Tx_data, uint16_t Data_length );
bool ENET_Encrypted_Receive( uint8_t *Rx_data, uint16_t *Data_length );

#endif