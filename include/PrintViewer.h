/*
 * PrintVewer.h
 *
 *  Created on: Feb 7, 2014
 *      Author: andy
 */

#ifndef PRINTVEWER_H_
#define PRINTVEWER_H_
#include <MemoryViewer.h>
#include "Agent.h"

namespace gimcs
{

class Storage;

/**
 * Visualizing memory in print format.
 */
class PrintViewer: public MemoryViewer
{
    public:
        PrintViewer();
        PrintViewer(Storage *);
        virtual ~PrintViewer();

        void Show();
        void ShowState(Agent::State);
    private:
        void PrintStateInfo(const struct State_Info_Header *) const;
};

}    // namespace gimcs
#endif /* PRINTVEWER_H_ */
