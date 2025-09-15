/**
 * @file enet_encrypted.c
 * @author Oscar Mercado omercadorico@gmail.com
 * @brief This file contains the definition of the API related tools.
 * 
 */
#include "enet_encrypted.h"

//Global data.

/*******************************************************************************
 * AES RELATED
 ******************************************************************************/
static struct AES_ctx Ctx;  //AES 128 control structure.
static uint8_t AES_key[16] = { AES_128_CBC_KEY };
static uint8_t AES_iv[16] = { AES_128_CBC_IV };

/*******************************************************************************
 * ENET RELATED
 ******************************************************************************/
static enet_handle_t ENET_handle;

//Enet PHY and MDIO interface handler
static mdio_handle_t MDIO_handle = { .ops = &MDIO_OPS };
static phy_handle_t PHY_handle   = { .phyAddr = PHY_ADDRESS, .mdioHandle = &MDIO_handle, .ops = &PHY_OPS };

//Buffer descriptors array.
//Buffer descriptors should be in non-cacheable region and should be align to "ENET_BUFF_ALIGNMENT".
AT_NONCACHEABLE_SECTION_ALIGN( static enet_rx_bd_struct_t RX_buff_des[ENET_RXBD_NUM], ENET_BUFF_ALIGNMENT );
AT_NONCACHEABLE_SECTION_ALIGN( static enet_tx_bd_struct_t TX_buff_des[ENET_TXBD_NUM], ENET_BUFF_ALIGNMENT );

//Data buffers.
/*The data buffers can be in cacheable region or in non-cacheable region.
 * If use cacheable region, the alignment size should be the maximum size of "CACHE LINE SIZE" and "ENET_BUFF_ALIGNMENT"
 * If use non-cache region, the alignment size is the "ENET_BUFF_ALIGNMENT".
 */
SDK_ALIGN( static uint8_t RX_data_buff[ENET_RXBD_NUM][SDK_SIZEALIGN( ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT )], APP_ENET_BUFF_ALIGNMENT );
SDK_ALIGN( static uint8_t TX_data_buff[ENET_TXBD_NUM][SDK_SIZEALIGN( ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT )], APP_ENET_BUFF_ALIGNMENT );

static uint8_t Device_MAC[6] = MAC_ADDRESS;
static uint8_t Dest_MAC[6] = DEST_MAC_ADDRESS;

/*******************************************************************************
 * API functions
 ******************************************************************************/
/**
 * @brief This function builds the Ethernet frame to transmit.
 * @note It also encrypts the data payload with AES 128.
 * 
 * @param Tx_data Pointer to transmit data payload.
 * @param Data_length Transmit data payload length in bytes.
 * @param Frame_length Pointer to data to store the ethernet frame length in bytes
 * 
 * @return Pointer to buffer with encrypted ethernet frame.
 */
static uint8_t* ENET_Encrypted_Build_Tx_Frame( uint8_t *Tx_data, uint16_t Data_length, uint16_t *Frame_Length )
{   
    static uint8_t Transmit_frame[ENET_DATA_LENGTH + 14]; //Frame to transmit.
    uint8_t *Encrypted_frame = NULL;
    uint8_t Padding_bytes = 0; //Padding bytes to add if necessary.
    uint16_t length = Data_length;

    //Cleaning buffer.
    memset( Transmit_frame, 0, sizeof( Transmit_frame ) );

    //Building MAC HEADER.
    memcpy( Transmit_frame, Dest_MAC, 6 );  //Destination MAC.
    memcpy( &Transmit_frame[6], Device_MAC, 6 );    //Source MAC.

    //Data payload length.
    if ( length < ENET_DATA_MINIM_LENGTH )
    {   //Padding.
        Padding_bytes = ENET_DATA_MINIM_LENGTH - length;
        length += Padding_bytes;
    }

    Transmit_frame[12] = ( length >> 8 ) & 0xFF;
    Transmit_frame[13] = length & 0xFF;

    //Data payload.
    //To do, encrypt data with AES 128.

    memcpy( &Transmit_frame[14], Tx_data, Data_length );
    *Frame_Length = length + 14;

    return Transmit_frame;
}

