
/*========================================================================*
 *                                                                        *
 *  Distributed by Whiteley Research Inc., Sunnyvale, California, USA     *
 *                       http://wrcad.com                                 *
 *  Copyright (C) 2017 Whiteley Research Inc., all rights reserved.       *
 *  Author: Stephen R. Whiteley, except as indicated.                     *
 *                                                                        *
 *  As fully as possible recognizing licensing terms and conditions       *
 *  imposed by earlier work from which this work was derived, if any,     *
 *  this work is released under the Apache License, Version 2.0 (the      *
 *  "License").  You may not use this file except in compliance with      *
 *  the License, and compliance with inherited licenses which are         *
 *  specified in a sub-header below this one if applicable.  A copy       *
 *  of the License is provided with this distribution, or you may         *
 *  obtain a copy of the License at                                       *
 *                                                                        *
 *        http://www.apache.org/licenses/LICENSE-2.0                      *
 *                                                                        *
 *  See the License for the specific language governing permissions       *
 *  and limitations under the License.                                    *
 *                                                                        *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES      *
 *   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-        *
 *   INFRINGEMENT.  IN NO EVENT SHALL WHITELEY RESEARCH INCORPORATED      *
 *   OR STEPHEN R. WHITELEY BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER     *
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,      *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE       *
 *   USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                        *
 *========================================================================*
 *               XicTools Integrated Circuit Design System                *
 *                                                                        *
 * WRspice Circuit Simulation and Analysis Tool:  Device Library          *
 *                                                                        *
 *========================================================================*
 $Id:$
 *========================================================================*/

/**********
based on jfetpar.c
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles

Modified to jfet2 for PS model definition ( Anthony E. Parker )
   Copyright 1994  Macquarie University, Sydney Australia.
**********/

#include "jfet2defs.h"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif


int
JFET2dev::setInst(int param, IFdata *data, sGENinstance *geninst)
{
    sJFET2instance *here = (sJFET2instance *)geninst;
    IFvalue *value = &data->v;

    switch(param) {
    case JFET2_TEMP:
        here->JFET2temp = value->rValue+CONSTCtoK;
        here->JFET2tempGiven = TRUE;
        break;
    case JFET2_AREA:
        here->JFET2area = value->rValue;
        here->JFET2areaGiven = TRUE;
        break;
    case JFET2_IC_VDS:
        here->JFET2icVDS = value->rValue;
        here->JFET2icVDSGiven = TRUE;
        break;
    case JFET2_IC_VGS:
        here->JFET2icVGS = value->rValue;
        here->JFET2icVGSGiven = TRUE;
        break;
    case JFET2_OFF:
        here->JFET2off = value->iValue;
        break;
    case JFET2_IC:
        switch(value->v.numValue) {
        case 2:
            here->JFET2icVGS = *(value->v.vec.rVec+1);
            here->JFET2icVGSGiven = TRUE;
            // fallthrough
        case 1:
            here->JFET2icVDS = *(value->v.vec.rVec);
            here->JFET2icVDSGiven = TRUE;
            data->cleanup();
            break;
        default:
            data->cleanup();
            return(E_BADPARM);
        }
        break;
    default:
        return(E_BADPARM);
    }
    return(OK);
}
