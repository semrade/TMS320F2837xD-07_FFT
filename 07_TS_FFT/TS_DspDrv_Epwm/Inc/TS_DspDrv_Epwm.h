/***********************************************************************************
 * File              :TS_DspDrv_Epwm.h
 *
 * Title             :
 *
 * Author            :Tarik SEMRADE
 *
 * Created on        :Apr 1, 2020
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
#ifndef TS_DSPDRV_EPWM_INC_TS_DSPDRV_EPWM_H_
#define TS_DSPDRV_EPWM_INC_TS_DSPDRV_EPWM_H_

#define TS_DSPDRV_SYS_FREQ            200000000UL
#define TS_DSPDRV_EPWM_CLKDIV         32UL

#define TS_DSPDRV_EPWM1_FREQ          10000UL
#define TS_DSPDRV_EPWM4_FREQ          1000UL
#define TS_DSPDRV_EPWM1_PERIOD        (uint16_t)(TS_DSPDRV_SYS_FREQ/(TS_DSPDRV_EPWM_CLKDIV*TS_DSPDRV_EPWM1_FREQ))
#define TS_DSPDRV_EPWM4_PERIOD        (uint16_t)(TS_DSPDRV_SYS_FREQ/(TS_DSPDRV_EPWM_CLKDIV*TS_DSPDRV_EPWM4_FREQ))
#define TS_DSPDRV_EPWM4_DUTY_CYCLE    (TS_DSPDRV_EPWM4_PERIOD/2)

void TS_DspDrv_ConfigEpwm(void);

#endif /* TS_DSPDRV_EPWM_INC_TS_DSPDRV_EPWM_H_ */
