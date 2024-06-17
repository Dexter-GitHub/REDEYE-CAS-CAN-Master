/*
 * ecan.c
 *
 *  Created on: 2024. 6. 14.
 *      Author: Dexter
 */
#include <stdio.h>
#include <string.h>
#include <ecan.h>
#include <sci.h>

static void ECAN_WriteCancle(void);
static interrupt void ECAN_RxIsr(void);
static bool ECAN_GetError(struct CANES_BITS *canes);

static struct ECAN_REGS ECanaShadow;
Uint16 gCanErrorCount;
static int8 gMbox0Buff[MAX_DATA_LEN] = {0,};
static int8 gMbox1Buff[MAX_DATA_LEN] = {0,};
bool isrFlag = false;

void ECAN_SetConfig(void)
{
    Uint16 i = 0;

    EALLOW;
    PieVectTable.ECAN0INTA = &ECAN_RxIsr;
    EDIS;

    ECanaMboxes.MBOX0.MSGID.bit.IDE = 0;                        /* ID 확장 여부 설정 : 11bit ID 사용 */
    ECanaMboxes.MBOX0.MSGID.bit.AAM = 0;                        /* 응답모드 설정 : Nomal transmit mode */
    ECanaMboxes.MBOX0.MSGID.bit.STDMSGID = REDEYE_CAN_01_ID;    /* Mailbox ID 설정 */
    ECanaMboxes.MBOX0.MSGCTRL.bit.DLC = MAX_DATA_LEN;           /* Mailbox Data-length 설정 (최대 8byte 설정 가능) */
    ECanaMboxes.MBOX0.MDL.all = 0x00000000U;                    /* Mailbox 데이터 초기화 */
    ECanaMboxes.MBOX0.MDH.all = 0x00000000U;                    /* Mailbox 데이터 초기화 */

    ECanaMboxes.MBOX1.MSGID.bit.IDE = 0;                        /* ID 확장 여부 설정 : 11bit ID 사용 */
    ECanaMboxes.MBOX1.MSGID.bit.AAM = 0;                        /* 응답모드 설정 : Nomal transmit mode */
    ECanaMboxes.MBOX1.MSGID.bit.STDMSGID = REDEYE_CAN_01_ID;    /* Mailbox ID 설정 */
    ECanaMboxes.MBOX1.MSGCTRL.bit.DLC = MAX_DATA_LEN;           /* Mailbox Data-length 설정 (최대 8byte 설정 가능) */
    ECanaMboxes.MBOX1.MDL.all = 0x00000000U;                    /* Mailbox 데이터 초기화 */
    ECanaMboxes.MBOX1.MDH.all = 0x00000000U;                    /* Mailbox 데이터 초기화 */

    ECanaRegs.CANMD.bit.MD0 = 0U;                               /* Mailbox 0번 : 송신 */
    ECanaRegs.CANMD.bit.MD1 = 1U;                               /* Mailbox 1번 : 수신 */

    ECanaRegs.CANME.bit.ME0 = 1U;                               /* Mailbox 0번 : Enable */
    ECanaRegs.CANME.bit.ME1 = 1U;                               /* Mailbox 1번 : Enable */    

    EALLOW;
    ECanaRegs.CANMIM.bit.MIM1 = 1;                              /* Mailbox 1번 인터럽트 : Enable */
    ECanaRegs.CANMIL.bit.MIL1 = 0;                              /* Mailbox 1번 : 0번 인터럽트 라인 */
    ECanaRegs.CANGIM.bit.I0EN = 1;                              /* 0번 인터럽트 라인 : Enable */    
    EDIS;

    PieCtrlRegs.PIEIER9.bit.INTx5 = 1;                          /* PIE 인터럽트 ECAN0_INTA Enable  */    
    IER |= M_INT9;                                              /* CPU 인터럽트 (INT9) Enable */ 
    
    for (i = 0; i < MAX_DATA_LEN; i++) {
        gMbox0Buff[i] += 1;
    }
    memset((Uint8 *)gMbox1Buff, 0, MAX_DATA_LEN);
}

void CanLpb(void)
{
    Uint32 i;
    
    ECAN_Write((Uint8 *)gMbox0Buff, 10000);
    for (i = 0 ; i < 20000000; i++);

    for (i = 0; i < 8; i++) {
        if (gMbox0Buff[i] != gMbox1Buff[i]) {
            gCanErrorCount++;
        }
    }

    for (i = 0; i < 8; i++) {
        gMbox0Buff[i] += 1;
        gMbox0Buff[i] &= 0xFF;
    }
}

