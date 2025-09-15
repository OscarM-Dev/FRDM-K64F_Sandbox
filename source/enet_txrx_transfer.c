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
    /* Hardware Initialization. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Disable SYSMPU. */
    SYSMPU_Enable( SYSMPU, false );

    PRINTF( "\r\nENET example start.\r\n" );
    
    ENET_Encrypted_Init();
    SDK_DelayAtLeastUs( ENET_TX_DELAY_US, ENET_CLK_FREQ );

    while ( 1 )
    {
        ENET_Encrypted_Test_Session();
    }
}
