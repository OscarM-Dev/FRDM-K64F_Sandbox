/**
 * @file main.c
 * @author Oscar Mercado, omercadorico@gmail.com
 * @brief This file contains a simple example of the use of the mbedTLS library to encrypt and decrypt a message
 * using AES-128.
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
#include "mbedtls/aes.h"
#include "fsl_debug_console.h"
#include "fsl_clock.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/
#define RAND_SEED  12345
#define AES_KEY_SIZE_BYTES     16    //AES-128 
#define AES_BLOCK_SIZE_BYTES   16

/*******************************************************************************
 * Private functions prototypes
 ******************************************************************************/
static int myrand( void *rng_state, uint8_t *output, size_t len );
static int8_t aes_encrypt_buffer( uint8_t *key, uint8_t *input, uint8_t *output, size_t length );
static int8_t aes_decrypt_buffer( uint8_t *key, uint8_t *input, uint8_t *output, size_t length );
static void aes_example( void );

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
 * @brief Encrypts a buffer using AES-128 ECB mode.
 *
 * @param key Pointer to AES key (16 bytes).
 * @param input Pointer to plaintext buffer.
 * @param output Pointer to ciphertext buffer.
 * @param length Buffer length (must be multiple of 16 bytes).
 *
 * @retval Operation result.
 */
static int8_t aes_encrypt_buffer( uint8_t *key, uint8_t *input, uint8_t *output, size_t length )
{
    int8_t result = -1;
    mbedtls_aes_context ctx;
    size_t offset = 0U;

    if ( ( key != NULL ) && ( input != NULL ) && ( output != NULL ) )
    {   
        //AES-ECB works on 16-byte blocks only.
        if ( ( length % AES_BLOCK_SIZE_BYTES ) == 0 )
        {
            //Initialize AES context structure.
            mbedtls_aes_init( &ctx );

            //Configure encryption key (128 bits).
            if ( mbedtls_aes_setkey_enc( &ctx, key, 128 ) == 0 )
            {
                //Process buffer block by block.
                while ( offset < length )
                {   
                    //Encrypt one 16-byte block.
                    if ( mbedtls_aes_crypt_ecb( &ctx, MBEDTLS_AES_ENCRYPT, &input[offset], &output[offset] ) != 0 )
                    {
                        result = -1;
                        break;
                    }

                    //Move to next block.
                    offset += AES_BLOCK_SIZE_BYTES;
                    result = 0;
                }
            }
            
            //Release AES context resources.
            mbedtls_aes_free( &ctx );
        }
    }

    return result;
}

/**
 * @brief Decrypts a buffer using AES-128 ECB mode.
 *
 * @param key Pointer to AES key (16 bytes).
 * @param input Pointer to ciphertext buffer.
 * @param output Pointer to plaintext buffer.
 * @param length Buffer length (must be multiple of 16 bytes).
 *
 * @retval Operation result.
 */
static int8_t aes_decrypt_buffer( uint8_t *key, uint8_t *input, uint8_t *output, size_t length )
{
    int8_t result = -1;
    mbedtls_aes_context ctx;
    size_t offset = 0U;

    if ( ( key != NULL ) && ( input != NULL ) && ( output != NULL ) )
    {   
        //AES-ECB works on 16-byte blocks only.
        if ( ( length % AES_BLOCK_SIZE_BYTES ) == 0U )
        {
            //Initialize AES context structure.
            mbedtls_aes_init( &ctx );

            //Configure decryption key (128 bits).
            if ( mbedtls_aes_setkey_dec( &ctx, key, 128 ) == 0 )
            {   
                //Process buffer block by block.
                while ( offset < length )
                {   
                    //Decrypt one 16-byte block.
                    if ( mbedtls_aes_crypt_ecb( &ctx, MBEDTLS_AES_DECRYPT, &input[offset], &output[offset] ) != 0 )
                    {
                        result = -1;
                        break;
                    }

                    //Move to next block.
                    offset += AES_BLOCK_SIZE_BYTES;
                    result = 0;
                }
            }

            //Release AES context resources.
            mbedtls_aes_free( &ctx );
        }
    }

    return result;
}

/**
 * @brief Demonstrates AES encryption and decryption of multiple messages.
 */
static void aes_example( void )
{
    uint8_t key[AES_KEY_SIZE_BYTES];
    mbedtls_ctr_drbg_context ctrDrbg;

    //Messages must be multiple of 16 bytes
    uint8_t msg1[16] = "AES examplemsg1";
    uint8_t msg2[16] = "AES examplemsg2";
    uint8_t msg3[16] = "AES examplemsg3";

    uint8_t cipher[16];
    uint8_t decrypted[16];

    //Initialize pseudo-random seed.
    srand( RAND_SEED );

    //Initialize DRBG context.
    mbedtls_ctr_drbg_init( &ctrDrbg );

    //Seed DRBG using custom entropy source. 
    if ( mbedtls_ctr_drbg_seed( &ctrDrbg, myrand, NULL, NULL, 0 ) == 0 )
    {
        uint8_t *messages[3] = { msg1, msg2, msg3 };

        //Generate random AES key.
        ( void ) mbedtls_ctr_drbg_random( &ctrDrbg, key, AES_KEY_SIZE_BYTES );
        
        for ( uint8_t m = 0; m < 3; m++ )
        {
            PRINTF( "\r\nPlaintext: %s\r\n", messages[m] );

            if ( aes_encrypt_buffer( key, messages[m], cipher, 16U ) == 0 )
            {
                PRINTF( "Ciphertext (HEX): " );

                for ( uint32_t i = 0U; i < 16U; i++ )
                {
                    PRINTF( "%02X ", cipher[i] );
                }

                PRINTF( "\r\n" );

                if ( aes_decrypt_buffer( key, cipher, decrypted, 16U ) == 0 )
                {
                    PRINTF( "Decrypted: %s\r\n", decrypted );
                }

                else
                {
                    PRINTF( "Decryption error\r\n" );
                }
            }

            else
            {
                PRINTF( "Encryption error\r\n" );
            }
        }
    }

    else
    {
        PRINTF( "Seed error\r\n" );
    }

    //Free DRBG resources.
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

    PRINTF( "\r\nMbedTLS AES example start.\r\n" );

    aes_example();

    while ( 1 )
    {
    }
}