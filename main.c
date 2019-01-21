/* p4_2.c UART0 Receive
 *
 * Receiving any key from terminal emulator (TeraTerm) of the
 * host PC to the UART0 on MSP432 LaunchPad. The UART0 is
 * is connected to XDS110 on the LaunchPad and it has
 * a virtual connection to the host PC COM port.
 * Launch TeraTerm on a PC and hit any key.
 * The LED program from P2_7 of Chapter 2 is used to turn
 * on the tri-color LEDs according to the key received.
 *
 * By default the subsystem master clock is 3 MHz.
 * Setting EUSCI_A0->BRW=26 with oversampling disabled yields 115200 Baud.
 *
 * Tested with Keil 5.20 and MSP432 Device Family Pack V2.2.0
 * on XMS432P401R Rev C.
 */

#include "msp.h"

void UART0_init(void);

int main(void) {
    UART0_init();

    /* initialize P2.2-P2.0 for tri-color LEDs */
    P2->SEL1 &= ~7;         /* configure P2.2-P2.0 as simple I/O */
    P2->SEL0 &= ~7;
    P2->DIR |= 7;           /* P2.2-2.0 set as output */

    while (1) {
        while(!(EUSCI_A0->IFG & 0x01)) { }  /* wait until receive buffer is full */
        P2->OUT = EUSCI_A0->RXBUF;       /* read the receive char and set the LEDs */
    }
}

void UART0_init(void)
{
    EUSCI_A0->CTLW0 |= 1;       /* put in reset mode for config */
    EUSCI_A0->MCTLW = 0;        /* disable oversampling */
    EUSCI_A0->CTLW0 = 0x0081;   /* 1 stop bit, no parity, SMCLK, 8-bit data */
    EUSCI_A0->BRW = 26;         /* 3,000,000 / 115200 = 26 */
    P1->SEL0 |= 0x0C;           /* P1.3, P1.2 for UART */
    P1->SEL1 &= ~0x0C;
    EUSCI_A0->CTLW0 &= ~1;      /* take UART out of reset mode */
}