bool ECAN_GetIsr(void)
{
    return isrFlag;
}

static interrupt void ECAN_RxIsr(void)
{
    gMbox1Buff[0] = ECanaMboxes.MBOX1.MDL.byte.BYTE0;
    gMbox1Buff[1] = ECanaMboxes.MBOX1.MDL.byte.BYTE1;
    gMbox1Buff[2] = ECanaMboxes.MBOX1.MDL.byte.BYTE2;
    gMbox1Buff[3] = ECanaMboxes.MBOX1.MDL.byte.BYTE3;
    gMbox1Buff[4] = ECanaMboxes.MBOX1.MDH.byte.BYTE4;
    gMbox1Buff[5] = ECanaMboxes.MBOX1.MDH.byte.BYTE5;
    gMbox1Buff[6] = ECanaMboxes.MBOX1.MDH.byte.BYTE6;
    gMbox1Buff[7] = ECanaMboxes.MBOX1.MDH.byte.BYTE7;    

    ECanaShadow.CANRMP.all = 0;
    ECanaShadow.CANRMP.bit.RMP1 =1;  /* Received Message Pending */
    ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;

    isrFlag = true;
}

void ECAN_SetSelfTestMode(Uint16 mode)
{
    EALLOW;
    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    if (mode == ECAN_SELF_TEST_MODE_ENABLE) {
        ECanaShadow.CANMC.bit.STM = 1;
    }
    else {
        ECanaShadow.CANMC.bit.STM = 0U;
    }
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
    EDIS;
}

STATUS ECAN_Write(Uint8 *data, const Uint32 timeOut)
{
    int16 ret = OK;

    ECanaMboxes.MBOX0.MDL.byte.BYTE0 = data[0];
    ECanaMboxes.MBOX0.MDL.byte.BYTE1 = data[1];
    ECanaMboxes.MBOX0.MDL.byte.BYTE2 = data[2];
    ECanaMboxes.MBOX0.MDL.byte.BYTE3 = data[3];
    ECanaMboxes.MBOX0.MDH.byte.BYTE4 = data[4];
    ECanaMboxes.MBOX0.MDH.byte.BYTE5 = data[5];
    ECanaMboxes.MBOX0.MDH.byte.BYTE6 = data[6];
    ECanaMboxes.MBOX0.MDH.byte.BYTE7 = data[7];

    ECanaShadow.CANTRS.all = 0U;
    ECanaShadow.CANTRS.bit.TRS0 = 1U;
    ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;

    do {
        ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
        /* CAN Error and Status Register */
        ECanaShadow.CANES.all = ECanaRegs.CANES.all;
        if (true == ECAN_GetError(&ECanaShadow.CANES.bit)) {
            ECAN_WriteCancle();            
            ret = ERROR;
            break;
        }
    } while (ECanaShadow.CANTA.bit.TA0 == 0U);

    ECanaRegs.CANTA.all = 0U;
    ECanaRegs.CANTA.bit.TA0 = 1U;

    return ret;
}

void ECAN_Read(Uint8 mboxNum, Uint8 *buff)
{
    switch (mboxNum) {
        case 1:
            memcpy(buff, gMbox1Buff, MAX_DATA_LEN);
            memset(gMbox1Buff, 0, MAX_DATA_LEN);
            isrFlag = false;
            break;

        default: break;
    }
}

static void ECAN_WriteCancle(void)
{
    ECanaShadow.CANTRR.bit.TRR0 = 1;
    ECanaRegs.CANTRR.all = ECanaShadow.CANTRR.all;
    while (ECanaRegs.CANAA.bit.AA0 == 0U);      /* 전송 취소 확인 */

    ECanaShadow.CANAA.bit.AA0 = 1;
    ECanaRegs.CANAA.all = ECanaShadow.CANAA.all;
}

static bool ECAN_GetError(struct CANES_BITS *canes)
{
    bool error = false;

    if (canes->FE == 1) {           /* Form error flag */
        error = true;
    }
    else if (canes->BE == 1) {      /* Bit error flag */
        error = true;
    }
    else if (canes->SA1 == 1) {     /* Stuck an dominant error */
        error = true;
    }
    else if (canes->CRCE == 1) {    /* CRC error */
        error = true;
    }
    else if (canes->SE == 1) {      /* Stuff error */
        error = true;
    }
    else if (canes->ACKE == 1) {    /* Acknowledge error */
        error = true;
    }
    else if (canes->BO == 1) {      /* Bus-off status error */
        error = true;
    }

    return error;
}
