/*
 * ecan.h
 *
 *  Created on: 2024. 6. 14.
 *      Author: Dexter
 */

#ifndef MODULE_INC_ECAN_H_
#define MODULE_INC_ECAN_H_

#include "DSP28x_Project.h"
#include "component.h"

#define ECAN_SELF_TEST_MODE_DISABLE     0U
#define ECAN_SELF_TEST_MODE_ENABLE      1U

#define MAX_DATA_LEN                    8U

#define REDEYE_CAN_01_ID                0x105
#define REDEYE_CAN_02_ID                0x482

void CanLpb(void);
bool ECAN_GetIsr(void);
void ECAN_SetConfig(void);
void ECAN_SetSelfTestMode(Uint16 mode);
void ECAN_Read(Uint8 mboxNum, Uint8 *buff);
STATUS ECAN_Write(Uint8 *data, const Uint32 timeOut);

#endif /* MODULE_INC_ECAN_H_ */
