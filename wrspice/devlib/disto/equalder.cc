
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
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1989 Jaijeet S. Roychowdhury
**********/

#include <math.h>
#include "distdefs.h"


// EqualDeriv equates partial derivatives.
//
void 
EqualDeriv(Dderivs *newd, Dderivs *old)
{
    newd->value = old->value;
    newd->d1_p = old->d1_p;
    newd->d1_q = old->d1_q;
    newd->d1_r = old->d1_r;
    newd->d2_p2 = old->d2_p2;
    newd->d2_q2 = old->d2_q2;
    newd->d2_r2 = old->d2_r2;
    newd->d2_pq = old->d2_pq;
    newd->d2_qr = old->d2_qr;
    newd->d2_pr = old->d2_pr;
    newd->d3_p3 = old->d3_p3;
    newd->d3_q3 = old->d3_q3;
    newd->d3_r3 = old->d3_r3;
    newd->d3_p2r = old->d3_p2r;
    newd->d3_p2q = old->d3_p2q;
    newd->d3_q2r = old->d3_q2r;
    newd->d3_pq2 = old->d3_pq2;
    newd->d3_pr2 = old->d3_pr2;
    newd->d3_qr2 = old->d3_qr2;
    newd->d3_pqr = old->d3_pqr;
}
