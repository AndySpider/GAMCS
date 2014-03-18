// -----------------------------------------------------------------------------
//
// GAMCS -- Generalized Intelligence Model and Computer Simulation
//
// Copyright (C) 2013-2014, Andy Huang  <andyspider@126.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// -----------------------------------------------------------------------------

#include <string>
#include <unistd.h>
#include <stdio.h>
#include "gamcs/Avatar.h"

using namespace gamcs;

/**
 * state representation:
 *  |   B\A   | silence | betrayal |
 *  | silence |    1    |    2     |
 *  | betrayal|    3    |    4     |
 *
 * action representation:
 * A:
 *   in silence:    silence(0) , betrayal(1)
 *   in betrayal:   silence(-1), betrayal(0)
 * B:
 *   in silence:    silence(0) , betrayal(2)
 *   in betrayal:   silence(-2), betrayal(0)
 *
 * payoff matrix (payoffA, payoffB):
 *  |   B\A   | silence | betrayal |
 *  | silence | -1, -1  |  0, -10  |
 *  | betrayal| -10, 0  |  -8, -8  |
 */

int current_state = 1;    // start from state 1

/* take turns to act */
bool actA = false;
bool actB = true;    // let A act first

int runtimes = 100;    // number of rounds
bool quit_notification = false;    // notify opponent to quit

class PrisonerA: public Avatar
{
    public:
        PrisonerA(std::string n) :
                Avatar(n)
        {
        }
        ~PrisonerA()
        {
        }

    private:
        Agent::State percieveState()
        {
            while (actB == false)
                usleep(10000);    // sleep, wait for B to act
            actB = false;    // next turn, set actB as false
            printf("%s, State: %d\n", name.c_str(), current_state);
            return current_state;
        }

        void performAction(Agent::Action act)
        {
            current_state += act;
            actA = true;
        }

        OSpace availableActions(Agent::State st)
        {
            static int count = 0;
            OSpace re;
            re.clear();
            if (count < runtimes)    // run times
            {
                switch (st)
                {
                    case 1:
                        re.add(0);
                        re.add(1);
                        break;
                    case 2:
                        re.add(0);
                        re.add(-1);
                        break;
                    case 3:
                        re.add(0);
                        re.add(1);
                        break;
                    case 4:
                        re.add(0);
                        re.add(-1);
                        break;
                    default:
                        printf(
                                "PrisonerA: ActionCandidates() - unknown state!\n");
                }
                count++;
            }
            else
            {
                actA = true;    //
                quit_notification = true;    // tell B to quit
            }
            return re;
        }

        float originalPayoff(Agent::State st)
        {
            float payoff = 0.0;
            switch (st)
            {
                case 1:
                    payoff = -1;
                    break;
                case 2:
                    payoff = 0;
                    break;
                case 3:
                    payoff = -10;
                    break;
                case 4:
                    payoff = -8;
                    break;
                default:
                    printf("PrisonerA: OriginalPayoff() - unknown state!\n");
            }
            return payoff;
        }
};

class PrisonerB: public Avatar
{
    public:
        PrisonerB(std::string n) :
                Avatar(n)
        {
        }
        ~PrisonerB()
        {
        }

    private:
        Agent::State percieveState()
        {
            while (actA == false)
                usleep(10000);    // sleep, wait for A to act
            actA = false;
            return current_state;
        }

        void performAction(Agent::Action act)
        {
            current_state += act;
            actB = true;
        }

        OSpace availableActions(Agent::State st)
        {
            OSpace re;
            re.clear();

            if (!quit_notification)    // check quit
            {
                switch (st)
                {
                    case 1:
                        re.add(0);
                        re.add(2);
                        break;
                    case 2:
                        re.add(0);
                        re.add(2);
                        break;
                    case 3:
                        re.add(0);
                        re.add(-2);
                        break;
                    case 4:
                        re.add(0);
                        re.add(-2);
                        break;
                    default:
                        printf(
                                "PrisonerA: ActionCandidates() - unknown state!\n");
                }
            }
            return re;
        }

        float originalPayoff(Agent::State st)
        {
            float payoff = 0.0;
            switch (st)
            {
                case 1:
                    payoff = -1;
                    break;
                case 2:
                    payoff = -10;
                    break;
                case 3:
                    payoff = 0;
                    break;
                case 4:
                    payoff = -8;
                    break;
                default:
                    printf("PrisonerA: OriginalPayoff() - unknown state!\n");
            }
            return payoff;
        }
};

