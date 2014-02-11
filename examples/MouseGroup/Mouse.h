#ifndef MOUSE_H
#define MOUSE_H
#include <vector>
#include <string>
#include "CSThreadAvatar.h"

class Mouse: public CSThreadAvatar
{
    public:
        Mouse(std::string);
        ~Mouse();

    private:
        Agent::State position;
        int count;

        Agent::State GetCurrentState();
        void PerformAction(Agent::Action);
        std::vector<Agent::Action> ActionCandidates(Agent::State);
        float OriginalPayoff(Agent::State);
};

#endif // MOUSE_H