/**
 * @brief This function gets the data payload of the received Ethernet frame with encrypted payload.
 * @note It also decrypts the data payload with AES 128.
 * 
 * @param Rx_frame Pointer to received ethernet frame ( buffer ).
 * @param Frame_length Length in bytes of received frame.
 * @param Rx_data Pointer to buffer to store the decrypted data payload without padding.
 * @param Data_length Pointer to data to store the data payload length in bytes.
 * 
 * @return result, result of operation. 
 */
static bool ENET_Encrypted_Get_Rx_Data( uint8_t *Rx_frame, uint16_t Frame_length, uint8_t *Rx_data, uint16_t *Data_length )
{
    bool result = E_OK;
    uint16_t length = Frame_length - 14;    //Data payload length without padding.
    uint8_t pad_value = 0;      //Pad value for AES padding.

    PRINTF( "Frame received. Length of %d bytes.\t", Frame_length );

    //MAC header.
    PRINTF( "Dest Address %02x:%02x:%02x:%02x:%02x:%02x Src Address %02x:%02x:%02x:%02x:%02x:%02x \r\n",
            Rx_frame[0], Rx_frame[1], Rx_frame[2], Rx_frame[3], Rx_frame[4], Rx_frame[5], Rx_frame[6], Rx_frame[7], Rx_frame[8], Rx_frame[9],
            Rx_frame[10], Rx_frame[11] );

    //Data payload.
    //Analizing if there is padding for completing the minimun length in the data payload, ignoring ethernet padding.
    if ( length == ENET_DATA_MINIM_LENGTH )
    {   //Padding could be present.
        length = 32;    //An estimated real payload length.
    }

    AES_init_ctx_iv( &Ctx, AES_key, AES_iv );   //Initializing AES control structure.
    AES_CBC_decrypt_buffer( &Ctx, &Rx_frame[14], length );  //Decrypting data payload.

    //Analizing AES padding, ignoring AES padding.
    //Obtaining pad value -->0x01 to 0x10 or 1 to 16, last data payload byte.
    pad_value = Rx_frame[ ( length + 14 ) - 1 ];
    
    if ( pad_value == 0 || pad_value > 16 )
    {   //Incorrect pad_value, usually enters when payload length <= 46
        length -= 16;
        pad_value = Rx_frame[ ( length + 14 ) - 1 ];
    }
    
    for ( uint16_t i = 0; i < length; i++ )
    {   //Detecting exactly where the padding starts.
        if ( Rx_frame[i + 14] == pad_value )
        {
            length = i;    //Real data payload length ignoring padding.
            break;
        }
    }

    memcpy( Rx_data, &Rx_frame[14], length );
    *Data_length = length;

    return result;
}   

/**
 * @brief This function initializes the ENET_Encrypted library.
 * @note ENET library is initialized, as well as the initial link with the other device.
 * 
 * @return result, result of operation.
 */
