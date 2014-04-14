// -----------------------------------------------------------------------------
//
// GAMCS -- Generalized Agent Model and Computer Simulation
//
// Copyright (C) 2013-2014, Andy Huang  <andyspider@126.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//
// -----------------------------------------------------------------------------
//
// Created on: Feb 17, 2014
//
// -----------------------------------------------------------------------------

#ifndef OSAGENT_H_
#define OSAGENT_H_

#include "gamcs/Agent.h"
#include "gamcs/Storage.h"

namespace gamcs
{

/**
 *  Open Storage Agent.
 *  Agent with storage interface.
 */
class OSAgent: public Agent, public Storage
{
    public:
        OSAgent(int id = 0, float discount_rate = 0.9, float threshold = 0.01) :
                Agent(id, discount_rate, threshold)
        {
        }

        virtual ~OSAgent()
        {
        }

        virtual void updatePayoff(State state) = 0; /**< update payoff from a specified state */
};

}    // namespace gamcs

#endif /* OSAGENT_H_ */
