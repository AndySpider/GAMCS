// -----------------------------------------------------------------------------
//
// GIMCS -- Generalized Intelligence Model and Computer Simulation
//
// Copyright (C) 2013-2014, Andy Huang  <andyspider@126.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// -----------------------------------------------------------------------------


#include <stdlib.h>
#include <sys/timeb.h>
#include <unistd.h>
#include "Avatar.h"
#include "Debug.h"

namespace gimcs
{

Avatar::Avatar() :
        name("unnamed avatar"), sps(-1), ava_loop_count(0), myagent(NULL), control_step_time(
                0)
{
}

Avatar::Avatar(std::string n) :
        name(n), sps(-1), ava_loop_count(0), myagent(NULL), control_step_time(
                0)
{
}

Avatar::~Avatar()
{
}

/**
 * \brief Launch a avatar continuously.
 */
void Avatar::Launch()
{
    while (true)
    {
        ava_loop_count++;    // inc count
        dbgmoreprt("Enter Launch Loop ", "----------------------------------------------------------- count == %d\n", ava_loop_count);

        unsigned long start_time = GetCurrentTime();

        /* Perceive the outside world */
        Agent::State cs = GetCurrentState();    // get current state
        dbgmoreprt("Launch():", "%s, State: %ld\n", name.c_str(), cs);

        /* Process stage */
        OSpace acts = ActionCandidates(cs);    // get all action candidates of a state

        Agent::Action act = myagent->Process(cs, acts);    // choose an action from candidates
        // check validation
        if (act == INVALID_ACTION)    // no valid actions available, reach a dead end, quit. !!!: be sure to check this before update stage
            break;// exit point here

        /* Update stage */
        float oripayoff = OriginalPayoff(cs);    // get original payoff of a state
        myagent->Update(oripayoff);    // agent update inner states

        /* Perform action to the outside world */
        PerformAction(act);    // otherwise, perform the action

        // handle time related job
        if (sps > 0)    // no control when sps <= 0
        {
            unsigned long end_time = GetCurrentTime();
            unsigned long consumed_time = end_time - start_time;
            long time_remaining = control_step_time - consumed_time;
            if (time_remaining > 0)    // remaining time
            {
                dbgmoreprt("",
                        "You got %ld milliseconds remaining to do other things.\n",
                        time_remaining);
                // do some useful things here if you don't want to sleep
                usleep(time_remaining * 1000);
            }
            else
            {
                WARNNING(
                        "time is not enough to run a step, %ldms in lack, try to decrease the sps!\n",
                        -time_remaining);
            }
        }
    }
    // quit
    dbgmoreprt("Exit Launch Loop", "----------------------------------------------------------- Id: %d Exit!\n", id);
    return;
}

/** \brief Get original payoff of each state.
 *  Return 1 for every state by default.
 * \param st state identity
 * \return original payoff of st
 *
 */
float Avatar::OriginalPayoff(Agent::State st)
{
    UNUSED(st);
    return 1.0;    // original payoff of states is 1.0 by default
}

unsigned long Avatar::GetCurrentTime()
{
    struct timeb tb;
    ftime(&tb);
    return 1000 * tb.time + tb.millitm;
}

}    // namespace gimcs
