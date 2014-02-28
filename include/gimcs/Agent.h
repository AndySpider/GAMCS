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
//
// Created on: Oct 19, 2013
//
// -----------------------------------------------------------------------------

#ifndef AGENT_H
#define AGENT_H
#include "gimcs/TSGIOM.h"
#include "gimcs/Debug.h"

namespace gimcs
{

const unsigned long INVALID_STATE = INVALID_INPUT;
const long INVALID_ACTION = INVALID_OUTPUT;

/**
 * Intelligent Agent
 */
class Agent: public TSGIOM
{
    public:
        typedef GIOM::Input State; /**< for agent we call an input as a state */
        typedef GIOM::Output Action; /**< action as output */
        typedef long EnvAction; /**< environment action */

        Agent();
        Agent(int);
        Agent(int, float, float);
        virtual ~Agent();

        void Update(float); /**< update memory, this function will call UpdateMemory() to do the real update */

        /* set and get functions */
        void SetDiscountRate(float);
        float GetDiscountRate() const;
        void SetThreshold(float);
        float GetThreshold() const;

    protected:
        int id; /**< agent Id */
        float discount_rate; /**< discount rate (0<,<1)when calculate state payoff */
        float threshold; /**< threshold used in payoff updating */

        OSpace Restrict(State, OSpace &) const; /**< reimplement restrict using maximun payoff rule  */

        /** These two functions are implementation dependant, declared as pure virtual functions */
        virtual OSpace MaxPayoffRule(State, OSpace &) const = 0; /**< implementation of maximun payoff rule */
        virtual void UpdateMemory(float) = 0; /**<  update states in memory given current state's original payoff*/
};

inline void Agent::SetDiscountRate(float dr)
{
    // check validity
    if (dr >= 1.0 || dr < 0)    // discount rate range [0, 1)
        ERROR(
                "Agent - discount rate must be bigger than 0 and smaller than 1.0!\n");

    discount_rate = dr;
}

inline float Agent::GetDiscountRate() const
{
    return discount_rate;
}

inline void Agent::SetThreshold(float th)
{
    if (th < 0)
    ERROR("Agent - threshold must be bigger than 0!\n");

    threshold = th;
}

inline float Agent::GetThreshold() const
{
    return threshold;
}

/** action information */
struct Action_Info_Header
{
        Agent::Action act; /**< action value */
        int eat_num; /**< number of environment actions which have been observed */
};

/** environment act information */
struct EnvAction_Info
{
        Agent::EnvAction eat; /**< environment action value */
        unsigned long count; /**< count of experiencing times */
        Agent::State nst; /**< next state */
};

/** Header of state information, this struct can be used to share with other agents */
struct State_Info_Header
{
        Agent::State st; /**< state value */
        float original_payoff; /**< original payoff */
        float payoff; /**< payoff */
        unsigned long count; /**< times of travelling through this state */
        int act_num; /**< number of actions which have been performed */
        unsigned int size; /**< size of the header (in Byte) */
};

/** memory information */
struct Memory_Info
{
        float discount_rate; /**< discount rate */
        float threshold; /**< threshold */
        unsigned long state_num; /**< total number of states in memroy */
        unsigned long lk_num; /**< total number of links between states in memory */
        Agent::State last_st; /**< last experienced state when saving memory */
        Agent::Action last_act; /**< last performed Agent::Action when saving memory */
};

}    // namespace gimcs
#endif // AGENT_H
