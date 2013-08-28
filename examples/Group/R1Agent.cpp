/**********************************************************************
*	@File:
*	@Created: 2013-8-19
*	@Author: AndyHuang - andyspider@126.coom
*	@Description:
*
*	@Modify date:
***********************************************************************/
#include "R1Agent.h"

R1Agent::R1Agent(int n, int m, float dr, float th):MyAgent(n, m, dr, th)
{
    dbgmoreprt("Enter R1Agent construct\n");
}

R1Agent::~R1Agent()
{
    dbgmoreprt("~R1Agent\n");
}

float R1Agent::OriginalPayoff(State st)
{
    if (st == 9)
        return 1;
    else if (st == 13)
        return 2;
    else
        return 0;
}

State R1Agent::ActionEffect(State st, Action act)
{
    if (act == 1)
        return st - 1;
    if (act == 2)
        return st + 1;
}
