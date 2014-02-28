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


#include <pthread.h>
#include <stdio.h>
#include "gimcs/CSMAgent.h"
#include "gimcs/Mysql.h"
#include "Saleman.h"

int main(void)
{
    CSMAgent *agent = new CSMAgent(1, 0.8, 0.01);

    Saleman *saleman = new Saleman("Saleman");
    saleman->SetSps(-1);
    saleman->ConnectAgent(agent);

    pthread_t tid = saleman->ThreadLaunch();

    pthread_join(tid, NULL);

    delete saleman;
    delete agent;
}

