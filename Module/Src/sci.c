/*
 * sci.c
 *
 *  Created on: 2024. 6. 17.
 *      Author: Dexter
 */
#include <stdio.h>
#include <string.h>
#include <sci.h>

static void SCIA_Xmit(const Uint16 a);
static interrupt void SCIA_RxFifoIsr(void);

static int8 gRxBuf[MAX_SCI_LEN];
static bool gRxIsr = false;

void SCI_SetConfig(void)
{
    /* Initialize SCI-A */
    SciaRegs.SCICCR.bit.SCICHAR = SCICHAR_LENGTH_8;         /* Character-length control bits 2-0 */
    SciaRegs.SCICTL1.bit.RXENA = 1U;
    SciaRegs.SCICTL1.bit.TXENA = 1U;    
    SciaRegs.SCICTL2.bit.TXINTENA = 1U;         /* SCITXBUF-register interrupt enable */
    SciaRegs.SCICTL2.bit.RXBKINTENA = 1U;       /* Receiver-buffer/break interrupt enable */
    /* 115200 baudrate */
    SciaRegs.SCIHBAUD = SCI_PRD >> 8;
    SciaRegs.SCILBAUD = SCI_PRD;
    SciaRegs.SCICTL1.all = 0x0023U;             /* SWRESET SCI software reset */
}

void SCI_SetFIFO(void)
{
    /* SCI 송신 FIFO 설정 = 0xE040 */
    SciaRegs.SCIFFTX.bit.TXFFIL = 0x1U;             /* SCI 송신 FIFO 인터럽트 레벨 설정 */
    SciaRegs.SCIFFTX.bit.TXFFINTCLR = 1U;           /* SCI 송신 FIFO 인터럽트 플래그 클리어 */
    SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1U;         /* SCI 송신 FIFO reset */
    SciaRegs.SCIFFTX.bit.SCIFFENA = 1U;             /* SCI FIFO enable */
    SciaRegs.SCIFFTX.bit.SCIRST = 1U;               /* SCI reset rx/tx channel */

    /* SCI 수신 FIFO 설정 = 0x204F */
    SciaRegs.SCIFFRX.bit.RXFFIL = RECV_INT_LEVEL;   /* SCI 수신 FIFO 인터럽트 레벨 설정 */
    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1U;           /* SCI 수신 FIFO 인터럽트 플래그 클리어 */    
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 1U;          /* SCI 수신 FIFO reset */

    SciaRegs.SCIFFCT.all = 0x00U;
}

void SCIA_StartRxIsr(void)
{
    SciaRegs.SCIFFRX.bit.RXFFIENA = 1U;     /* SCI 수신 FIFO 인터럽트 Enable */
}

void SCIA_EnableIsr(void)
{
    EALLOW;
    PieVectTable.SCIRXINTA = &SCIA_RxFifoIsr;
    EDIS;

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;      // Enable the PIE block
    PieCtrlRegs.PIEIER9.bit.INTx1 = 1U;     /* SCIRXINTA(SCI-A) */

    IER |= M_INT9;
    EINT;
}

void SCIA_EnableExtFuncIsr(PINT intFunc)
{
    EALLOW;
    PieVectTable.SCIRXINTA = intFunc;
    EDIS;

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;      // Enable the PIE block
    PieCtrlRegs.PIEIER9.bit.INTx1 = 1U;     /* SCIRXINTA(SCI-A) */

    IER |= M_INT9;
    EINT;
}

void SCIA_Print(const char *fmt, ...)
{
    va_list ap;
    char str[MAX_SCI_LEN] = {0,};

    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);

    SCIA_TxMsg(str);
}

void SCIA_TxMsg(const char *msg)
{
    int i = 0;

    while (msg[i] != NULL) {
        SCIA_Xmit(msg[i]);
        i++;
    }
}

static void SCIA_Xmit(const Uint16 a)
{
    while (SciaRegs.SCIFFTX.bit.TXFFST != 0U);
    SciaRegs.SCITXBUF = a;
}

void SCIA_TxData(int8 *data, const Uint32 size)
{
    Uint16 i = 0;
    
    for (i = 0; i < size; i++) {
        SCIA_Xmit(data[i]);
    }
}

void SCIA_RxData(int8 *data, const Uint32 size)
{
    Uint16 i = 0;

    if (size <= MAX_SCI_LEN) {
        for (i = 0; i < size; i++) {
            data[i] = gRxBuf[i];
        }
    }
}

Uint16 SCIA_ReadRxAll(void)
{
    return SciaRegs.SCIRXBUF.all;    
}

int8 SCIA_ReadRxDt(void)
{
    return SciaRegs.SCIRXBUF.bit.RXDT;    
}

bool SCIA_RxInterrupt(void)
{
    return gRxIsr;
}

void SCIA_RxFifoClear(void)
{
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 0U;      /* Reset RX FIFO */
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 1U;      /* Enable RX FIFO */

    SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;        /* Clear Overflow flag */
    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;        /* Clear Interrupt flag */    

    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9;    /* Issue PIE ack */

    gRxIsr = false;
}

void SCIA_RxIsrClear(void)
{
    SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1U;       /* Clear Overflow flag */
    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1U;       /* Clear interrupt flag */
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9;    /* Issue PIE ack */
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 0U;
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 1U;
}

void SCIA_ErrorHandler(void)
{
    /* Framing Error 처리 */
    if (SciaRegs.SCIRXST.bit.FE) {
        SCIA_Reset();   
    }

    /* Parity Error 처리 */
    if (SciaRegs.SCIRXST.bit.PE) {
        SCIA_Reset();   
    }

    /* Overrun Error 처리 */
    if (SciaRegs.SCIRXST.bit.OE) {
        SCIA_Reset();   
    }    
}

void SCIA_Reset(void)
{
    SciaRegs.SCICTL1.bit.SWRESET = 0;
    SciaRegs.SCICTL1.bit.SWRESET = 1;
}

static interrupt void SCIA_RxFifoIsr(void)
{
    Uint16 i;

    for (i = 0U; i < RECV_INT_LEVEL; i++) {
        gRxBuf[i] = SciaRegs.SCIRXBUF.bit.RXDT; /* Receive emulation buffer data */     
    }    
    
    SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1U;       /* Clear Overflow flag */
    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1U;       /* Clear interrupt flag */
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9;    /* Issue PIE ack */
    
    gRxIsr = true;    
}
