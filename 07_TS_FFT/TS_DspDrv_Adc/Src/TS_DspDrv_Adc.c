/***********************************************************************************
 * File              :TS_DspDrv_Adc.c
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
/**********************************************************************************
 *  Included Files
 *
 *********************************************************************************/
#include "F2837xD_device.h"
#include "F2837xD_Adc_defines.h"
#include "F2837xD_GlobalPrototypes.h"
#include "F2837xD_Examples.h"
#include "TS_DspFFT_X.h"
/**********************************************************************************
 *  External Included Files
 *
 *********************************************************************************/

/**********************************************************************************
 *  Defines
 *
 *********************************************************************************/

/**********************************************************************************
 *  Global Variables
 *
 *********************************************************************************/

Uint16 u16FftBufIndex = 0;
Uint16 u16RunFFT = 0;
/**********************************************************************************
 * \function:       TS_DspDrv_ConfigAdc
 * \brief           main `0` numbers
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void TS_DspDrv_ConfigAdc(void)
{
    EALLOW;

    /* write configurations */
    /* set ADCCLK divider to /4 */
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6;
    AdcSetMode(ADC_ADCA, ADC_BITRESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    /* Set pulse positions to late */
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    /* power up the ADC */
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    /* delay for 1ms to allow ADC time to power up */
    DELAY_US(1000);

    /*
     * ADC interrupt comes at the end of conversion
     * EOC0 will trigger ADCA1_INT
     * Enable ADCA_INT1
     * Enable ADCA_INT1 Continuous mode
     */

    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0;
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;
    AdcaRegs.ADCINTSEL1N2.bit.INT1CONT = 1;

    /*
     * Determine minimum acquisition window (in SYSCLKS) based on
     * resolution
     */
    if (ADC_BITRESOLUTION_12BIT == AdcaRegs.ADCCTL2.bit.RESOLUTION)
    {
        /* Acquisitin time is 75ns */
        AdcaRegs.ADCSOC0CTL.bit.ACQPS = 14;
    }
    else
    {
        /* resolution is 16-bit */
        AdcaRegs.ADCSOC0CTL.bit.ACQPS = 63;
    }

    /*
     * set SOC0 channel select to ADCINA0
     * set SOC0 start trigger on EPWM1A interrupt
     */

    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5;

    EDIS;
}
/**********************************************************************************
 * \function:       Adca1_ISR
 * \brief           PIE1.1 @0x000D40  ADC-A interrupt #1
 * \param[in]       void
 * \return          void
 **********************************************************************************/
interrupt void TS_DspDrv_Adca1ISR(void)
{
    /* Clear ADCINT1 flag */
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    /* Store the filtered output */
    t_rInpBuf[u16FftBufIndex++] = AdcaResultRegs.ADCRESULT0 - (1 << 11);

    /* Buffer is full start FFT */
    if (u16FftBufIndex == 1024)
    {
        /* if the full FFT size is reached, set the runFFT flag */
        u16RunFFT = 1;
        u16FftBufIndex = 0;
    }

    /* Acknowledge the end-of-task interrupt for ADCA_INT1*/
    PieCtrlRegs.PIEACK.all = M_INT1;
}
