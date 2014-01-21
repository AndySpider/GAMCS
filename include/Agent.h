#ifndef AGENT_H
#define AGENT_H

#include <stddef.h>
#include "TSGIOM.h"

const unsigned long INVALID_STATE = INVALID_INPUT;
const long INVALID_ACTION = INVALID_OUTPUT;

/**
 * Interface of an autonomous agent.
 */
class Agent: public TSGIOM
{
    public:
        typedef GIOM::Input State; /**< for agent we call an input as a state */
        typedef GIOM::Output Action; /**< action as output */
        typedef unsigned long EnvAction; /**< environment action, "exact" for short */

        Agent();
        Agent(float, float);

        virtual ~Agent();

        /** These two functions are implementation dependant, declared as pure virtual functions */
        virtual struct State_Info_Header *GetStateInfo(State) const = 0; /**<  collect information of specified state from memory */
        virtual int MergeStateInfo(const struct State_Info_Header *) = 0; /**<  merge recieved state information into memory */
        void Update(float); /**< update memory, this function will call UpdateMemory() to do the real update */

        /* set and get functions */
        void SetDiscountRate(float);
        float GetDiscountRate();
        void SetThreshold(float);
        float GetThreshold();
    protected:
        float discount_rate; /**< discount rate (0<,<1)when calculate state payoff */
        float threshold; /**< threshold used in payoff updating */

        std::vector<Action> Restrict(State, const std::vector<Action> &); /**< reimplement restrict using maximun payoff rule  */

        virtual std::vector<Action> MaxPayoffRule(State,
                const std::vector<Action> &) = 0; /**< implementation of maximun payoff rule */
        virtual void UpdateMemory(float) = 0; /**<  update states in memory given current state's original payoff*/
};

/** backward link struct for a state
 *  pst + pact + peat = current state
 */
struct BackLink
{
        Agent::State pst; /**< previous state */
        Agent::Action pact; /**< previous action */
        Agent::EnvAction peat; /**< previous exact */
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
        int act_num; /**< number of actions which have been performed */
        int eat_num; /**< number of environment actions which have been observed */
        int lk_num; /**< number of links to other states */
        size_t size; /**< size of the header (in Byte) */
};

/** memory information */
struct Memory_Info
{
    float discount_rate;    /**< discount rate */
    float threshold;        /**< threshold */
    unsigned long state_num;    /**< total number of states in memroy */
    unsigned long lk_num;       /**< total number of links between states in memory */
    Agent::State last_st;      /**< last experienced state when saving memory */
    Agent::Action last_act;    /**< last performed Agent::Action when saving memory */
};

#endif // AGENT_H
