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

bool MbSharedState::MbValid(const unsigned long pOwnerId) {
  return (MailboxAgentMap.count(pOwnerId) > 0);
}

SsvId MbSharedState::GetMbvId(const unsigned long pSenderId) {
  return MailboxAgentMap.find(pSenderId)->second;
}

bool MbSharedState::WriteMbMsg(const LpId &pSender, const unsigned long pReceiverId, unsigned long pTime,
                               const AbstractValue *pValue) {
  //spdlog::debug("Total num of mbv: {}",MailboxVariableMap.size());
  SsvId dstId = MailboxAgentMap.find(pReceiverId)->second;
  MailboxVariable *dstMbv = MailboxVariableMap.find(dstId)->second;
  assert(MailboxVariableMap.find(dstId) != MailboxVariableMap.end());
  // TODO see the variable in map
  //spdlog::debug("prepare to add msg to {0}", dstId.id());
  // bool rollback_flag = dstMbv->AddMbMessage(pValue, pTime, pSender);
  return dstMbv->AddMbMessage(pValue, pTime, pSender);

}
//    auto mbWriteMapIter = MbWriteMap.find(pSender);
//
//    if (mbWriteMapIter == MbWriteMap.end()) {
//      std::ostringstream out;
//      pValue->Serialise(out);
//      spdlog::critical("Can't find agent in mbWriteMap: function call was: MbSharedState::WriteMbMsg(({},{}),{},{},{})",
//                    pSender.GetId(), pSender.GetRank(), pReceiverId, pTime, out.str());
//      exit(1);
//    }
//    for (auto recordIterator = mbWriteMapIter->second.begin();
//         recordIterator != mbWriteMapIter->second.end(); recordIterator++) {
//      if (get<0>(*recordIterator) > pTime) {
//        mbWriteMapIter->second.insert(recordIterator, make_tuple(pTime, pReceiverId));
//      }
//      // mbWriteMapIter.emplace_back(pTime, pReceiver);
//      // find the nearest log to pTime
//    }
// }


void MbSharedState::Add(const SsvId &pSsvId, const unsigned long pAgentId) {
  if (ContainsVariable(pSsvId)) {
    spdlog::critical("MbSharedState::Add({0},{1}): Already contains {0}", pSsvId.id(), pAgentId);
    exit(1);
  }

  MailboxVariableMap[pSsvId] = new MailboxVariable(pSsvId, pAgentId);
  MailboxAgentMap[pAgentId] = pSsvId;
#ifdef SSV_LOCALISATION
  spdlog::debug("MbSharedState::Add({},{})", pSsvId.id(), pAgentId);
  ACCalculator->InitialiseCounters(pSsvId);

#endif
}

void MbSharedState::Insert(const SsvId &pSsvId, const MailboxVariable &pMbVariable, RollbackList &pRbList) {
  if (ContainsVariable(pSsvId)) {
    LOG(logERROR) << "";
    exit(1);
  }
  const unsigned long pAgentId = pMbVariable.GetOwnerAgentId();
  MailboxVariableMap[pSsvId] = new MailboxVariable(pSsvId, pAgentId);
#ifdef SSV_LOCALISATION
  ACCalculator->InitialiseCounters(pSsvId);

#endif
}

void MbSharedState::Delete(const SsvId &) {}


MailboxVariable MbSharedState::GetCopy(const SsvId &) {
}

SerialisableList<MbMail> MbSharedState::Read(const unsigned long pOwnerId, unsigned long pTime) {
  // unsigned long agentId = pAgent.GetId();
  SsvId fSsvId = MailboxAgentMap.find(pOwnerId)->second;
  return MailboxVariableMap.find(fSsvId)->second->ReadMb(pOwnerId, pTime);
}

void MbSharedState::RollbackRead(const unsigned long pOwnerId, unsigned long pTime) {
  // rollbacks reads that shouldnt happen
  SsvId mbvId = MailboxAgentMap.find(pOwnerId)->second;
  MailboxVariable *mbv = MailboxVariableMap.find(mbvId)->second;
  mbv->PeformReadAnti(pOwnerId, pTime);
}


void
MbSharedState::RollbackWrite(const unsigned long pOwnerId, const LpId &pSender, unsigned long pTime, bool &rb_needed) {
  SsvId mbvId = MailboxAgentMap.find(pOwnerId)->second;
  MailboxVariable *mbv = MailboxVariableMap[mbvId];
  // unsigned long readUntil = MailboxVariableMap.find(mbvId)->second->GetReadUntil();

  // two situations here
  // one: the message haven't been read yet
  // just delete that message in our statebase
  // two: the message has been read
  // then delete it from statebase
  // and roll back the owner (add it into the rollbacklist)

  mbv->PerformWriteAnti(pSender, pTime, rb_needed);

}

//void MbSharedState::RemoveMessageList(const SsvId &, RollbackList &) {}
//
//vector<tuple<unsigned long, LpId>> MbSharedState::GetMsgToRollback(const LpId &pAgent, unsigned long readUntil) {
//   auto mbWriteMapIterator = MbWriteMap.find(pAgent);
//   list<tuple<unsigned long, LpId>> result;
//
//   auto writeIterator = mbWriteMapIterator->second.begin();
//   while (writeIterator != mbWriteMapIterator->second.end()) {
//      if (get<0>(*writeIterator) > readUntil) {
//         result.push_back(*writeIterator);
//      }
//      writeIterator++;
//   }
//}

int MbSharedState::GetRankFromAgentId(unsigned long agentId) {
  return fAgentIdToRankMap[agentId];
}

//RollbackList MbSharedState::GetRollbacklist(const LpId &pOwner, unsigned long pTime) {
//   RollbackList rollbackList;
//   SsvId mbvId = MailboxAgentMap.find(pOwner)->second;
//   MailboxVariable dstMbv = MailboxVariableMap.find(mbvId)->second;
//   vector<pair<LpId, unsigned long>> RbRecordList;
//   RbRecordList = dstMbv.GetRbList(pTime);
//
//   auto RecordListIterator = RbRecordList.begin();
//   while (RecordListIterator != RbRecordList.end()) {
//      rollbackList.AddLp(RecordListIterator->first, RecordListIterator->second);
//      RecordListIterator++;
//   }
//
//   return rollbackList;
//}

void MbSharedState::SetMailboxAgentMap(const map<unsigned long, SsvId> &maMap) {
  MailboxAgentMap = maMap;
}

void MbSharedState::RemoveOldMessages(unsigned long agentId, unsigned long pTime) {
  SsvId mbvId = MailboxAgentMap.find(agentId)->second;
  MailboxVariable *mbv = MailboxVariableMap[mbvId];
  mbv->RemoveOldMessage(pTime);
}

void MbSharedState::SetAgentIdToRankMap(map<unsigned long, int> agentIdToRankMap) {
  this->fAgentIdToRankMap = agentIdToRankMap;
}
