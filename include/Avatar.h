#ifndef AVATAR_H
#define AVATAR_H
#include <stdlib.h>
#include "Agent.h"
#include "CCCNet.h"

/**
* Avatar Interface.
* An avatar is an agent given meanings.
*/
class Avatar
{
    public:
        Avatar();
        Avatar(int);
        virtual ~Avatar();

        void Launch();     /**< launch this avatar */
        pthread_t ThreadLaunch();      /**< if it's a group, launch each of its  members in a thread */

        void ConnectAgent(Agent *);     /**< connect to an agent */
        void SetCCCNet(CCCNet *);        /**< set which communication network this avatar is belonged to */
        void SetFreq(int);              /**< set frequence of communication with neighbours */

        static bool quit;               /**< tell all Avatar instances to quit, it's class scope */
    protected:
        int id;         /**< avatar Id */
        int freq;       /**< communication frequence */
        Agent *agent;   /**< connected agent */
        CCCNet *cccnet;   /**< my group */

        virtual Agent::Agent::State GetCurrentState() = 0;    /**< get current Agent::State */
        virtual void DoAction(Agent::Action) = 0;      /**< perform an Agent::Action */
        virtual std::vector<Agent::Action> ActionCandidates(Agent::State) = 0;   /**< return a list of all Agent::Action candidates of a Agent::State */
        virtual float OriginalPayoff(Agent::State);    /**< original payoff of a Agent::State */

        virtual void SendStateInfo(Agent::State);      /**< send information of a Agent::State to all its neighbours */
        virtual void RecvStateInfo();           /**< recieve Agent::State information from neighbours */

        static void* hook(void* args) {         /**< hook to run a class function(Launch() here) in a thread */
            reinterpret_cast<Avatar *>(args)->Launch();
            return NULL;
        }
    private:
};

#endif // AVATAR_H
