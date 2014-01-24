#ifndef CSAGENT_H
#define CSAGENT_H

#include <unordered_map>
#include <string>
#include <mysql/mysql.h>
#include "Agent.h"

class Storage;

/**
* Computer Simulation of Agent, which uses computers to implement an agent.
*/
class CSAgent : public Agent
{
public:
    typedef std::unordered_map<Agent::State, void *> StatesMap; /**< hash map from state value to state struct */
    enum m_StMark {SAVED, NEW, MODIFIED};   /**< storage status of a state */

    CSAgent();
    CSAgent(float, float);
    virtual ~CSAgent();

    struct State_Info_Header *GetStateInfo(Agent::State) const;                 /**< implementing GetStateInfo function */
    void MergeStateInfo(const struct State_Info_Header *);               /**< implementing MergeStateInfo function */
    static void PrintStateInfo(const struct State_Info_Header *);         /**< print state information gracefully */
    void SetDBArgs(std::string, std::string, std::string, std::string);                 /**< set database related arguments */
    void InitMemory();              /**< load memory from database */
    void SaveMemory();              /**< save memory to database */

    void SetStorage(Storage *);     /**< set storage device */

private:
    unsigned long state_num;                  /**< total number of states in memory */
    unsigned long lk_num;                    /**< total number of links between states in memory */

    Storage *storage;   /**< dump memroy to a mass storage device */

    std::vector<Agent::Action> MaxPayoffRule(Agent::State, const std::vector<Agent::Action> &);    /**< implementing maximun payoff rule */
    void UpdateMemory(float);            /**< implementing UpdateMemory of Agent */

    void PrintProcess(unsigned long, unsigned long, char *) const;

    struct m_State *head;           /**< memory head*/
    StatesMap states_map;           /**< hash map from state values to state struct */
    struct m_State *cur_mst;        /**< state struct for current state value */

    void LoadState(Agent::State);               /**< fetch state struct by state value */

    void FreeMemory();              /**< free all space of memory in computer memory*/

    void RemoveState(struct m_State *);             /**< remove "root" state */

    void LinkStates(struct m_State *, Agent::EnvAction, Agent::Action, struct m_State *);  /**< link two states in memory with specfic exact and action */
    std::vector<Agent::Action> BestActions(const struct m_State *, const std::vector<Agent::Action>&);           /**< find the best action of a state */
    struct m_State *SearchState(Agent::State) const;             /**< search state in memory by its identity */
    void UpdateState(struct m_State *);             /**< update state payoff backward recursively */

    struct m_State *NewState(Agent::State);                /**< create a new state struct in memory */
    struct m_EnvAction *NewEa(Agent::EnvAction);
    struct m_Action *NewAc(Agent::Action);
    struct m_ForwardArcState *NewFas(Agent::EnvAction, Agent::Action);
    struct m_BackArcState *NewBas();
    void FreeState(struct m_State *);               /**< free a state struct */
    void FreeEa(struct m_EnvAction *);
    void FreeAc(struct m_Action *);
    void FreeFas(struct m_ForwardArcState *);
    void FreeBas(struct m_BackArcState *);
    void AddStateToMemory(struct m_State *);    /**< add a state struct to memory */

    struct m_Action *Act2Struct(Agent::Action, const struct m_State *) const;                  /**< find the Agent::Action struct address according to identity */
    struct m_EnvAction *Eat2Struct(Agent::EnvAction, const struct m_State *) const;              /**< find the exact strut address according to identity */
    struct m_State *StateByEatAct(Agent::EnvAction, Agent::Action, const struct m_State *) const;      /**< find the following state according to exact and Agent::Action*/
    float MaxPayoffInEat(Agent::EnvAction, const struct m_State *) const;                       /**< maximun payoff of all following states under a specfic exact */
    float Prob(const struct m_EnvAction*, const struct m_State *) const;                       /**< probability of a exact */

    float CalStatePayoff(const struct m_State *) const;             /**< calculate payoff of a state */
    float CalActPayoff(Agent::Action, const struct m_State *) const;       /**< calculate payoff of an Agent::Action */
};

inline void CSAgent::SetStorage(Storage *stg)
{
    storage = stg;
}

/** implementation of environment Agent::Action information */
struct m_EnvAction
{
    Agent::EnvAction eat;               /**< eact value */
    unsigned long count;        /**< eact count */
    struct m_EnvAction *next;    /**< next struct */
};

/** implementation of Agent::Action information */
struct m_Action
{
    Agent::Action act;                 /**< Agent::Action value */
    float payoff;               /**< Agent::Action payoff */
    struct m_Action *next;
};

/** implementation of forward link information */
struct m_ForwardArcState
{
    Agent::EnvAction eat;               /**< exact */
    Agent::Action act;                 /**< Agent::Action */
    struct m_State *nstate;     /**< following state */
    struct m_ForwardArcState *next;
};

/** implementation of backward link information */
struct m_BackArcState
{
    struct m_State *pstate;     /**< previous state */
    struct m_BackArcState *next;
};

/** state information */
struct m_State
{
    Agent::State st;                   /**< state value */
    float payoff;               /**< state payoff */
    float original_payoff;      /**< original payoff of state */
    unsigned long count;        /**< state count */
    enum CSAgent::m_StMark mark;         /**< mark used for storage */
    struct m_EnvAction *ealist;  /**< exacts of this state */
    struct m_Action *atlist;    /**< actions of this state */
    struct m_ForwardArcState *flist;    /**< forward links */
    struct m_BackArcState *blist;       /**< backward links */
    struct m_State *next;
};

#endif // CSAGENT_H
