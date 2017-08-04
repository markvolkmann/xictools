
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

/***************************************************************************
JSPICE3 adaptation of Spice3f2 - Copyright (c) Stephen R. Whiteley 1992
Copyright 1990 Regents of the University of California.  All rights reserved.
Authors: 1987 Gary W. Ng
         1993 Stephen R. Whiteley
****************************************************************************/

#include <stdio.h>
#include "bjtdefs.h"
#include "noisdefs.h"


//    This routine names and evaluates all of the noise sources
//    associated with BJT's.  It starts with the model *firstModel and
//    traverses all of its insts.  It then proceeds to any other models
//    on the linked list.  The total output noise density generated by
//    all of the BJT's is summed with the variable "OnDens".
//

// define the names of the noise sources
static const char *BJTnNames[BJTNSRCS] = {
    // Note that we have to keep the order
    // consistent with the index definitions
    // in BJTdefs.h
    //
    "_rc",     // noise due to rc
    "_rb",     // noise due to rb
    "_re",     // noise due to re
    "_ic",     // noise due to ic
    "_ib",     // noise due to ib
    "_1overf", // flicker (1/f) noise
    ""         // total transistor noise
};


int
BJTdev::noise (int mode, int operation, sGENmodel *genmod, sCKT *ckt,
    sNdata *data, double *OnDens)
{
    char noiname[N_MXVLNTH];
    sBJTmodel *model = static_cast<sBJTmodel*>(genmod);
    if (operation == N_OPEN) {
        // see if we have to to produce a summary report
        // if so, name all the noise generators
        if (static_cast<sNOISEAN*>(ckt->CKTcurJob)->NStpsSm == 0)
            return (OK);
        if (mode == N_DENS) {
            for ( ; model; model = model->next()) {
                sBJTinstance *inst;
                for (inst = model->inst(); inst; inst = inst->next()) {
                    
                    for (int i = 0; i < BJTNSRCS; i++) {

                        (void)sprintf(noiname, "onoise_%s%s",
                            (char*)inst->GENname, BJTnNames[i]);
                        Realloc(&data->namelist, data->numPlots+1,
                            data->numPlots);
                        ckt->newUid(&data->namelist[data->numPlots++], 0,
                            noiname, UID_OTHER);
                        // we've added one more plot
                    }
                }
            }
            return (OK);
        }

        if (mode == INT_NOIZ) {
            for ( ; model; model = model->next()) {
                sBJTinstance *inst;
                for (inst = model->inst(); inst; inst = inst->next()) {
                    
                    for (int i = 0; i < BJTNSRCS; i++) {
                        (void)sprintf(noiname, "onoise_total_%s%s",
                            (char*)inst->GENname, BJTnNames[i]);
                        Realloc(&data->namelist, data->numPlots+2,
                            data->numPlots);
                        ckt->newUid(&data->namelist[data->numPlots++], 0,
                            noiname, UID_OTHER);
                        (void)sprintf(noiname, "inoise_total_%s%s",
                            (char*)inst->GENname, BJTnNames[i]);
                        ckt->newUid(&data->namelist[data->numPlots++], 0,
                            noiname, UID_OTHER);
                        // we've added two more plots
                    }
                }
            }
        }
        return (OK);
    }

    if (operation == N_CALC) {
        if (mode == N_DENS) {
            for ( ; model; model = model->next()) {
                sBJTinstance *inst;
                for (inst = model->inst(); inst; inst = inst->next()) {
                    
                    double noizDens[BJTNSRCS];
                    double lnNdens[BJTNSRCS];
                    NevalSrc(&noizDens[BJTRCNOIZ], &lnNdens[BJTRCNOIZ],
                        ckt, THERMNOISE, inst->BJTcolPrimeNode,
                        inst->BJTcolNode,
                        model->BJTcollectorConduct * inst->BJTarea);

                    NevalSrc(&noizDens[BJTRBNOIZ], &lnNdens[BJTRBNOIZ],
                        ckt, THERMNOISE, inst->BJTbasePrimeNode,
                        inst->BJTbaseNode, inst->BJTgx);

                    NevalSrc(&noizDens[BJT_RE_NOISE], &lnNdens[BJT_RE_NOISE],
                        ckt, THERMNOISE, inst->BJTemitPrimeNode,
                        inst->BJTemitNode,
                        model->BJTemitterConduct * inst->BJTarea);

                    NevalSrc(&noizDens[BJTICNOIZ], &lnNdens[BJTICNOIZ],
                        ckt, SHOTNOISE, inst->BJTcolPrimeNode,
                        inst->BJTemitPrimeNode, inst->BJTcc);

                    NevalSrc(&noizDens[BJTIBNOIZ], &lnNdens[BJTIBNOIZ],
                        ckt, SHOTNOISE, inst->BJTbasePrimeNode,
                        inst->BJTemitPrimeNode, inst->BJTcb);

                    NevalSrc(&noizDens[BJTFLNOIZ], 0, ckt,
                        N_GAIN, inst->BJTbasePrimeNode,
                        inst->BJTemitPrimeNode, 0.0);

                    noizDens[BJTFLNOIZ] *= model->BJTfNcoef * 
                        exp(model->BJTfNexp *
                        log(SPMAX(FABS(inst->BJTcb),N_MINLOG))) /
                        data->freq;

                    lnNdens[BJTFLNOIZ] = 
                        log(SPMAX(noizDens[BJTFLNOIZ],N_MINLOG));

                    noizDens[BJTTOTNOIZ] = noizDens[BJTRCNOIZ] +
                        noizDens[BJTRBNOIZ] + noizDens[BJT_RE_NOISE] +
                        noizDens[BJTICNOIZ] + noizDens[BJTIBNOIZ] +
                        noizDens[BJTFLNOIZ];

                    lnNdens[BJTTOTNOIZ] = log(noizDens[BJTTOTNOIZ]);

                    *OnDens += noizDens[BJTTOTNOIZ];

                    int i;
                    if (data->delFreq == 0.0) { 

                        // if we haven't done any previous integration,
                        // we need to initialize our "history" variables

                        for (i = 0; i < BJTNSRCS; i++)
                            inst->BJTnVar[LNLSTDENS][i] = lnNdens[i];

                        // clear out our integration variables if it's
                        // the first pass

                        if (data->freq ==
                                ((sNOISEAN*)ckt->CKTcurJob)->JOBac.fstart()) {

                            for (i = 0; i < BJTNSRCS; i++) {
                                inst->BJTnVar[OUTNOIZ][i] = 0.0;
                                inst->BJTnVar[INNOIZ][i] = 0.0;
                            }
                        }
                    }
                    else {
                        // data->delFreq != 0.0 (we have to integrate)

                        // In order to get the best curve fit, we have
                        // to integrate each component separately

                        for (i = 0; i < BJTNSRCS; i++) {

                            if (i != BJTTOTNOIZ) {
                                double tempOnoise;
                                double tempInoise;
                                tempOnoise = data->integrate(noizDens[i],
                                    lnNdens[i], inst->BJTnVar[LNLSTDENS][i]);
                                tempInoise = data->integrate(noizDens[i]*
                                    data->GainSqInv,
                                    lnNdens[i] + data->lnGainInv,
                                    inst->BJTnVar[LNLSTDENS][i] +
                                    data->lnGainInv);

                                inst->BJTnVar[LNLSTDENS][i] = lnNdens[i];
                                data->outNoiz += tempOnoise;
                                data->inNoise += tempInoise;

                                if (((sNOISEAN*)ckt->CKTcurJob)->NStpsSm
                                        != 0) {

                                    inst->BJTnVar[OUTNOIZ][i] += tempOnoise;
                                    inst->BJTnVar[OUTNOIZ][BJTTOTNOIZ] +=
                                        tempOnoise;
                                    inst->BJTnVar[INNOIZ][i] += tempInoise;
                                    inst->BJTnVar[INNOIZ][BJTTOTNOIZ] +=
                                        tempInoise;
                                }
                            }
                        }
                    }
                    if (data->prtSummary) {
                        for (i = 0; i < BJTNSRCS; i++) {
                            // print a summary report
                            data->outpVector[data->outNumber++] =
                                noizDens[i];
                        }
                    }
                }
            }
            return (OK);
        }

        if (mode == INT_NOIZ) {
            // already calculated, just output
            if (static_cast<sNOISEAN*>(ckt->CKTcurJob)->NStpsSm == 0)
                return (OK);
            for ( ; model; model = model->next()) {
                sBJTinstance *inst;
                for (inst = model->inst(); inst; inst = inst->next()) {
                    
                    for (int i = 0; i < BJTNSRCS; i++) {
                        data->outpVector[data->outNumber++] =
                            inst->BJTnVar[OUTNOIZ][i];
                        data->outpVector[data->outNumber++] =
                            inst->BJTnVar[INNOIZ][i];
                    }
                }
            }
        }
    }
    return (OK);
}
            

