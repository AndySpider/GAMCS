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
// Created on: Feb 17, 2014
//
// -----------------------------------------------------------------------------


#ifndef CSTHREADEXNET_H_
#define CSTHREADEXNET_H_
#include <set>
#include "gimcs/ExNet.h"

namespace gimcs
{

/**
 * Channel used to transfer messages, every member has a channel.
 */
struct Channel
{
        pthread_mutex_t mutex;    // lock the channel when use it
        int ptr;    // point of current message in message pool
        int msg_num;    // number of messages in this channel
        struct Msg *msg;    // message pool
};

/**
 *
 */
class CSThreadExNet: public ExNet
{
    public:
        enum
        {
            MSG_POOL_SIZE = 100
        };

        enum
        {
            MAX_MEMBER = 1000
        };

        enum
        {
            DATA_SIZE = 4096
        };

        CSThreadExNet();
        CSThreadExNet(int);
        virtual ~CSThreadExNet();

        void loadTopoFromFile(char *); /**< load topological structure of network from a file */
        void dumpTopoToFile(char *) const; /**< dump topological structure of network to file */

    private:
        void addMember(int);
        void addNeighbour(int, int);
        void removeMember(int);
        void removeNeighbour(int, int);

        std::set<int> getNeighbours(int) const;
        std::set<int> getAllMembers() const;
        bool checkNeighbourShip(int, int) const;

        int send(int, int, void *, size_t); /**< the interface members can use to send messages to a neighbour */
        int recv(int, int, void *, size_t); /**< the interface members can use to recv message from a neighbour */

        struct Channel *getChannel(int); /**< get channel of a specified member */
        void notify(int); /**< notify a member of new messages recieved */

        int wrapInc(int); /**< increase message point, when reach MSG_POOL_SIZE wrap from 0 */
        int wrapDec(int); /**< decrease message point, when reach 0 wrap from MSG_POOL_SIZE */

        std::set<int> members; /**< save all members */
        struct Channel channels[MAX_MEMBER]; /**< channels for all members */
        struct Neigh *neighlist[MAX_MEMBER]; /**< neighbour list for all members */
};

/**
 * \brief Get one's channel.
 * \param id member id
 * \return channel address
 */
inline struct Channel *CSThreadExNet::getChannel(int id)
{
    return &channels[id];
}

inline std::set<int> CSThreadExNet::getAllMembers() const
{
    return members;
}

/**
 * Message transfered between members.
 */
struct Msg
{
        int sender_id;
        char data[CSThreadExNet::DATA_SIZE];    // message body
};

/**
 * Neightbour list of a member
 */
struct Neigh
{
        int id; /**< the neigh's ID */
        struct Neigh *next; /**< next neigh */
};

}    // namespace gimcs
#endif /* CSTHREADEXNET_H_ */
