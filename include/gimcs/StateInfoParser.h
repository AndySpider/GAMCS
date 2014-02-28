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
// Created on: Feb 28, 2014
//
// -----------------------------------------------------------------------------

#ifndef STATEINFOPARSER_H_
#define STATEINFOPARSER_H_
#include "gimcs/Agent.h"

namespace gimcs
{

/**
 *
 */
class StateInfoParser
{
    public:
        StateInfoParser();
        StateInfoParser(const State_Info_Header *);
        virtual ~StateInfoParser();

        Action_Info_Header *Move2Act(Agent::Action);
        Action_Info_Header *FirstAct();
        Action_Info_Header *NextAct();
        EnvAction_Info *Move2Eat(Agent::EnvAction);
        EnvAction_Info *FirstEat();
        EnvAction_Info *NextEat();

    private:
        const State_Info_Header *my_sthd;
        unsigned char *atp;
        int act_index;
        unsigned char *eap;
        int eat_index;
};

} /* namespace gimcs */

#endif /* STATEINFOPARSER_H_ */