bool ENET_Encrypted_Init( void )
{
    //ENET local data.
    bool result = E_OK;
    enet_config_t ENET_config;
    phy_config_t PHY_config = { 0 };
    bool autonego = false;
    bool link = false;
    phy_speed_t speed;
    phy_duplex_t duplex;
    status_t status;
    enet_data_error_stats_t eErrStatic;
    volatile uint32_t count = 0;

    //ENET initial configuration.
    //Buffer configuration.
    enet_buffer_config_t Buff_config[] = { {
        ENET_RXBD_NUM,
        ENET_TXBD_NUM,
        SDK_SIZEALIGN( ENET_RXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT ),
        SDK_SIZEALIGN( ENET_TXBUFF_SIZE, APP_ENET_BUFF_ALIGNMENT ),
        RX_buff_des,
        TX_buff_des,
        &RX_data_buff[0][0],
        &TX_data_buff[0][0],
        true,
        true,
        NULL,
    } };

    /* Get default configuration for MII
     * config.miiMode = kENET_RmiiMode;
     * config.miiSpeed = kENET_MiiSpeed100M;
     * config.miiDuplex = kENET_MiiFullDuplex;
     * config.rxMaxFrameLen = ENET_FRAME_MAX_FRAMELEN;
     */
    ENET_GetDefaultConfig( &ENET_config );

    //The MIIMode should be set according to the different PHY interfaces.
    #ifdef EXAMPLE_PHY_INTERFACE_RGMII
        ENET_config.miiMode = kENET_RgmiiMode;
    #else
        ENET_config.miiMode = kENET_RmiiMode;
    #endif

    //PHY configuration.
    PHY_config.phyAddr = PHY_ADDRESS;
    PHY_config.autoNeg = true;

    //MDI configuration.
    MDIO_handle.resource.base = ENET_BASE_ADD;
    MDIO_handle.resource.csrClock_Hz = ENET_CLK_FREQ;

    //Initialize PHY and wait auto-negotiation over.
    PRINTF( "Wait for PHY init...\r\n" );

    do
    {  
        status = PHY_Init( &PHY_handle, &PHY_config );    //Initialize PHY.

        if ( status == kStatus_Success )
        {
            PRINTF( "Wait for PHY link up...\r\n" );

            //Wait for auto-negotiation success and link up.
            count = PHY_AUTONEGO_TIMEOUT_COUNT;
            do
            {
                PHY_GetAutoNegotiationStatus( &PHY_handle, &autonego );
                PHY_GetLinkStatus( &PHY_handle, &link );

                if ( autonego && link ) //auto-negotiation over.
                {
                    break;
                }

            } while ( --count );

            if ( !autonego )
            {
                PRINTF ("PHY Auto-negotiation failed. Please check the cable connection and link partner setting.\r\n" );
            }
        }
    } while ( !( link && autonego ) );

    #if PHY_STABILITY_DELAY_US
        //Wait a moment for PHY status to be stable.
        SDK_DelayAtLeastUs( PHY_STABILITY_DELAY_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY );
    #endif

    //Get the actual PHY link speed.
    PHY_GetLinkSpeedDuplex( &PHY_handle, &speed, &duplex );

    //Change the MII speed and duplex for actual link status.
    ENET_config.miiSpeed  = ( enet_mii_speed_t )speed;
    ENET_config.miiDuplex = ( enet_mii_duplex_t )duplex;

    ENET_Init( ENET_BASE_ADD, &ENET_handle, &ENET_config, Buff_config, Device_MAC, ENET_CLK_FREQ );
    ENET_ActiveRead( ENET_BASE_ADD );

    return result;
}

/**
 * @brief This function builds and sends an Ethernet frame with encrypted data payload.
 * 
 * @param Tx_data Pointer to transmit data payload.
 * @param Data_length Length in bytes of transmit data payload.
 * @return result, result of operation.
 */
bool ENET_Encrypted_Send( uint8_t *Tx_data, uint16_t Data_length )
{
    bool result = NOT_OK;
    bool link = false;
    uint8_t *Tx_frame = NULL;
    uint16_t Frame_length = 0;

    //Checking if link is up
    if ( PHY_GetLinkStatus( &PHY_handle, &link ) == kStatus_Success )
    {
        if ( link )
        {
            //Building encrypted ethernet frame.
            Tx_frame = ENET_Encrypted_Build_Tx_Frame( Tx_data, Data_length, &Frame_length );

            //Transmitting frame.
            if ( ENET_SendFrame( ENET_BASE_ADD, &ENET_handle, Tx_frame, Frame_length, 0, false, NULL ) == kStatus_Success )
            {
                PRINTF( "Frame transmitted!\r\n" );
                result = E_OK;
            }

            else
            {
                PRINTF( " \r\nTransmit frame failed!\r\n" );
            }
        }
     }

    return result;
}

/**
 * @brief This function receives an Ethernet frame with encrypted data payload.
 * @note It also decrypts the data payload with AES 128.
 * 
 * @param Rx_data Pointer to buffer to store the received decrypted data payload.
 * @param Data_length Pointer to data to store the received decrypted data payload length.
 * @return result, result of operation. 
 */
