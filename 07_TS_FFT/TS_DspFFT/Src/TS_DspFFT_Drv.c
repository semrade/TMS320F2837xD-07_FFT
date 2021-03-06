/***********************************************************************************
 * File              :TS_DspFFT_Drv.c
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
#include "TS_DspFFT_Drv.h"
#include "string.h"
#include "fpu_rfft.h"
/**********************************************************************************
 *  External Included Files
 *
 *********************************************************************************/

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

/* FFT real project */
RFFT_F32_STRUCT rfft;

/**********************************************************************************
 * \function:       TS_DspFFT_Setup
 * \brief           main `6` numbers
 * \param[in]       p_prInputBuf,p_prOutputBuf,p_prCoefBuf,p_prMagBuf,p_u16Size,
 *                  p_u16Stage
 * \return          void
 **********************************************************************************/
void TS_DspFFT_Setup(float32 *p_prInputBuf, float32 *p_prOutputBuf,
                     float32 *p_prCoefBuf, float32 *p_prMagBuf,
                     Uint16 p_u16Size, Uint16 p_u16Stage)
{
    rfft.FFTSize = p_u16Size;
    rfft.FFTStages = p_u16Stage;
    rfft.InBuf = p_prInputBuf;
    rfft.OutBuf = p_prOutputBuf;
    rfft.CosSinBuf = p_prCoefBuf;
    rfft.MagBuf = p_prMagBuf;

    RFFT_f32_sincostable(&rfft);
}

/**********************************************************************************
 * \function:       TS_DspFFT_BufInit
 * \brief           main `4` numbers
 * \param[in]       p_prInputBuf, p_prOutputBuf,p_prCoefBuf, p_prMagBuf
 * \return          void
 **********************************************************************************/
void TS_DspFFT_BufInit(float32 *p_prInputBuf, float32 *p_prOutputBuf,
                       float32 *p_prCoefBuf, float32 *p_prMagBuf)
{
    memset(p_prInputBuf, 0, sizeof(p_prInputBuf));
    memset(p_prOutputBuf, 0, sizeof(p_prOutputBuf));
    memset(p_prCoefBuf, 0, sizeof(p_prCoefBuf));
    memset(p_prMagBuf, 0, sizeof(p_prMagBuf));
}

/**********************************************************************************
 * \function:       TS_DspFFT_Compute
 * \brief           main `0` numbers
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void TS_DspFFT_Compute(void)
{

    /* Calculate FFT */
    RFFT_f32(&rfft);

    /* Calculate magnitude, result stored */
    RFFT_f32_mag_TMU0(&rfft);

}
