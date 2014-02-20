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


#include <map>
#include <string>
#include "DotViewer.h"
#include "Agent.h"
#include "Storage.h"

namespace gimcs
{

DotViewer::DotViewer() :
        MemoryViewer(), last_state(INVALID_STATE), last_action(INVALID_ACTION)
{
}

DotViewer::DotViewer(Storage *sg) :
        MemoryViewer(sg), last_state(INVALID_STATE), last_action(INVALID_ACTION)
{
}

DotViewer::~DotViewer()
{
}

void DotViewer::Show()
{
    int re = storage->Connect();
    if (re != 0)    // connect failed
    {
        WARNNING("DotViewer Show(): connect to storage failed!\n");
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
    while (st != INVALID_STATE)    // get state value
    {
        struct State_Info_Header *stif = storage->GetStateInfo(st);
        if (stif != NULL)
        {
            DotStateInfo(stif);
            free(stif);
            st = storage->NextState();
        }
        else
            ERROR("Show(): state: %ld information is NULL!\n", st);
    }
    printf("}\n");    // digraph
    storage->Close();
}

void DotViewer::CleanShow()
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
    while (st != INVALID_STATE)    // get state value
    {
        struct State_Info_Header *stif = storage->GetStateInfo(st);
        if (stif != NULL)
        {
            CleanDotStateInfo(stif);
            free(stif);
            st = storage->NextState();
        }
        else
            ERROR("Show(): state: %ld information is NULL!\n", st);
    }
    printf("}\n");    // digraph
    storage->Close();
}

void DotViewer::DotStateInfo(const struct State_Info_Header *stif) const
{
    /* generated state example:
     *
     * subgraph state9
     * {
     *    rank="same"
     *    st9 [label="9"]
     *    subgraph
     *    {
     *      rank="sink"
     *      node [shape="point"]
     *      eat0in9 [label=""]
     *      eat1in9 [label=""]
     *      eat0in9 -> eat1in9 [style=dashed, dir=none]
     *    }
     *    st9 -> eat0in9 [label="eat: 0 (123)"]
     *    st9 -> eat1in9 [label="eat: 1 (12)"]
     * }
     *
     * eat0in9 -> st8 [label="act: -1 (0.3)"]
     * eat0in9 -> st10 [label="act: 1 (0.4)"]
     *
     */

    if (stif == NULL) return;

    std::string st_color;
    if (stif->st == last_state)
        st_color = "#D3D300";    // highlight last state node
    else
        st_color = "black";

    // store env action and action information
    std::map<Agent::Action, float> actmap;

    unsigned char *p = (unsigned char *) stif;
    printf("\nsubgraph state%ld\n{\n", stif->st);
    printf("rank=\"same\"\n");
    printf("st%ld [label=\"%ld\\n(%.2f)\", color=\"%s\"]\n", stif->st, stif->st,
            stif->payoff, st_color.c_str());
    // env action nodes and env action ---> env action
    printf("subgraph \n{\n");
    printf("rank=\"sink\"\n");    // env nodes should be drawing under state node
    printf("node [shape=\"point\"]\n");
    p += sizeof(struct State_Info_Header);
    struct EnvAction_Info *eaif = (struct EnvAction_Info *) p;
    for (int i = 0; i < stif->eat_num; i++)
    {
        printf("eat%sin%ld [label=\"\", height=0.3]\n",
                Eat2String(eaif[i].eat).c_str(), stif->st);

        int j = i + 1;
        if (j < stif->eat_num)
        {
            printf("eat%sin%ld -> eat%sin%ld [style=dashed, dir=none]\n",
                    Eat2String(eaif[i].eat).c_str(), stif->st,
                    Eat2String(eaif[j].eat).c_str(), stif->st);
        }
    }
    printf("}\n");    // subgraph

    // state ---> env actions
    for (int i = 0; i < stif->eat_num; i++)
    {
        printf(
                "st%ld -> eat%sin%ld [label=<<font color=\"blue\">%ld (%ld)</font>>, color=\"blue\", weight=2.]\n",
                stif->st, Eat2String(eaif[i].eat).c_str(), stif->st,
                eaif[i].eat, eaif[i].count);
    }
    printf("}\n");    // end of subgraph state

    // store actions and their payoff
    int len = stif->eat_num * sizeof(struct EnvAction_Info);
    p += len;
    struct Action_Info *atif = (struct Action_Info *) p;
    for (int i = 0; i < stif->act_num; i++)
    {
        actmap[atif[i].act] = atif[i].payoff;    // act: payoff
    }

    // env actions ---> next states
    len = stif->act_num * sizeof(struct Action_Info);
    p += len;
    struct Forward_Link_Info *lk = (struct Forward_Link_Info *) p;
    for (int i = 0; i < stif->lk_num; i++)
    {
        if (stif->st == last_state && lk[i].act == last_action)    // highlight last action edge
            printf(
                    "eat%sin%ld -> st%ld [label=<<font color=\"#D3D300\">%ld (%.2f)</font>>, color=\"#D3D300\", weight=1.]\n",
                    Eat2String(lk[i].eat).c_str(), stif->st, lk[i].nst,
                    lk[i].act, actmap[lk[i].act]);
        else
            printf(
                    "eat%sin%ld -> st%ld [label=<<font color=\"red\">%ld (%.2f)</font>>, color=\"red\", weight=1.]\n",
                    Eat2String(lk[i].eat).c_str(), stif->st, lk[i].nst,
                    lk[i].act, actmap[lk[i].act]);
    }
}

void DotViewer::CleanDotStateInfo(const struct State_Info_Header *stif) const
{
    /* generated state example:
     *
     * st9 [label="9"]
     *
     * st9 -> st8 [label="<0, -1>"]
     * st9 -> st10 [label="<0, 1>"]
     */
    if (stif == NULL) return;

    printf("\nst%ld [label=\"%ld\"]\n", stif->st, stif->st);
    unsigned char *p = (unsigned char *) stif;
    p += sizeof(struct State_Info_Header);
    int len = stif->eat_num * sizeof(struct EnvAction_Info);
    p += len;    // skip env actions
    len = stif->act_num * sizeof(struct Action_Info);
    p += len;    // skip actions
    struct Forward_Link_Info *lk = (struct Forward_Link_Info *) p;    // reach forward links
    for (int i = 0; i < stif->lk_num; i++)
    {
        printf("st%ld -> st%ld [label=\"<%ld, %ld>\"]\n", stif->st, lk[i].nst,
                lk[i].eat, lk[i].act);
    }
}

const std::string DotViewer::Eat2String(Agent::EnvAction eat) const
{
    char tmp[28];
    if (eat >= 0)
    {
        sprintf(tmp, "%ld", eat);
        std::string str(tmp);
        return str;
    }
    else    // eat < 0, since dot doesn't support minus sign, so we convert '-' to '_'
    {
        sprintf(tmp, "_%ld", -eat);
        std::string str(tmp);
        return str;
    }
}

void DotViewer::ShowState(Agent::State st)
{
    int re = storage->Connect();
    if (re != 0)    // connect failed
    {
        WARNNING("DotViewer ShowState(): connect to storage failed!\n");
        return;
    }

    printf(
            "/* This is the dot file of agent memory automaticlly generated by DotViewer */\n\n");

    // generated dot file example:
    /*
     * digraph Mouse_1 {
     * node [color=black,shape=circle]
     * rank="same"
     * st1 [label="1\n(3.00)"]
     *
     * subgraph {
     * rank="same"
     * eat1in1 [shape="point", label="", height=0.3]
     * eat0in1 [shape="point", label="", height=0.3]
     * eat1in1 -> eat0in1 [style=dashed, dir=none]
     * }
     * st1 -> eat1in1 [label=<<font color="blue">e: 1 (1)</font>>, color="blue", weight=3.]
     * st1 -> eat0in1 [label=<<font color="blue">e: 0 (4)</font>>, color="blue", weight=3.]
     *
     * eat1in1 -> st1 [label=<<font color="red">a: -1 (0.60)</font>>, color="red", weight=1.]
     * eat0in1 -> st2 [label=<<font color="red">a: 1 (2.74)</font>>, color="red", weight=1.]
     * st2 [label="2\n(1.00)"]
     * }
     */

    printf("digraph %s_%ld \n{\n", storage->GetMemoryName().c_str(), st);
    // state info
    printf("node [color=black,shape=circle]\n");
    printf("rank=\"same\"\n");
    printf("label=\"infoset of state %ld in memory %s\"\n", st,
            storage->GetMemoryName().c_str());

    struct State_Info_Header *stif = storage->GetStateInfo(st);
    if (stif != NULL)
    {
        // show state info
        // store env action and action information
        std::map<Agent::Action, float> actmap;

        unsigned char *p = (unsigned char *) stif;
        printf("st%ld [label=\"%ld\\n(%.2f)\"]\n", stif->st, stif->st,
                stif->payoff);
        // env action nodes and env action ---> env action
        printf("\nsubgraph {\n");    // use subgraph to align env action nodes
        printf("rank=\"same\"\n");
        p += sizeof(struct State_Info_Header);
        struct EnvAction_Info *eaif = (struct EnvAction_Info *) p;
        for (int i = 0; i < stif->eat_num; i++)
        {
            printf("eat%sin%ld [shape=\"point\", label=\"\", height=0.3]\n",
                    Eat2String(eaif[i].eat).c_str(), stif->st);

            int j = i + 1;
            if (j < stif->eat_num)
            {
                printf("eat%sin%ld -> eat%sin%ld [style=dashed, dir=none]\n",
                        Eat2String(eaif[i].eat).c_str(), stif->st,
                        Eat2String(eaif[j].eat).c_str(), stif->st);
            }
        }
        printf("}\n");    // subgraph

        // state ---> env actions
        for (int i = 0; i < stif->eat_num; i++)
        {
            printf(
                    "st%ld -> eat%sin%ld [label=<<font color=\"blue\">%ld (%ld)</font>>, color=\"blue\", weight=3.]\n",
                    stif->st, Eat2String(eaif[i].eat).c_str(), stif->st,
                    eaif[i].eat, eaif[i].count);
        }

        // store actions and their payoff
        int len = stif->eat_num * sizeof(struct EnvAction_Info);
        p += len;
        struct Action_Info *atif = (struct Action_Info *) p;
        for (int i = 0; i < stif->act_num; i++)
        {
            actmap[atif[i].act] = atif[i].payoff;    // act: payoff
        }

        // env actions ---> next states
        len = stif->act_num * sizeof(struct Action_Info);
        p += len;
        struct Forward_Link_Info *lk = (struct Forward_Link_Info *) p;
        for (int i = 0; i < stif->lk_num; i++)
        {
            printf(
                    "eat%sin%ld -> st%ld [label=<<font color=\"red\">%ld (%.2f)</font>>, color=\"red\", weight=1.]\n",
                    Eat2String(lk[i].eat).c_str(), stif->st, lk[i].nst,
                    lk[i].act, actmap[lk[i].act]);

            if (lk[i].nst != stif->st)    // get the payoff of next state, exclude self
            {
                struct State_Info_Header *nstif = storage->GetStateInfo(
                        lk[i].nst);
                if (nstif == NULL)    // shouldn't happen
                    ERROR("next state: %ld returns NULL!\n", lk[i].nst);

                printf("st%ld [label=\"%ld\\n(%.2f)\"]\n", lk[i].nst, lk[i].nst,
                        nstif->payoff);    // print out next state
                free(nstif);
            }
        }

        free(stif);    // don't forget to free
    }
    else    // state not found
    {
        printf("state %ld not found in memory!\n", st);
    }
    printf("}\n");    // digraph
    storage->Close();
}

}    // namespace gimcs
