#ifndef PDES_MAS_MBSHAREDSTATE_H
#define PDES_MAS_MBSHAREDSTATE_H

#include <util/Serialisable.h>
#include <state/AbstractValue.h>
#include <state/SsvId.h>
#include <lp/LpId.h>
#include <map>
#include <vector>
#include <lp/AccessCostCalculator.h>
#include <rangequery/RangeRoutingTable.h>
#include "MailboxVariable.h"

using namespace std;
namespace pdesmas {
  class MbSharedState {
  private:
    map<SsvId, MailboxVariable *> MailboxVariableMap;
    map<unsigned long, SsvId> MailboxAgentMap; // map agentid with mailbox variable id
    map<LpId, vector<tuple<unsigned long,  unsigned long >>> MbWriteMap;
    // map<sender,vector(tuple(time,receiver))>
    AccessCostCalculator *ACCalculator;
    map<unsigned long, int> fAgentIdToRankMap;

    bool ContainsVariable(const SsvId &);

  public:
    MbSharedState();

    ~MbSharedState();

    void SetMailboxAgentMap(const map<unsigned long, SsvId> &);

//    vector<tuple<unsigned long, LpId>> GetMsgToRollback(const LpId &, unsigned long);

    void SetAccessCostCalculator(AccessCostCalculator *);

    void UpdateAccessCount(const SsvId &, Direction, unsigned long);

    void Add(const SsvId &pSsvId, const unsigned long pAgentId);

    void Insert(const SsvId &pSsvId, const MailboxVariable &pMbVariable, bool &rb_needed);

    void Delete(const SsvId &);

    MailboxVariable GetCopy(const SsvId &);

    bool MbValid(const unsigned long pOwnerId);

    bool WriteMbMsg(const LpId &pSender, const unsigned long pReceiverId, unsigned long pTime,
                    const AbstractValue *pValue);

    SsvId GetMbvId(const unsigned long pSenderId);

    unsigned long GetOwnerId(const SsvId &pMbvId);

    SerialisableList<MbMail> Read(const unsigned long pOwnerId, unsigned long pTime);

    void RollbackRead(const unsigned long pOwnerId, unsigned long pTime);

    void RollbackWrite(const unsigned long pOwnerId, const LpId &pSender, unsigned long pTime, bool &rb_needed);

    RollbackList GetRollbacklist(const LpId &, unsigned long);

    void RemoveOldMessages(unsigned long pTime); // garbage collection

    void RemoveMessageList(const SsvId &, bool &rb_needed, int& rb_ts);

    int GetRankFromAgentId(unsigned long agentId);

    void SetAgentIdToRankMap(map<unsigned long, int> agentIdToRankMap);
  };
}


#endif
