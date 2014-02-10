#ifndef AGENT_H
#define AGENT_H
#include <stddef.h>
#include <set>
#include "TSGIOM.h"
#include "Debug.h"

const unsigned long INVALID_STATE = INVALID_INPUT;
const long INVALID_ACTION = INVALID_OUTPUT;

class CommNet;

/**
 * Interface of an autonomous agent.
 */
class Agent: public TSGIOM
{
    public:
        typedef GIOM::Input State; /**< for agent we call an input as a state */
        typedef GIOM::Output Action; /**< action as output */
        typedef long EnvAction; /**< environment action, "exact" for short */

        Agent();
        Agent(int);
        Agent(int, float, float);
        virtual ~Agent();

        void Update(float); /**< update memory, this function will call UpdateMemory() to do the real update */
        void Communicate(); /**< share memory with others */

        static void PrintStateInfo(const struct State_Info_Header *); /**< print state information gracefully */
        /* vector and get functions */
        void SetDiscountRate(float);
        float GetDiscountRate();
        void SetThreshold(float);
        float GetThreshold();
        void SetUnseenStatePayoff(float);
        void SetUnseenActionPayoff(float);
        void SetUnseenEActionMaxpayoff(float);

        /* network related stuff */
        void JoinCommNet(CommNet *); /**< set join a communication network */
        void LeaveCommNet(); /**< leave network */
        void AddNeighbour(int, int); /**< add a neighbour */
        void ChangeNeighCommInterval(int, int); /**< change the interval to communicate with a neighbour */
        void RemoveNeighbour(int); /**< remove a neighbour */

    protected:
        int id; /**< agent Id */
        float discount_rate; /**< discount rate (0<,<1)when calculate state payoff */
        float threshold; /**< threshold used in payoff updating */

        float unseen_state_payoff; /**< payoff of unseen states */
        float unseen_action_payoff; /**< payoff of unseen actions, this controls the agent's curiosity */
        float unseen_eaction_maxpayoff; /**< maxmun payoff of states belonging to an unseen environment action, this controls the agent's world view */

        CommNet *commnet; /**< which network this agent is belonged to */

        std::vector<Action> Restrict(State, const std::vector<Action> &); /**< reimplement restrict using maximun payoff rule  */

        /** These two functions are implementation dependant, declared as pure virtual functions */
        virtual std::vector<Action> MaxPayoffRule(State,
                const std::vector<Action> &) = 0; /**< implementation of maximun payoff rule */
        virtual void UpdateMemory(float) = 0; /**<  update states in memory given current state's original payoff*/
        virtual struct State_Info_Header *GetStateInfo(State) const = 0; /**<  collect information of specified state from memory */
        virtual void MergeStateInfo(const struct State_Info_Header *) = 0; /**<  merge recieved state information into memory */
        virtual State NextStateToSend(int) = 0; /**< return the next state to be sent to a specified neighbour */

        std::set<int> GetMyNeighbours();
        bool CheckNeighbourShip(int);
        int GetNeighCommInterval(int); /**< get interval to comminucate with this neighbour */

        void SendStateInfo(int, State); /**< send information of a state to a neighbour */
        void RecvStateInfo(); /**< recieve state information from neighbours */
};

inline void Agent::SetDiscountRate(float dr)
{
    // check validity
    if (dr >= 1.0 || dr < 0)    // discount rate range [0, 1)
        ERROR(
                "Agent - discount rate must be bigger than 0 and smaller than 1.0!\n");

    discount_rate = dr;
}

inline float Agent::GetDiscountRate()
{
    return discount_rate;
}

inline void Agent::SetThreshold(float th)
{
    if (th < 0)
    ERROR("Agent - threshold must be bigger than 0!\n");

    threshold = th;
}

inline float Agent::GetThreshold()
{
    return threshold;
}

inline void Agent::SetUnseenStatePayoff(float pf)
{
    unseen_state_payoff = pf;
}

inline void Agent::SetUnseenActionPayoff(float pf)
{
    unseen_action_payoff = pf;
}

inline void Agent::SetUnseenEActionMaxpayoff(float pf)
{
    unseen_eaction_maxpayoff = pf;
}

/** forward link struct for a state
 *  current state + act + eat = nst
 */
struct Forward_Link
{
        Agent::Action act; /**< action */
        Agent::EnvAction eat; /**< env act */
        Agent::State nst; /**< next state */
};

/** action information */
struct Action_Info
{
        Agent::Action act; /**< action value */
        float payoff; /**< action payoff  */
};

/** environment act information */
struct EnvAction_Info
{
        Agent::EnvAction eat; /**< environment action value */
        unsigned long count; /**< count of experiencing times */
};

/** Header of state information, this struct can be used to communicate with other agents */
struct State_Info_Header
{
        Agent::State st; /**< state value */
        float original_payoff; /**< original payoff */
        float payoff; /**< payoff */
        unsigned long count; /**< times of travelling through this state */
        int eat_num; /**< number of environment actions which have been observed */
        int act_num; /**< number of actions which have been performed */
        int lk_num; /**< number of forward links to other states */
        size_t size; /**< size of the header (in Byte) */
};

/** memory information */
struct Memory_Info
{
        float discount_rate; /**< discount rate */
        float threshold; /**< threshold */
        unsigned long state_num; /**< total number of states in memroy */
        unsigned long lk_num; /**< total number of links between states in memory */
        Agent::State last_st; /**< last experienced state when saving memory */
        Agent::Action last_act; /**< last performed Agent::Action when saving memory */
};

#endif // AGENT_H
