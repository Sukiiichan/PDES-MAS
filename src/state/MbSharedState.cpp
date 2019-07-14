#include <rangequery/RangeRoutingTable.h>
#include "MbSharedState.h"

using namespace pdesmas;

MbSharedState::MbSharedState() {
    RRTable = NULL;
    ACCalculator = NULL;
}

MbSharedState::~MbSharedState() {}

void MbSharedState::SetRangeRoutingTable(RangeRoutingTable *pRRTable) {
    RRTable = pRRTable;
}

void MbSharedState::SetAccessCostCalculator(AccessCostCalculator *pACCalculator) {
    ACCalculator = pACCalculator;
}

void MbSharedState::UpdateAccessCount(const SsvId &pSsvId, Direction pDirection, unsigned long numberOfHops) {
    unsigned long access, hops;
    access = ACCalculator->UpdateAccessCount(pDirection, 1, pSsvId);
    hops = ACCalculator->UpdateHopCount(pDirection, numberOfHops, pSsvId);
    ACCalculator->UpdateLoad(hops, access, numberOfHops + hops, access + 1);
}

bool MbSharedState::ContainsVariable(const SsvId &pSsvId) {
    return (MailboxVariableMap.count(pSsvId) > 0);
}


void MbSharedState::Add(const SsvId &pSsvId, unsigned long pTime, const LpId &pAgent) {
    if (ContainsVariable(pSsvId)) {
        LOG(logERROR) << "";
        exit(1);
    }
    MailboxVariable newMailboxVariable(pSsvId, pAgent);
    MailboxVariableMap[pSsvId] = newMailboxVariable;
    MailboxAgentMap[pSsvId] = pAgent;
}

void MbSharedState::Insert(const SsvId &pSsvId, const MailboxVariable &pMbVariable, RollbackList &pRbList) {
    if (ContainsVariable(pSsvId)){
        LOG(logERROR) << "";
        exit(1);
    }
    const LpId &pAgent = pMbVariable.GetOwnerAgent();
    MailboxVariableMap[pSsvId] = MailboxVariable(pSsvId, pAgent);
    // MAMap del old
}

void MbSharedState::Delete(const SsvId &) {}

MailboxVariable MbSharedState::GetCopy(const SsvId &) {}

AbstractValue *MbSharedState::Read(const SsvId &, const LpId &, unsigned long) {}

void MbSharedState::RemoveMbMessages(unsigned long) {}

void MbSharedState::RemoveMessageList(const SsvId &, RollbackList &) {}
