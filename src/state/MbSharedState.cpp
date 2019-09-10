#include <rangequery/RangeRoutingTable.h>
#include "MbSharedState.h"
#include "MailboxVariable.h"

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

bool MbSharedState::MbValid(const LpId &pLpId) {
   return (MailboxAgentMap.count(pLpId) > 0);
}

SsvId MbSharedState::GetMbvId(const LpId &pSender) {
   return MailboxAgentMap.find(pSender)->second;
}

bool MbSharedState::WriteMbMsg(const LpId &pSender, const LpId &pReceiver, unsigned long pTime,
                               const AbstractValue *pValue) {
   SsvId dstId = MailboxAgentMap.find(pReceiver)->second;
   MailboxVariable dstMbv = MailboxVariableMap.find(dstId)->second;
   if (dstMbv.AddMbMessage(pValue, pTime, pSender)) {
      auto mbWriteMapIterator = MbWriteMap.find(pSender);
      mbWriteMapIterator->second.emplace_back(pTime, pReceiver);
      // TODO may need to sort em
   }
   return false;
}

void MbSharedState::Add(const SsvId &pSsvId, unsigned long pTime, const LpId &pAgent) {
   if (ContainsVariable(pSsvId)) {
      LOG(logERROR) << "";
      exit(1);
   }
   MailboxVariable newMailboxVariable(pSsvId, pAgent);
   MailboxVariableMap[pSsvId] = newMailboxVariable;
   MailboxAgentMap[pAgent] = pSsvId;
}

void MbSharedState::Insert(const SsvId &pSsvId, const MailboxVariable &pMbVariable, RollbackList &pRbList) {
   if (ContainsVariable(pSsvId)) {
      LOG(logERROR) << "";
      exit(1);
   }
   const LpId &pAgent = pMbVariable.GetOwnerAgent();
   MailboxVariableMap[pSsvId] = MailboxVariable(pSsvId, pAgent)
   // TODO MAMap del old
}

void MbSharedState::Delete(const SsvId &) {}

MailboxVariable MbSharedState::GetCopy(const SsvId &) {}

AbstractValue *MbSharedState::Read(const LpId &pAgent, unsigned long pTime) {
   // unsigned long agentId = pAgent.GetId();
   SsvId fSsvId = MailboxAgentMap.find(pAgent)->second;
   return MailboxVariableMap.find(fSsvId)->second.ReadMb(pAgent, pTime);
}

void MbSharedState::RemoveMbMessages(unsigned long) {}

void MbSharedState::RemoveMessageList(const SsvId &, RollbackList &) {}

vector<tuple<unsigned long, LpId>>MbSharedState::GetMsgToRollback(const LpId &pAgent, unsigned long readUntil) {
   // TODO return msg and agent influenced by rollback
   auto mbWriteMapIterator = MbWriteMap.find(pAgent);
   list<tuple<unsigned long, LpId>> result;

   auto writeIterator = mbWriteMapIterator->second.begin();
   while(writeIterator!=mbWriteMapIterator->second.end()){
      if(get<0>(*writeIterator) > readUntil){
         result.push_back(*writeIterator);
      }
      writeIterator ++;
   }
}