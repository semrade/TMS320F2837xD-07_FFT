/***********************************************************************************
 * File              :TS_DspFFT_Drv.h
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
#ifndef TS_DSPFFT_INC_TS_DSPFFT_DRV_H_
#define TS_DSPFFT_INC_TS_DSPFFT_DRV_H_



void TS_DspFFT_Setup(float32 * p_prInputBuf, float32 * p_prOutputBuf,
                     float32 * p_prCoefBuf, float32 * p_prMagBuf,
                     Uint16 p_u16Size, Uint16 p_u16Stage);

void TS_DspFFT_BufInit(float32 * p_prInputBuf, float32 * p_prOutputBuf,
                    float32 * p_prCoefBuf, float32 * p_prMagBuf);

void TS_DspFFT_Compute(void);

#endif /* TS_DSPFFT_INC_TS_DSPFFT_DRV_H_ */
