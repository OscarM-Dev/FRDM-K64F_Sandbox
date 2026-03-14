/**
 * @file main.c
 * @author Oscar Mercado, omercadorico@gmail.com
 * @brief This file contains the code for the final project option 2, in which a message is 
 * encrypted using AES128, then hashed with sha256 and signed using ECDSA.
 * 
 * @note The mbedtls library is used.
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "mbedtls/sha256.h"
#include "mbedtls/aes.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/ecp.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "fsl_debug_console.h"
#include "fsl_clock.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/
#define RAND_SEED  12345
#define AES_KEY_SIZE_BYTES 16
#define AES_BLOCK_SIZE_BYTES 16
#define SHA256_OUTPUT_SIZE 32
#define ECDSA_SIG_MAX_LEN 72

/*******************************************************************************
 * Private functions prototypes
 ******************************************************************************/
static int myrand( void *rng_state, uint8_t *output, size_t len );
static int8_t sha256_compute( uint8_t *input, size_t length, uint8_t *output );
static int8_t aes_encrypt_buffer( uint8_t *key, uint8_t *input, uint8_t *output );
static int8_t ecdsa_sign_hash( uint8_t *hash, uint8_t *sig, size_t *sig_len );
static void crypto_project( void );

/*******************************************************************************
 * Private functions definition
 ******************************************************************************/
/**
 * @brief Simple entropy source using rand().
 *
 * @note Not cryptographically secure, used only for demonstration.
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
 * @brief Encrypts a single AES-128 block.
 *
 * @param key Pointer to AES key.
 * @param input Pointer to plaintext.
 * @param output Pointer to ciphertext.
 *
 * @retval Operation result.
 */
static int8_t aes_encrypt_buffer( uint8_t *key, uint8_t *input, uint8_t *output )
{
    int8_t result = -1;
    mbedtls_aes_context ctx;

    if ( ( key != NULL ) && ( input != NULL ) && ( output != NULL ) )
    {
        /* Initialize AES context */
        mbedtls_aes_init( &ctx );

        /* Configure encryption key */
        if ( mbedtls_aes_setkey_enc( &ctx, key, 128 ) == 0 )
        {
            /* Encrypt 16-byte block */
            if ( mbedtls_aes_crypt_ecb( &ctx, MBEDTLS_AES_ENCRYPT, input, output ) == 0 )
            {
                result = 0;
            }
        }

        /* Release AES resources */
        mbedtls_aes_free( &ctx );
    }

    return result;
}

/**
 * @brief Signs a SHA-256 hash using ECDSA.
 *
 * @param hash Pointer to message hash.
 * @param sig Pointer to signature buffer.
 * @param sig_len Pointer to signature length.
 *
 * @retval Operation result.
 */
static int8_t ecdsa_sign_hash( uint8_t *hash, uint8_t *sig, size_t *sig_len )
{
    int8_t result = -1;
    mbedtls_ecdsa_context ctx;
    mbedtls_ctr_drbg_context ctr_drbg;

    srand( RAND_SEED );

    if ( ( hash != NULL ) && ( sig != NULL ) && ( sig_len != NULL ) )
    {
        //Initialize ECDSA context.
        mbedtls_ecdsa_init( &ctx );

        //Initialize DRBG context.
        mbedtls_ctr_drbg_init( &ctr_drbg );

        //Seed random generator
        if ( mbedtls_ctr_drbg_seed( &ctr_drbg, myrand, NULL, NULL, 0 ) == 0 )
        {
            //Load elliptic curve parameters.
            if ( mbedtls_ecdsa_genkey( &ctx, MBEDTLS_ECP_DP_SECP256R1, mbedtls_ctr_drbg_random, &ctr_drbg ) == 0 )
            {
                //Generate digital signature.
                if ( mbedtls_ecdsa_write_signature( &ctx, MBEDTLS_MD_SHA256, hash, SHA256_OUTPUT_SIZE,
                        sig, sig_len, mbedtls_ctr_drbg_random, &ctr_drbg ) == 0 )
                {
                    result = 0;
                }
            }
        }

        //Free resources.
        mbedtls_ctr_drbg_free( &ctr_drbg );
        mbedtls_ecdsa_free( &ctx );
    }

    return result;
}

/**
 * @brief Demonstrates encryption, hashing of ciphertext, and digital signature.
 */
static void crypto_project( void )
{
    uint8_t message[16] = "CryptoMessageFF";
    uint8_t aes_key[AES_KEY_SIZE_BYTES] = "AESKey123456789";
    uint8_t cipher[AES_BLOCK_SIZE_BYTES];
    uint8_t hash[SHA256_OUTPUT_SIZE];
    uint8_t signature[ECDSA_SIG_MAX_LEN];
    size_t sig_len = 0;

    PRINTF( "Original Message: %s\r\n", message );

    //Encrypt message.
    if ( aes_encrypt_buffer( aes_key, message, cipher ) == 0 )
    {
        PRINTF( "AES Ciphertext:\r\n" );

        for ( uint32_t i = 0; i < AES_BLOCK_SIZE_BYTES; i++ )
        {
            PRINTF( "%02X ", cipher[i] );
        }

        PRINTF( "\r\n" );

        //Hash ciphertext.
        if ( sha256_compute( cipher, AES_BLOCK_SIZE_BYTES, hash ) == 0 )
        {
            PRINTF( "SHA256 Hash of Ciphertext:\r\n" );

            for ( uint32_t i = 0; i < SHA256_OUTPUT_SIZE; i++ )
            {
                PRINTF( "%02X", hash[i] );
            }

            PRINTF( "\r\n" );

            //Sign hash.
            if ( ecdsa_sign_hash( hash, signature, &sig_len ) == 0 )
            {
                PRINTF( "ECDSA Signature:\r\n" );

                for ( uint32_t i = 0; i < sig_len; i++ )
                {
                    PRINTF( "%02X", signature[i] );
                }

                PRINTF( "\r\n" );

                PRINTF( "Signature length: %d bytes\r\n", ( int ) sig_len );
            }

            else
            {
                PRINTF( "Signature error\r\n" );
            }
        }

        else
        {
            PRINTF( "Hash error\r\n" );
        }
    }

    else
    {
        PRINTF( "Encryption error\r\n" );
    }
}

/*******************************************************************************
 * Main
 ******************************************************************************/
int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    PRINTF( "Crypto project, option 2:\r\n" );

    crypto_project();

    while (1)
    {
    }
}