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

#include <stdio.h>
#include "gamcs/CSOSAgent.h"
#ifdef _MYSQL_FOUND_
#include "gamcs/Mysql.h"
#endif
#include "Saleman.h"

int main(void)
{
    CSOSAgent *agent = new CSOSAgent(1, 0.8, 0.01);

    Saleman *saleman = new Saleman();
    saleman->connectAgent(agent);

    int count = 0;
    while (count < 5000)
    {
        saleman->step();

        count++;
    }

    delete saleman;
    delete agent;
}

