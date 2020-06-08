/***********************************************************************************
 * File              :TS_DspDrv_Epwm.c
 *
 * Title             :
 *
 * Author            :Tarik SEMRADE
 *
 * Created on        :Mar 31, 2020
 *
 * Version           :
 *
 * Description       :
 *
 * Copyright (c) 2020 Tarik SEMRADE
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 **********************************************************************************/
#include "TS_DspDrv_Epwm.h"
#include "F2837xD_device.h"
#include "F2837xD_EPwm_defines.h"


/**********************************************************************************
 * \function:       TS_DspDrv_ConfigAdc
 * \brief           Configure the EPWMs
 *   EPWM1A will run at EPWM1_FREQ Hz and serves as the sampling clock for ADC channel
 *   A1 which will be sampling the slower running EPWM4A (at EPWM4_FREQ Hz)
 *   The default time base for the EPWM module is half the system clock
 *   i.e. TBCLK = SYSCLKOUT/2
 *   EPWM1A will be setup in count-up mode and an event generated every period
 *   this event will trigger the ADC to start sampling on channel A1
 *   EPWM4A is setup for updown count mode with CMPA level at the half period
 *   point giving a 50% duty cycle square wave at EPWM4_FREQ Hz
 *   For a desired PWM frequency F_pwm (1/T_pwm), we have
 *   T_pwm = 2 x TBPRD / TBCLK
 *   TBPRD = TBCLK/(2*F_pwm)
 *   For e.g. F_pwm = 10KHz
 *   TBPRD = 45e6/(2*10e3)
 *         = 2250
 *   F_pwm = 1KHz
 *   TBPRD = 45e6/(2*1e3)
 *         = 22500
 *   For e.g. F_pwm = 100Hz
 *   TBPRD = 45e6/(2*10e2)
 *         = 225000
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void
TS_DspDrv_ConfigEpwm(void)
{
    /*
     *   Enable start of conversion (SOC) on A
     *   An SOC event will occur when the ePWM counter is zero
     *   The ePWM will generate an SOC on the first event
     *
     *   Set the period for ePWM1
     *   By default TBPRD = 1/2 SYSCLKOUT
     *   Set the counter for up count mode
     */

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    /* CLKDIV = /16 HSPCLKDIV =/2 */
    /* TBCLK = SYSCLK/32*/
    EPwm1Regs.TBCTL.bit.CLKDIV = 4;

    EPwm1Regs.TBPRD = TS_DSPDRV_EPWM1_PERIOD;
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
    EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;

    /*
    * For testing - monitor the EPWM1A output
    * which toggles once every ePWM period (i.e
    * at the start of conversion)
    */
    EPwm1Regs.AQCTLA.bit.ZRO = AQ_TOGGLE;
    EPwm1Regs.TBCTL.bit.FREE_SOFT = 3;

    /*
    * Disable the timer (counter mode is halt)
    * Set the free/soft emulation bits to ignore the
    * emulation halt (PWM will continue to count when
    * the CPU is halted)
    */

    EPwm4Regs.TBCTL.bit.CTRMODE = 0x3;
    EPwm4Regs.TBCTL.bit.FREE_SOFT = 3;

    /*
    * Clear the counter
    * Set the period and timer phase
    * Specify when the compare A event will occur
    */

    /* CLKDIV = /16 HSPCLKDIV =/2 */
    /* TBCLK = SYSCLK/32 */
    EPwm4Regs.TBCTL.bit.CLKDIV = 4;

    EPwm4Regs.TBCTR = 0x0000;
    EPwm4Regs.TBPRD = TS_DSPDRV_EPWM4_PERIOD;
    EPwm4Regs.TBPHS.bit.TBPHS = 0x0000;
    EPwm4Regs.CMPA.bit.CMPA = TS_DSPDRV_EPWM4_DUTY_CYCLE;

    /*
    * On compare A, when counting up, pull the EPWM A output high
    * On compare A, when counting down, pull the EPWM A output low
    * Set the counter to up/down count mode
    */

    EPwm4Regs.AQCTLA.bit.CAU = AQ_SET;
    EPwm4Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

    /* Configure the GPIO0 pin to be EPWM1A output */
    /* Configure the GPIO6 pin to be EPWM4A output */
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;
    EDIS;
}
