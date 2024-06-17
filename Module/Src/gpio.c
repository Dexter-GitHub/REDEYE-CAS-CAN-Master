/*
 * gpio.c
 *
 *  Created on: 2024. 6. 17.
 *      Author: Dexter
 */
#include "gpio.h"

void GPIO_SetConfig(void)
{
    EALLOW;
    GpioCtrlRegs.GPBDIR.bit.GPIO63 = CONFIG_GPIO_OUTPUT;

    GpioDataRegs.GPBCLEAR.all = 0xFFFFFFFFU;    
    EDIS;
}

int16 GPIO_WritePin(const GPIO_PortNum port, const Uint16 pin, const Uint32 val, const GPIO_PinState pinState)
{
    int16 ret = OK;

    switch (port) {
        case GPIOA:
            if (pin <= GPIO_PIN_15) {
                if (GPIO_GENERAL_PURPOSE_IO != MUX_REG_MASK(GpioCtrlRegs.GPAMUX1.all, pin)) {
                    ret = ERROR;
                }
            }
            else if (pin < GPIOA_PIN_MAX) {
                if (GPIO_GENERAL_PURPOSE_IO != MUX_REG_MASK(GpioCtrlRegs.GPAMUX2.all, pin)) {
                    ret = ERROR;
                }
            }
            else {
                ret = ERROR;
            }

            if (ret == OK) {
                if (CONFIG_GPIO_OUTPUT == DIR_REG_MASK(GpioCtrlRegs.GPADIR.all, pin)) {
                    if (pinState == GPIO_PIN_SET) {
                        GpioDataRegs.GPASET.all |= val;
                    }
                    else {
                        GpioDataRegs.GPACLEAR.all |= val;
                    }
                }
                else {
                    ret = ERROR;
                }
            }
            break;

        case GPIOB:
            if (pin <= GPIO_PIN_47) {
                if (GPIO_GENERAL_PURPOSE_IO != MUX_REG_MASK(GpioCtrlRegs.GPBMUX1.all, pin)) {
                    ret = ERROR;
                }
            }
            else if (pin < GPIOB_PIN_MAX) {
                if (GPIO_GENERAL_PURPOSE_IO != MUX_REG_MASK(GpioCtrlRegs.GPBMUX2.all, pin)) {
                    ret = ERROR;
                }
            }
            else {
                ret = ERROR;
            }

            if (ret == OK) {
                if (CONFIG_GPIO_OUTPUT == DIR_REG_MASK(GpioCtrlRegs.GPBDIR.all, pin)) {
                    if (pinState == GPIO_PIN_SET) {              
                        GpioDataRegs.GPBSET.all |= val;
                    }
                    else {
                        GpioDataRegs.GPBCLEAR.all |= val;
                    }
                }
                else {
                    ret = ERROR;
                }
            }
            break;

        case GPIOC:
            if (pin <= GPIO_PIN_79) {
                if (GPIO_GENERAL_PURPOSE_IO != MUX_REG_MASK(GpioCtrlRegs.GPCMUX1.all, pin)) {
                    ret = ERROR;
                }
            }
            else if (pin < GPIOC_PIN_MAX) {
                if (GPIO_GENERAL_PURPOSE_IO != MUX_REG_MASK(GpioCtrlRegs.GPCMUX2.all, pin)) {
                    ret = ERROR;
                }
            }
            else {
                ret = ERROR;
            }

            if (ret == OK) {
                if (CONFIG_GPIO_OUTPUT == DIR_REG_MASK(GpioCtrlRegs.GPCDIR.all, pin)) {
                    if (pinState == GPIO_PIN_SET) {
                        GpioDataRegs.GPCSET.all |= val;
                    }
                    else {
                        GpioDataRegs.GPCCLEAR.all |= val;
                    }
                }
                else {
                    ret = ERROR;
                }
            }
            break;

        default:
            ret = ERROR;
            break;
    }

    return ret;
}



