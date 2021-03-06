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

#include "gamcs/CSOSAgent.h"
#include "Prisoners.h"
#ifdef _MYSQL_FOUND_
#include "gamcs/Mysql.h"
#endif

int main(void)
{
    float discount_rate = 0.0;    // discount rate determines the final equilibrium!
    CSOSAgent agentA(1, discount_rate, 0.01);
    CSOSAgent agentB(2, discount_rate, 0.01);

#ifdef _MYSQL_FOUND_
    Mysql mysqlA("localhost", "root", "huangk", "PrisonerA");
    Mysql mysqlB("localhost", "root", "huangk", "PrisonerB");

    agentA.loadMemoryFromStorage(&mysqlA);
    agentB.loadMemoryFromStorage(&mysqlB);
#endif

    PrisonerA pA;
    PrisonerB pB;
    pA.connectAgent(&agentA);
    pB.connectAgent(&agentB);

    // launch
    int count = 0;
    while (count < 100)
    {
        pA.step();
        pB.step();

        count++;
    }

#ifdef _MYSQL_FOUND_
    agentA.dumpMemoryToStorage(&mysqlA);
    agentB.dumpMemoryToStorage(&mysqlB);
#endif
    return 0;
}

