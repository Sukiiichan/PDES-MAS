#ifndef PDES_MAS_MBSHAREDSTATE_H
#define PDES_MAS_MBSHAREDSTATE_H

#include <util/Serialisable.h>
#include <state/AbstractValue.h>
#include <state/SsvId.h>
#include <lp/LpId.h>
#include <map>
#include <lp/AccessCostCalculator.h>
#include "MailboxVariable.h"

using namespace std;
namespace pdesmas {
    class MbSharedState {
    private:
        map<SsvId, MailboxVariable> MailboxVariableMap;
        map<SsvId, LpId > MailboxAgentMap; // map agentid with mailbox variable id
        RangeRoutingTable *RRTable;
        AccessCostCalculator *ACCalculator;

        bool ContainsVariable(const SsvId &);

    public:
        MbSharedState();

        ~MbSharedState();

        void SetRangeRoutingTable(RangeRoutingTable *);

        void SetAccessCostCalculator(AccessCostCalculator *);

        void UpdateAccessCount(const SsvId &, Direction, unsigned long);

        void Add(const SsvId &, unsigned long, const LpId &);

        void Insert(const SsvId &, const MailboxVariable &, RollbackList &);

        void Delete(const SsvId &);

        MailboxVariable GetCopy(const SsvId &);


        AbstractValue *Read(const SsvId &, const LpId &, unsigned long);

        void RemoveMbMessages(unsigned long);

        void RemoveMessageList(const SsvId &, RollbackList &);
    };
}


#endif
