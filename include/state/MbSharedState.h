#ifndef PDES_MAS_MBSHAREDSTATE_H
#define PDES_MAS_MBSHAREDSTATE_H

#include <util/Serialisable.h>
#include <state/AbstractValue.h>
#include <state/SsvId.h>
#include <lp/LpId.h>
#include <map>
#include <vector>
#include <lp/AccessCostCalculator.h>
#include "MailboxVariable.h"

using namespace std;
namespace pdesmas {
    class MbSharedState {
    private:
        map<SsvId, MailboxVariable> MailboxVariableMap;
        map<LpId, SsvId> MailboxAgentMap; // map agentid with mailbox variable id
        map<LpId, vector<tuple<unsigned long, LpId>>> MbWriteMap;
        RangeRoutingTable *RRTable;
        AccessCostCalculator *ACCalculator;

        bool ContainsVariable(const SsvId &);

    public:
        MbSharedState();

        ~MbSharedState();

        vector<tuple<unsigned long, LpId>> GetMsgToRollback(const LpId &, unsigned long);

        void SetRangeRoutingTable(RangeRoutingTable *);

        void SetAccessCostCalculator(AccessCostCalculator *);

        void UpdateAccessCount(const SsvId &, Direction, unsigned long);

        void Add(const SsvId &, unsigned long, const LpId &);

        void Insert(const SsvId &, const MailboxVariable &, RollbackList &);

        void Delete(const SsvId &);

        MailboxVariable GetCopy(const SsvId &);

        bool MbValid(const LpId &);

        bool WriteMbMsg(const LpId &, const LpId &, unsigned long, const AbstractValue*);

        SsvId GetMbvId(const LpId &);

        AbstractValue *Read(const LpId &, unsigned long);

        void RollbackRead(const LpId&, unsigned long, RollbackList);

        void RollbackWrite();

        RollbackList GetRollbacklist(const LpId&, unsigned long);

        void RemoveMbMessages(unsigned long);

        void RemoveMessageList(const SsvId &, RollbackList &);
    };
}


#endif
