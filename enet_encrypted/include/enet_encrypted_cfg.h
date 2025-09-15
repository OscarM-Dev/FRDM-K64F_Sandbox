/**
 * @file enet_encrypted_cfg.h
 * @author Oscar Mercado omercadorico@gmail.com
 * @brief This header contains the user configurable macros for the library.
 * 
 */
#ifndef ENET_ENCRYPTED_CFG_H
#define ENET_ENCRYPTED_CFG_H

/*******************************************************************************
 * AES RELATED
 ******************************************************************************/
#define CBC 1       //AES algorithm to use.
#define AES_128 1   //AES key size to use.
#define AES_128_CBC_KEY 'M', 'y', '1', '6', 'b', 'y', 't', 'e', 'K', 'e', 'y', '0', '0', '0', '0','0'
#define AES_128_CBC_IV  'M', 'y', '1', '6', 'b', 'y', 't', 'e', 'I', 'V', '0', '0', '0', '0', '0','0'

/*******************************************************************************
 * ENET RELATED
 ******************************************************************************/
#define MAC_ADDRESS         { 0xD4, 0xBE, 0xD9, 0x45, 0x22, 0x60 }  //Device MAC ADDRESS.
#define DEST_MAC_ADDRESS    { 0xC8, 0x5B, 0x76, 0x6F, 0xAE, 0x25 }  //PC MAC ADDRESS.
#define ENET_DATA_LENGTH    500    //Data payload length ( for TX and RX payloads, important for buffers ).
#define ENET_TX_DELAY_S     3      //Delay in s for transmission.
#define ENET_TX_DELAY_US    ENET_TX_DELAY_S * 1000000   //Delay in us for transmission.

#endif