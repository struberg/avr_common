#ifndef __USART_H__
    #define __USART_H__

#include <stdbool.h>

#ifndef USART_PORT
    #define USART_PORT PORTB
#endif

#ifndef USART_RX_PIN
    #define USART_RX_PIN PIN3_bm
#endif

#ifndef USART_RX_PINCTRL
    #define USART_RX_PINCTRL PORTB_PIN3CTRL
#endif

#ifndef USART_TX_PIN
    #define USART_TX_PIN PIN2_bm
#endif

#ifndef USART_TX_PINCTRL
    #define USART_TX_PINCTRL PORTB_PIN2CTRL
#endif


#ifndef USART_RX_BUFSIZE
    #define USART_RX_BUFSIZE 16
#endif
#ifndef USART_TX_BUFSIZE
    #define USART_TX_BUFSIZE 16
#endif

/**
 * @brief callback function which gets called whenever a full command got received via USART
 * 
 * Message format:
 * A message is a string terminated with a CR.
 * The client shall respond with an "ACK" or a message payload after the message got received and processed.
 * 
 * NOTE that the receiver callback function gets invoked from inside an Interrupt Service Routine.
 * That means it should only do some quick operations and NOT send anything back directly.
 * Instead only set some flags which can be read in the main loop and respond there when back from the ISR.
 * 
 * @param message uint8_t* pointer to the buffer
 * @param msgLen uint8_t length of the received bytes
 * @param completed FALSE if the message did not fit into the receive buffer and is only a part of the whole, 
 *             TRUE if the message is complete
 * @return 0 if the command was executed successfully, 1 otherwise.
 */
typedef int8_t (*usart_execute_command_t)(char* message, uint8_t msgLen, bool completed);

void usart_init(usart_execute_command_t usartCommandExecutor);

/**
 * @brief check if the USART transmit is busy
 * 
 * @return true if busy
 */
bool usart_tx_busy(void);

/**
 * @brief set the message to be sent
 * 
 * @param message 
 * @param maxLen 
 * @return int8_t 0 if command is stored to be sent, 1 if the tx is busy 
 */
int8_t usart_send(char* message, int maxLen); 

#endif