/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdlib.h>
#include "pin_mux.h"
#include "board.h"
#include "enet_encrypted.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main( void )
{
    uint32_t testTxNum = 0;

    /* Hardware Initialization. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Disable SYSMPU. */
    SYSMPU_Enable( SYSMPU, false );

    PRINTF( "\r\nENET example start.\r\n" );
    
    ENET_Encrypted_Init();

    while ( 1 )
    {
        /* Get the Frame size */
        //status = ENET_GetRxFrameSize( &g_handle, &length, 0 );

        /* Call ENET_ReadFrame when there is a received frame. */
        /*if ( length != 0 )
        {
            /* Received valid frame. Deliver the rx buffer with the size equal to length. */
      /*      uint8_t *data = ( uint8_t * )malloc( length );
            status = ENET_ReadFrame( EXAMPLE_ENET, &g_handle, data, length, 0, NULL );
            
            //Validating dest MAC address.
            if ( memcmp( g_macAddr, data, 6U ) == 0 )   //Received a valid frame
            {
                PRINTF( " A frame received. the length %d ", length );
                PRINTF( " Dest Address %02x:%02x:%02x:%02x:%02x:%02x Src Address %02x:%02x:%02x:%02x:%02x:%02x \r\n",
                    data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9],
                    data[10], data[11] );
            }
            free( data );
        }*/

       // else if ( status == kStatus_ENET_RxFrameError )
        //{
            /* Update the received buffer when error happened. */
            /* Get the error information of the received g_frame. */
           // ENET_GetRxErrBeforeReadFrame( &g_handle, &eErrStatic, 0 );
            /* update the receive buffer. */
           // ENET_ReadFrame( EXAMPLE_ENET, &g_handle, NULL, 0, 0, NULL );
        //}

        if ( testTxNum < ENET_TRANSMIT_DATA_NUM )
        {
            ENET_Encrypted_Send( NULL, 10 );
            testTxNum++;
        }
    }
}
