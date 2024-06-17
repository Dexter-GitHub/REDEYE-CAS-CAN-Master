/*
 * sci.h
 *
 *  Created on: 2024. 6. 17.
 *      Author: Dexter
 */
#ifndef MODULE_INC_SCI_H_
#define MODULE_INC_SCI_H_

#include "component.h"
#include "DSP28x_Project.h"

#define MAX_SCI_LEN             256U
#define RECV_INT_LEVEL          0x4U        
#define SCICHAR_LENGTH_1        0x0U
#define SCICHAR_LENGTH_2        0x1U
#define SCICHAR_LENGTH_3        0x2U
#define SCICHAR_LENGTH_4        0x3U
#define SCICHAR_LENGTH_5        0x4U
#define SCICHAR_LENGTH_6        0x5U
#define SCICHAR_LENGTH_7        0x6U
#define SCICHAR_LENGTH_8        0x7U

#define BAUD_RATE               (Uint32)115200U
#define CPU_FREQ                (Uint32)150E6
#define LSPCLK_FREQ             (Uint32)(CPU_FREQ/4U)
#define SCI_FREQ                (Uint32)100E3
#define SCI_PRD                 (Uint32)((LSPCLK_FREQ / (BAUD_RATE * 8U)) - 1U)

void SCIA_Reset(void);
void SCI_SetFIFO(void);
void SCI_SetConfig(void);
int8 SCIA_ReadRxDt(void);
void SCIA_EnableIsr(void);
void SCIA_StartRxIsr(void);
void SCIA_RxIsrClear(void);
Uint16 SCIA_ReadRxAll(void);
void SCIA_RxFifoClear(void);
bool SCIA_RxInterrupt(void);
void SCIA_ErrorHandler(void);
void SCIA_TxMsg(const int8 *msg);
void SCIA_Print(const int8 *fmt, ...);
void SCIA_EnableExtFuncIsr(PINT intFunc);
void SCIA_TxData(int8 *data, const Uint32 size);
void SCIA_RxData(int8 *data, const Uint32 size);

#endif /* MODULE_INC_SCI_H_ */
