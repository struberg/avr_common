#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"

static usart_execute_command_t usart_exec_fn;
static char usartRxBuf[USART_RX_BUFSIZE+1];
static uint8_t rxPos = 0;

static char usartTxBuf[USART_TX_BUFSIZE+1];
static uint8_t txSize = 0;
static uint8_t txPos = 0;


/* Normal Mode, Baud register value */
#define USART0_BAUD_RATE(BAUD_RATE) ((float)(10000000 * 64 / (16 * (float)BAUD_RATE)) + 0.5)


int8_t usart_send(char* message, int maxLen) {
    if (usart_tx_busy()) {
        return 1;
    }
    for (int i=0; i< maxLen && i < USART_TX_BUFSIZE; i++) {
        usartTxBuf[txSize] = message[i];
        txSize++;
        if (message[i] == 0) {
            break;
        }
    }

    if (txSize == 0) {
        // there was no message
        // means we are done
        return 0;
    }

    // we terminate our message with a linebreak
    usartTxBuf[txSize] = '\n';
    txSize++;
    txPos = 0;

    // Enable DRE interrupt
	USART0.CTRLA |= (1 << USART_DREIE_bp);

    if (txSize > 0) {
        // send the first byte
        USART0.TXDATAL = usartTxBuf[txPos++];
    }

    return 0; // all ok
}


bool usart_tx_busy(void) {
    return txSize > 0;
}



/**
 * @brief Initialize USART interface
 * If module is configured to disabled state, the clock to the USART is disabled
 * if this is supported by the device's clock system.
 *
 * @return Initialization status.
 * @return 1 the USART init was successful, 0 the USART init was not successful
 */
int8_t USART_0_init(void) {

	USART0.BAUD = (uint16_t)USART0_BAUD_RATE(19200); /* set baud rate register */

	USART0.CTRLA = 0 << USART_ABEIE_bp      /* Auto-baud Error Interrupt Enable: disabled */
	               | 1 << USART_DREIE_bp    /* Data Register Empty Interrupt Enabled */
	               | 0 << USART_LBME_bp     /* Loop-back Mode Enable: disabled */
	               | USART_RS485_DISABLE_gc /* RS485 Mode disabled */
	               | 1 << USART_RXCIE_bp    /* Receive Complete Interrupt Enable: enabled */
	               | 0 << USART_RXSIE_bp    /* Receiver Start Frame Interrupt Enable: disabled */
	               | 0 << USART_TXCIE_bp;   /* Transmit Complete Interrupt Enable: disabled */

	USART0.CTRLB = 0 << USART_MPCM_bp       /* Multi-processor Communication Mode: disabled */
	               | 0 << USART_ODME_bp     /* Open Drain Mode Enable: disabled */
	               | 1 << USART_RXEN_bp     /* Receiver Enable: enabled */
	               | USART_RXMODE_NORMAL_gc /* Normal mode */
	               | 0 << USART_SFDEN_bp    /* Start Frame Detection Enable: disabled */
	               | 1 << USART_TXEN_bp;    /* Transmitter Enable: enabled */

	USART0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc /* Asynchronous Mode */
	               | USART_CHSIZE_8BIT_gc      /* Character size: 8 bit */
	               | USART_PMODE_EVEN_gc       /* Even Parity */
	               | USART_SBMODE_1BIT_gc;     /* 1 stop bit */

	// USART0.DBGCTRL = 0 << USART_DBGRUN_bp; /* Debug Run: disabled */

	// USART0.EVCTRL = 0 << USART_IREI_bp; /* IrDA Event Input Enable: disabled */

	// USART0.RXPLCTRL = 0x0 << USART_RXPL_gp; /* Receiver Pulse Length: 0x0 */

	// USART0.TXPLCTRL = 0x0 << USART_TXPL_gp; /* Transmit pulse length: 0x0 */


	/* Initialize ringbuffers 
	uint8_t x;
	x = 0;

	USART_0_rx_tail     = x;
	USART_0_rx_head     = x;
	USART_0_rx_elements = x;
	USART_0_tx_tail     = x;
	USART_0_tx_head     = x;
	USART_0_tx_elements = x;
    */
    return 1;
}

void usart_init(usart_execute_command_t usartCommandExecutor) {
    usart_exec_fn = usartCommandExecutor;

	// Set pin direction to input for RX
    USART_PORT.DIRCLR = USART_RX_PIN;

	// Set pin direction to output  for TX
    USART_PORT.OUTCLR = USART_TX_PIN;
    USART_PORT.DIRSET = USART_TX_PIN;

	USART_0_init();
}


/* Interrupt service routine for RX complete */
ISR(USART0_RXC_vect) {
    uint8_t val = USART0.RXDATAL;
    if (val == 0 || val == '\n' || val == '\r') {
        // message is finished
        usartRxBuf[rxPos] = 0; // termination
        (*usart_exec_fn)(usartRxBuf, rxPos, true);
        
        rxPos = 0;
    }
    else if (rxPos >= USART_RX_BUFSIZE) {
        // buffer limit got exceeded without the message being completed
        usartRxBuf[rxPos] = 0; // termination
        (*usart_exec_fn)(usartRxBuf, rxPos, false);
        rxPos = 0;
    }
    else {
        usartRxBuf[rxPos++] = val;
    }
}

/* Interrupt service routine for Data Register Empty */
ISR(USART0_DRE_vect) {
    if (txPos == txSize) {
        // Disable UDRE interrupt
        USART0.CTRLA &= ~(1 << USART_DREIE_bp);
        txPos = 0;
        txSize = 0;
    }
    else {
        // otherwise just send the next byte 
        USART0.TXDATAL = usartTxBuf[txPos++];
    }
}