bool ENET_Encrypted_Receive( uint8_t *Rx_data, uint16_t *Data_length )
{
    bool result = NOT_OK;
    bool link = false;
    uint32_t length = 0;    //Received frame length.
    uint8_t *Received_frame = NULL;

    //Checking if link is up
    if ( PHY_GetLinkStatus( &PHY_handle, &link ) == kStatus_Success )
    {
        if ( link )
        {
            //Checking if an ethernet frame was received.
            ENET_GetRxFrameSize( &ENET_handle, &length, 0 );

            if ( length != 0 )
            {   //Frame received.
                Received_frame = ( uint8_t *) malloc( length );

                ENET_ReadFrame( ENET_BASE_ADD, &ENET_handle, Received_frame, length, 0, NULL ); //Storing received frame in buffer.
                
                //Validating dest MAC address.
                if ( memcmp( Device_MAC, Received_frame, 6 ) == 0 )   
                {   //Received a valid frame
                    ENET_Encrypted_Get_Rx_Data( Received_frame, length, Rx_data, Data_length ); //Obtaining decrypted data payload.
                    result = E_OK;
                }

                free( Received_frame );
            }
        }
    }

    return result;
}

/**
 * @brief This function implements the comunication session with the python script in which a sequence of strings are transmitted and received.
 * 
 */
void ENET_Encrypted_Test_Session( void )
{   
    //Transmit strings.
    static const uint8_t *Strings_tx[16] = 
    {
        "No todo lo que es oro reluce...",
        "Aún en la oscuridad...",
        "¿Qué es la vida?",
        "No temas a la oscuridad...",
        "Hasta los más pequeños...",
        "No digas que el sol se ha puesto...",
        "El coraje se encuentra...",
        "No todos los tesoros...",
        "Es peligroso...",
        "Un mago nunca llega tarde...",
        "Aún hay esperanza...",
        "El mundo está cambiando...",
        "Las raíces profundas...",
        "No se puede...",
        "Y sobre todo...",
        "De las cenizas, un fuego..."
    };

    //Receive strings.
    static const uint8_t *Strings_rx[16] =
    {
        "...Ni todos los que vagan están perdidos.",
        "...brilla una luz.",
        "Nada más que un breve caminar a la luz del sol.",
        "...pues en ella se esconden las estrellas.",
        "...pueden cambiar el curso del futuro.",
        "...si aún te queda la luna.",
        "...en los lugares más inesperados.",
        "...son oro y plata.",
        "...cruzar tu puerta.",
        "...ni pronto, Frodo Bolsón. Llega precisamente cuando se lo propone.",
        "...mientras la Compañía permanezca fiel.",
        "...Siento que algo se avecina.",
        "...no alcanzan las heladas.",
        "...pasar.",
        "...cuidado con el Anillo.",
        "...se despertará."
    };

    static uint8_t Rx_data[ENET_DATA_LENGTH];
    uint8_t i = 0;
    uint16_t Rx_data_length;

    while ( true )
    {
        for ( i = 0; i < 16; i++ )
        {
            //Transmiting string.
            ENET_Encrypted_Send( ( uint8_t* ) Strings_tx[i],  strlen( ( const char* ) Strings_tx[i] ) + 1 );

            //Receiving answer.
            memset( Rx_data, 0, sizeof( Rx_data ) );    //Cleaning buffer.
            while ( ENET_Encrypted_Receive( Rx_data, &Rx_data_length ) != E_OK );   //Waiting for answer.

            if ( memcmp( Rx_data, Strings_rx[i], Rx_data_length ) == 0 )
            {   //Received expected answer.
                PRINTF( "Data received of %d bytes: %s\r\n", Rx_data_length, Rx_data );
            }

            else
            {
                PRINTF( "Unexpected data received\r\n");
            }

            SDK_DelayAtLeastUs( ENET_TX_DELAY_US, ENET_CLK_FREQ );
        }
    }
}