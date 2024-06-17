/*
 * main.c
 *
 *  Created on: 2024. 6. 14.
 *      Author: Dexter
 */
#include "DSP28x_Project.h"
#include "component.h"
#include "gpio.h"
#include "sci.h"
#include "ecan.h"

void main(void)
{
    Uint8 txBuff[MAX_DATA_LEN] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    Uint8 rxBuff[MAX_DATA_LEN] = {0,};
    Uint16 i = 0, j = 7;
    Uint8 compareData = 1;
    bool status = true;

    InitSysCtrl();
    InitGpio();    
    InitECanGpio();
    InitECan();

    DINT;
    InitPieCtrl();

    IER = 0x0000;
    IFR = 0x0000;

    InitPieVectTable();

    EnableInterrupts();
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

    GPIO_SetConfig();
    ECAN_SetConfig();

    EINT;
    ERTM;

    GPIO_WritePin(GPIOB, GPIO_PIN_63, GPIO_PIN_63_VAL, GPIO_PIN_RESET);
    
    FOREVER {
        if (true == ECAN_GetIsr()) {
            ECAN_Read(1, rxBuff);
            for (i = 0; i < MAX_DATA_LEN; i++) {                
                if (rxBuff[i] != compareData) {
                    status = false;
                }

                txBuff[j] = rxBuff[i];

                j--;
                compareData++;
            }

            compareData = 1;
            j = 7;
            if (status == true) {
                GPIO_WritePin(GPIOB, GPIO_PIN_63, GPIO_PIN_63_VAL, GPIO_PIN_SET);
                DELAY_US(1000000);
                GPIO_WritePin(GPIOB, GPIO_PIN_63, GPIO_PIN_63_VAL, GPIO_PIN_RESET);
                ECAN_Write(txBuff, 1000);
            }
            else {
                status = true;
            }
        }
        DELAY_US(1000000);
    }
}
