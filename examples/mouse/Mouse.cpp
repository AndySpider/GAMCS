/**********************************************************************
 *	@File:
 *	@Created: 2013-8-19
 *	@Author: AndyHuang - andyspider@126.coom
 *	@Description:
 *
 *	@Modify date:
 ***********************************************************************/
#include "Mouse.h"

Mouse::Mouse(std::string n) :
        Incarnation(n), count(0)
{
    position = 3;
}

Mouse::~Mouse()
{
    //dtor
}

IAgent::State Mouse::GetCurrentState()
{
    return position;
}

void Mouse::PerformAction(IAgent::Action act)
{
    position += act;

    if (position > 8) position = 8;
    if (position < 1) position = 1;
    return;
}

/** \brief Get all outputs of each possible input.
 * By default, for a "I:N/O:M" it will return outputs with values from 1 to M for each input.
 *
 * \param in input identity
 * \return all possible outputs for the input
 *
 */

OSpace Mouse::ActionCandidates(IAgent::State st)
{
//    UNUSED(st);
    if (count < 50)
    {
        OSpace acts;
        acts.clear();
        acts.add(1);
        acts.add(-1);
        count++;
        return acts;
    }
    else
        return OSpace();    // return an empty list
}

float Mouse::OriginalPayoff(IAgent::State st)
{
    if (st == 6)
        return 1;
    else
        return 0;
}
