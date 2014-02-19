// -----------------------------------------------------------------------------
//
// GIMCS -- Generalized Intelligence Model and Computer Simulation
//
// Copyright (C) 2013-2014, Andy Huang  <andyspider@126.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
// -----------------------------------------------------------------------------


#include "Debug.h"
#include "Agent.h"

namespace gimcs
{

/**
 * \brief Pretty print State information
 * \param specified State information header
 */
void PrintStateInfo(const struct State_Info_Header *stif)
{
    if (stif == NULL) return;

    int i = 0;
    printf("++++++++++++++++++++++++ State: %ld ++++++++++++++++++++++++++\n",
            stif->st);
    printf("Original payoff: %.2f,\t Payoff: %.2f,\t Count: %ld\n",
            stif->original_payoff, stif->payoff, stif->count);
    printf("------------------- ExActions, Num: %d -----------------------\n",
            stif->eat_num);
    unsigned char *p = (unsigned char *) stif;
    p += sizeof(struct State_Info_Header);
    struct EnvAction_Info *eaif = (struct EnvAction_Info *) p;
    for (i = 0; i < stif->eat_num; i++)
    {
        printf("\t EnvAction: %ld,\t\t Count: %ld\n", eaif[i].eat,
                eaif[i].count);
    }
    printf("--------------------- Actions, Num: %d -----------------------\n",
            stif->act_num);
    int len = stif->eat_num * sizeof(struct EnvAction_Info);
    p += len;
    struct Action_Info *atif = (struct Action_Info *) p;
    for (i = 0; i < stif->act_num; i++)
    {
        printf("\t Action: %ld,\t\t Payoff: %.2f\n", atif[i].act,
                atif[i].payoff);
    }
    printf("------------------- ForwardLinks, Num: %d --------------------\n",
            stif->lk_num);
    len = stif->act_num * sizeof(struct Action_Info);
    p += len;
    struct Forward_Link_Info *lk = (struct Forward_Link_Info *) p;
    for (i = 0; i < stif->lk_num; i++)
    {
        printf("\t ForwardLink:\t\t  .|+++ %ld +++ %ld ++> %ld\n", lk[i].eat,
                lk[i].act, lk[i].nst);
    }
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");

    return;
}

}    // namespace gimcs

