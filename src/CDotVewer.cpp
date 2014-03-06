/*
 * CDotVewer.cpp
 *
 *  Created on: Feb 21, 2014
 *      Author: andy
 */

#include "gimcs/CDotViewer.h"
#include "gimcs/Agent.h"
#include "gimcs/Storage.h"
#include "gimcs/StateInfoParser.h"

namespace gimcs
{

CDotViewer::CDotViewer() :
        last_state(Agent::INVALID_STATE), last_action(Agent::INVALID_ACTION)
{
}

CDotViewer::CDotViewer(Storage *sg) :
        DotViewer(sg), last_state(Agent::INVALID_STATE), last_action(Agent::INVALID_ACTION)
{
}

CDotViewer::~CDotViewer()
{
}

void CDotViewer::Show()
{
    int re = storage->Connect();
    if (re != 0)    // connect failed
    {
        WARNNING("DotViewer CleanShow(): connect to storage failed!\n");
        return;
    }

    printf(
            "/* This is the dot file of agent memory automaticlly generated by DotViewer */\n\n");

    // generate dot syntax
    printf("digraph %s \n{\n", storage->GetMemoryName().c_str());

    // memory info
    struct Memory_Info *memif = storage->GetMemoryInfo();
    if (memif != NULL)
    {
        printf(
                "label=\"memory %s\\ndiscount rate: %.2f, threshold: %.2f, #states: %ld, #links: %ld\"\n",
                storage->GetMemoryName().c_str(), memif->discount_rate,
                memif->threshold, memif->state_num, memif->lk_num);
        // store last status
        last_state = memif->last_st;
        last_action = memif->last_act;
        free(memif);    // free it, the memory struct are not a substaintial struct for running, it's just used to store meta-memory information
    }
    else
    {
        printf("Memory not found in storage!\n");
        printf("}\n");    // digraph
        storage->Close();
        return;
    }

    // states info
    printf("node [color=black,shape=circle]\n");
    printf("rank=\"same\"\n");
    // print states info
    Agent::State st = storage->FirstState();
    while (st != Agent::INVALID_STATE)    // get state value
    {
        struct State_Info_Header *stif = storage->GetStateInfo(st);
        if (stif != NULL)
        {
            CleanDotStateInfo(stif);
            free(stif);
            st = storage->NextState();
        }
        else
            ERROR("Show(): state: %" ST_FMT " information is NULL!\n", st);
    }
    printf("}\n");    // digraph
    storage->Close();
}

void CDotViewer::CleanDotStateInfo(const struct State_Info_Header *sthd) const
{
    /* generated state example:
     *
     * st9 [label="9"]
     *
     * st9 -> st8 [label="<0, -1>"]
     * st9 -> st10 [label="<0, 1>"]
     */
    if (sthd == NULL) return;

    printf("\nst%" ST_FMT " [label=\"%" ST_FMT "\"]\n", sthd->st, sthd->st);

    StateInfoParser sparser(sthd);
    Action_Info_Header *athd = NULL;
    EnvAction_Info *eaif = NULL;

    athd = sparser.FirstAct();
    while (athd != NULL)
    {
        eaif = sparser.FirstEat();
        while (eaif != NULL)
        {
            printf("st%" ST_FMT " -> st%" ST_FMT " [label=\"<%" ACT_FMT ", %" ACT_FMT ">\"]\n", sthd->st,
                    eaif->nst, athd->act, eaif->eat);

            eaif = sparser.NextEat();
        }

        athd = sparser.NextAct();
    }
}

} /* namespace gimcs */
