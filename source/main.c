/**
 * @file main.c
 * @author Oscar Mercado, omercadorico@gmail.com
 * @brief This file contains a simple example of the use of mbedTLS library to generate pseudo random numbers 
 * in a buffer and print those numbers via serial terminal.
 * 
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "mbedtls/version.h"
#include "mbedtls/platform.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "fsl_debug_console.h"
#include "fsl_clock.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/
#define NUM_RANDOM 15
#define RAND_SEED  12345

/*******************************************************************************
 * Private functions prototypes
 ******************************************************************************/
static int myrand( void *rng_state, uint8_t *output, size_t len );
static void generate_random_numbers( void );

/*******************************************************************************
 * Private functions definition
 ******************************************************************************/
/**
 * @brief This function is used as the input entropy of the mbedTLS pseudo random numbers generator.
 * @note rand() is not entirely cryptographically secure.
 * 
 * @retval Operation result.
 */
static int myrand( void *rng_state, uint8_t *output, size_t len )
{
    size_t use_len;
    int rnd;

    ( void ) rng_state;

    while ( len > 0 )
    {
        use_len = len;
        if ( use_len > sizeof( int ) )
        {
            use_len = sizeof( int );
        }
            
        rnd = rand();
        memcpy( output, &rnd, use_len );

        output += use_len;
        len -= use_len;
    }

    return 0;
}

/**
 * @brief This function generates 15 pseudo random numbers with mbedTLS 
 * and prints them via serial terminal.
 * 
 */
void generate_random_numbers( void )
{
    mbedtls_ctr_drbg_context ctrDrbg;
    uint32_t number;

    //Generating seed for RAND function.
    srand( RAND_SEED );

    //Initializing drbg and generating seed with rand source entropy.
    mbedtls_ctr_drbg_init( &ctrDrbg );

    if ( mbedtls_ctr_drbg_seed( &ctrDrbg, myrand, NULL, NULL, 0 ) != 0 )
    {
        PRINTF( "Seed error\r\n" );
        return;
    }

    PRINTF( "%d pseudo random numbers:\r\n", NUM_RANDOM );

    //Generating random numbers.
    for ( uint8_t i = 0; i < NUM_RANDOM; i++ )
    {
        if ( mbedtls_ctr_drbg_random( &ctrDrbg, ( uint8_t * ) &number, sizeof( number ) ) != 0 )
        {
            PRINTF( "Random error\r\n" );
            break;
        }

        PRINTF( "Pseudo random number %d\t%lu\r\n", i + 1, ( uint32_t ) number );
    }

    //Free resources.
    mbedtls_ctr_drbg_free( &ctrDrbg );
}

/*******************************************************************************
 * Private functions definition
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Hardware Initialization. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    PRINTF( "\r\nMbedTLS example start.\r\n" );
    generate_random_numbers();

    while ( 1 )
    {
    }
}