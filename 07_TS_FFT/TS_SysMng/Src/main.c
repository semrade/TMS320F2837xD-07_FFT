/***********************************************************************************
 * File                     :main.c
 *
 * Title                    :
 *
 * Author                   :Tarik SEMRADE
 *
 * Description              :
 *
 * Version                  :
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
 *********************************************************************************/

/**********************************************************************************
 *  Included Files
 *
 *********************************************************************************/

#include "F28x_Project.h"
#include "device.h"
#include "main.h"
#include "fpu_rfft.h"
#include "TS_DspDrv_Adc.h"
#include "TS_DspDrv_Epwm.h"
#include "TS_DspFFT_Drv.h"
/**********************************************************************************
 *  Defines
 *
 *********************************************************************************/
#define RFFT_SIZE_BUF 1024U

/**********************************************************************************
 *  Global Variables
 *
 *********************************************************************************/
/*  FFT Global tables */
/*
 * Buffer alignment for the input array,
 * RFFT_f32u(optional), RFFT_f32(required)
 * Output of FFT overwrites input if RFFT_STAGES is ODD
 */
#pragma DATA_SECTION(t_rInpBuf,"IOBufferBuf");
float32 t_rInpBuf[RFFT_SIZE_BUF];

/* Output of FFT here if RFFT_STAGES is EVEN */
#pragma DATA_SECTION(t_rOutBuf,"IOBufferBuf");
float32 t_rOutBuf[RFFT_SIZE_BUF];

/* Additional Buffer used in Magnitude calc */
#pragma DATA_SECTION(t_rRfftMagBuf,"RFFTmagBuf");
float32 t_rRfftMagBuf[RFFT_SIZE_BUF / 2 + 1];

/* Twiddle buffer */
#pragma DATA_SECTION(t_rRfftF32Coef,"RFFTtwiddlesBuf");
float32 t_rRfftF32Coef[RFFT_SIZE_BUF];

Uint16 u16FftBufIndex = 0;
Uint16 u16RunFFT = 0;
/**********************************************************************************
 * \function:       main
 * \brief           main `0` numbers
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void main(void)
{

    /* Set up system flash and turn peripheral clocks */
    InitSysCtrl();

    /* Init all gpio to input */
    InitGpio();

    /* Globally disable maskable CPU interrupts */
    DINT;

    /* Clear and disable all PIE interrupts */
    InitPieCtrl();

    /* Individually disable maskable CPU interrupts */
    IER = 0x0000;

    /* Clear all CPU interrupt flags */
    IFR = 0x0000;

    /* Populate the PIE interrupt vector table with */
    InitPieVectTable();

    /***********************Interrupt linking functions*****************************/
    EALLOW;
    PieVectTable.ADCA1_INT = &Adca1_ISR;
    PieCtrlRegs.PIEIER1.bit.INTx1 = 0x01;
    EDIS;

    /************************Peripheral Initialization*****************************/

    /* ADC configuration for interrupt and storing data */
    TS_DspDrv_ConfigAdc();

    /* Epwm configuration for triggering ADC and feed FFT Input */
    TS_DspDrv_ConfigEpwm();

    /* Buffer Initialization */
    TS_DspFFT_BufInit(&t_rInpBuf[0], &t_rOutBuf[0], &t_rRfftF32Coef[0],
                      &t_rRfftMagBuf[0]);

    /* Initialization of FFT object */
    TS_DspFFT_Setup(&t_rInpBuf[0], &t_rOutBuf[0], &t_rRfftF32Coef[0],
                    &t_rRfftMagBuf[0], 1024u, 10u);

    /* Enable group 1 interrupts */
    IER |= (M_INT1);

    /* Enable Global interrupt INTM */
    EINT;

    /* Enable Global real-time interrupt DBGM */
    ERTM;

    /* Infinite led loop */
    while (1)
    {
        if (1u == u16RunFFT)
        {
            /* Run FFT algorithm */
            TS_DspFFT_Compute();

            /* Reset FFT flag */
            u16RunFFT = 0u;
        }
    }

}
/**********************************************************************************
 * \function:       Adca1_ISR
 * \brief           PIE1.1 @0x000D40  ADC-A interrupt #1
 * \param[in]       void
 * \return          void
 **********************************************************************************/
interrupt void Adca1_ISR(void)
{
    /* Clear ADCINT1 flag */
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    /* Store the filtered output */
    t_rInpBuf[u16FftBufIndex++] = AdcaResultRegs.ADCRESULT0 - (1 << 11);

    /* Buffer is full start FFT */
    if (u16FftBufIndex == RFFT_SIZE_BUF)
    {
        /* if the full FFT size is reached, set the runFFT flag */
        u16RunFFT = 1;
        u16FftBufIndex = 0;
    }

    /* Acknowledge the end-of-task interrupt for ADCA_INT1*/
    PieCtrlRegs.PIEACK.all = M_INT1;
}
