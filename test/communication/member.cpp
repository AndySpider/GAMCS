/*
 * member.cpp
 *
 *  Created on: Feb 11, 2014
 *      Author: andy
 */

#include "member.h"

Member::Member(std::string n) :
        CSThreadAvatar(n), count(0)
{
    position = 5;
}

Member::~Member()
{

}

Agent::State Member::GetCurrentState()
{
    return position;
}

void Member::PerformAction(Agent::Action act)
{
    position += act;

    if (position > 15) position = 15;
    if (position < 1) position = 1;
    return;
}

OutList Member::ActionCandidates(Agent::State st)
{
    if (count < 500)
    {
        OutList acts;
        acts.clear();
        if (st == 1)    // position 1
        {
            acts.add(1);
            count++;
            return acts;
        }

        acts.add(1);
        acts.add(-1);
        count++;
        return acts;
    }
    else
        return OutList();    // return an empty list
}

float Member::OriginalPayoff(Agent::State st)
{
    if (st == 9)
        return 1;
    else if (st == 13)
        return 2;
    else
        return 0;
}
