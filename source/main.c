/**
 * @file main.c
 * @author Oscar Mercado, omercadorico@gmail.com
 * @brief This file contains a simple example of the use of the mbedTLS library to encrypt and decrypt a message
 * using ChaCha20.
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
#include "mbedtls/chacha20.h"
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
static int8_t chacha20_encrypt( uint8_t *key, uint8_t *nonce, uint32_t counter, size_t length, uint8_t *input, uint8_t *output );
static int8_t chacha20_decrypt( uint8_t *key, uint8_t *nonce, uint32_t counter, size_t length, uint8_t *input, uint8_t *output );

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
 * @brief This function encrypts a given buffer with ChaCha20 using mbedTLS library.
 * 
 * @param key Pointer to encryption key.
 * @param nonce Pointer to nonce.
 * @param counter Initial counter value to use for keystream.
 * @param length Number of bytes to encrypt ( input buffer size ).
 * @param input Pointer to input buffer ( plain text ).
 * @param output Pointer to output buffer ( cypher text ).
 * @retval Operation result.
 */
int8_t chacha20_encrypt( uint8_t *key, uint8_t *nonce, uint32_t counter, size_t length, uint8_t *input, uint8_t *output )
{
    int8_t result = -1;
    mbedtls_chacha20_context ctx;

    if ( key != NULL && nonce != NULL && input != NULL && output != NULL )
    {
        //Initialize chacha20 context struct.
        mbedtls_chacha20_init( &ctx );

        //Register input matrix for keystrem generation.
        mbedtls_chacha20_setkey( &ctx, key );
        mbedtls_chacha20_starts( &ctx, nonce, counter );

        //Generate n 64byte keystreams and xor them with input buffer.
        result = mbedtls_chacha20_update( &ctx, length, input, output );

        //Free chacha20 context struct.
        mbedtls_chacha20_free( &ctx );  
    }   

    return result;
}

/**
 * @brief This function decrypts a given buffer with ChaCha20 using mbedTLS library.
 * 
 * @param key Pointer to encryption key.
 * @param nonce Pointer to nonce.
 * @param counter Initial counter value to use for keystream.
 * @param length Number of bytes to decrypt ( input buffer size ).
 * @param input Pointer to input buffer ( cypher text ).
 * @param output Pointer to output buffer ( plain text ).
 * @retval Operation result.
 */
int8_t chacha20_decrypt( uint8_t *key, uint8_t *nonce, uint32_t counter, size_t length, uint8_t *input, uint8_t *output )
{
    return chacha20_encrypt( key, nonce, counter, input, output, length );
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

    PRINTF( "\r\nMbedTLS chacha20 example start.\r\n" );

    while ( 1 )
    {
    }
}