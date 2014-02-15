/*
 * main.cpp
 *
 *  Created on: Feb 11, 2014
 *      Author: andy
 */

#include <pthread.h>
#include <stdio.h>
#include "CSThreadDENet.h"
#include "CSIAgent.h"
#include "member.h"
#include "Mysql.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("#commnet_test member_num dotfile\n");
        return -1;
    }

    int member_num = atoi(argv[1]);    // number of member
    char name[16];    // member name

    CSThreadDENet parallelnet(1);    // communication network

    // storage of each member
//    Mysql *mysql[member_num];
//    char db_name[16];

    Member *members[member_num];
    CSIAgent *agents[member_num];
    pthread_t tids[member_num];

    // create and set up each member
    for (int i = 0; i < member_num; i++)
    {
        // storage
//        Mysql *ml = new Mysql();
//        sprintf(db_name, "Member_%d", i);
//        ml->SetDBArgs("localhost", "root", "huangk", db_name);

        // agent
        CSIAgent *agent = new CSIAgent(i + 1, 0.8, 0.01);    // agent id start from 1
//        agent->LoadMemoryFromStorage(ml);

        // avatar
        sprintf(name, "Member_%d", i + 1);
        Member *member = new Member(name);
        member->SetSps(-1);
        member->ConnectAgent(agent);
        member->JoinDENet(&parallelnet);

//        mysql[i] = ml;
        members[i] = member;
        agents[i] = agent;
    }

    // load topo
    parallelnet.LoadTopoFromFile(argv[2]);

    /* launch members */
    for (int i = 0; i < member_num; i++)
        tids[i] = members[i]->ThreadLaunch();    // launch

    // wait
    for (int i = 0; i < member_num; i++)
    {
        pthread_join(tids[i], NULL);

        // all threads quit now
        // save memory to storage
//        agents[i]->DumpMemoryToStorage(mysql[i]);

        // clear
        delete members[i];
        delete agents[i];
//        delete mysql[i];
    }
    // save topo structure
//    commnet.DumpTopoToFile("commnet.dot");
    printf("------- quit!\n");

    return 0;
}

