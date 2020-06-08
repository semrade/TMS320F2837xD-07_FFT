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


/**********************************************************************************
 *  Global Variables
 *
 *********************************************************************************/
RFFT_F32_STRUCT rfft;


/**********************************************************************************
 * \function:       TS_DspFFT_Setup
 * \brief           main `0` numbers
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void
TS_DspFFT_Setup(float32 * p_prInputBuf, float32 * p_prOutputBuf,
                float32 * p_prCoefBuf, float32 * p_prMagBuf,
                Uint16 p_u16Size, Uint16 p_u16Stage)
{
    rfft.FFTSize = p_u16Size;
    rfft.FFTStages = p_u16Stage;
    rfft.InBuf = p_prInputBuf;
    rfft.OutBuf = p_prOutputBuf;
    rfft.CosSinBuf = p_prCoefBuf;
    rfft.MagBuf = p_prMagBuf;

    RFFT_f32_sincostable (&rfft);
}

/**********************************************************************************
 * \function:       TS_DspFFT_BufInit
 * \brief           main `0` numbers
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void
TS_DspFFT_BufInit(float32 * p_prInputBuf, float32 * p_prOutputBuf,
                  float32 * p_prCoefBuf, float32 * p_prMagBuf)
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
void
TS_DspFFT_Compute(void)
{

    RFFT_f32(&rfft);            /* Calculate FFT */
    RFFT_f32_mag_TMU0(&rfft);   /* Calculate magnitude, result stored */

}
