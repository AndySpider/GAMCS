/*
 * main.cpp
 *
 *  Created on: Feb 17, 2014
 *      Author: andy
 */

#include "messager.h"
#include "CSMAgent.h"

int main(void)
{
    CSMAgent ma(1, 0.9, 0.01);
    Msger msger(1);
    msger.ConnectMAgent(&ma);
    msger.Run();
}


