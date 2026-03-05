/**
 * @file main.c
 * @author Oscar Mercado, omercadorico@gmail.com
 * @brief This file contains a simple example of the use of the mbedTLS library to packet multiples buffers with SHA-256.
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
#include "mbedtls/sha256.h"
#include "fsl_debug_console.h"
#include "fsl_clock.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/
#define SHA256_OUTPUT_SIZE_BYTES   32U

/*******************************************************************************
 * Private functions prototypes
 ******************************************************************************/
static int8_t sha256_compute( uint8_t *input, size_t length, uint8_t *output );
static void sha256_example( void );

/*******************************************************************************
 * Private functions definition
 ******************************************************************************/
/**
 * @brief Computes SHA-256 hash of an input buffer.
 *
 * @param input Pointer to input buffer.
 * @param length Input buffer length in bytes.
 * @param output Pointer to 32-byte hash output buffer.
 *
 * @retval Operation result.
 */
static int8_t sha256_compute( uint8_t *input, size_t length, uint8_t *output )
{
    int8_t result = -1;
    mbedtls_sha256_context ctx;

    if ( ( input != NULL ) && ( output != NULL ) )
    {
        //Initialize SHA-256 context structure.
        mbedtls_sha256_init( &ctx );

        //Configure SHA-256 (0 = SHA-256, 1 = SHA-224).
        if ( mbedtls_sha256_starts_ret( &ctx, 0 ) == 0 )
        {
            //Process input buffer.
            if ( mbedtls_sha256_update_ret( &ctx, input, length ) == 0 )
            {
                //Finalize and produce 32-byte digest.
                if ( mbedtls_sha256_finish_ret( &ctx, output ) == 0 )
                {
                    result = 0;
                }
            }
        }

        //Release SHA-256 context resources.
        mbedtls_sha256_free( &ctx );
    }

    return result;
}

/**
 * @brief Demonstrates SHA-256 hashing of multiple messages.
 */
static void sha256_example( void )
{
    //Different size messages
    uint8_t msg1[] = "Short msg";
    uint8_t msg2[] = "Medium length message for SHA256 test";
    uint8_t msg3[] = "Longer message example to demonstrate that SHA256 always produces fixed 32 byte output regardless of input size.";

    uint8_t hash[SHA256_OUTPUT_SIZE_BYTES];

    uint8_t *messages[3] = { msg1, msg2, msg3 };
    size_t lengths[3];

    //Compute lengths dynamically
    for ( uint8_t i = 0; i < 3; i++ )
    {
        lengths[i] = strlen( ( char * ) messages[i] );
    }

    //Apply hash function to each message.
    for ( uint8_t m = 0; m < 3; m++ )
    {
        PRINTF( "Message: %s\r\n", messages[m] );
        PRINTF( "Length: %u bytes\r\n", ( uint32_t ) lengths[m] );

        if ( sha256_compute( messages[m], lengths[m], hash ) == 0 )
        {
            PRINTF( "SHA-256 (HEX): " );

            for ( uint32_t i = 0; i < SHA256_OUTPUT_SIZE_BYTES; i++ )
            {
                PRINTF( "%02X", hash[i] );
            }

            PRINTF( "\r\nOutput length: %u bytes\r\n", SHA256_OUTPUT_SIZE_BYTES );
        }

        else
        {
            PRINTF( "Hash error\r\n" );
        }
    }
}

/*******************************************************************************
 * Public functions definition
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

    PRINTF( "MbedTLS SHA-256 example start.\r\n" );

    sha256_example();

    while ( 1 )
    {
    }
}