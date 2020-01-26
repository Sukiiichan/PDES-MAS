#include <rangequery/RangeRoutingTable.h>
#include "MbSharedState.h"
#include "MailboxVariable.h"

using namespace pdesmas;


MbSharedState::~MbSharedState() {}


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
  if (MailboxAgentMap.find(pReceiverId) == MailboxAgentMap.end()) {
    spdlog::critical("MailboxAgentMap agent not found! {}", pReceiverId);
    exit(1);
  }
  MailboxVariable *dstMbv = MailboxVariableMap.find(dstId)->second;
  if (MailboxVariableMap.find(dstId) == MailboxVariableMap.end()) {
    spdlog::critical("MBV not found! {}", dstId.id());
    exit(1);
  }
  // TODO see the variable in map
  //spdlog::debug("prepare to add msg to {0}", dstId.id());
  // bool rollback_flag = dstMbv->InsertMbMessageWithRollback(pValue, pTime, pSender);
  return dstMbv->InsertMbMessageWithRollback(pValue, pTime, pSender);

}


void MbSharedState::Add(const SsvId &pSsvId, const unsigned long pAgentId) {
  if (ContainsVariable(pSsvId)) {
    spdlog::critical("MbSharedState::Add({0},{1}): Already contains {2}", pSsvId.id(), pAgentId, pSsvId.id());
    exit(1);
  }

  MailboxVariableMap[pSsvId] = new MailboxVariable(pSsvId, pAgentId);
  MailboxAgentMap[pAgentId] = pSsvId;
#ifdef SSV_LOCALISATION
  spdlog::debug("MbSharedState::Add({0},{1})", pSsvId.id(), pAgentId);
  ACCalculator->InitialiseCounters(pSsvId);

#endif
}

void MbSharedState::Insert(const SsvId &pSsvId, const MailboxVariable &pMbVariable, bool &rb_needed) {
  if (ContainsVariable(pSsvId)) {
    spdlog::critical("MbSharedState::Insert({}) which already exist", pSsvId.id());
    exit(1);
  }
  const unsigned long pAgentId = pMbVariable.GetOwnerAgentId();
  MailboxVariableMap[pSsvId] = new MailboxVariable(pSsvId, pAgentId);
  for (auto i:MailboxVariableMap[pSsvId]->GetMessageList()) {
    bool rb = MailboxVariableMap[pSsvId]->InsertMbMessageWithRollback(i.GetValueCopy(), i.GetTime(), i.GetSender());
    rb_needed = rb_needed || rb;
  }
  // MailboxVariableMap[pSsvId] = new MailboxVariable(pMbVariable);

  MailboxAgentMap[pAgentId] = pSsvId;
#ifdef SSV_LOCALISATION
  ACCalculator->InitialiseCounters(pSsvId);

#endif
}

void MbSharedState::Delete(const SsvId &pSSVID) {
  if (!ContainsVariable(pSSVID)) {
    LOG(logERROR) << "MbSharedState::Delete# Trying to delete a non-existant variable!";
    exit(1);
  }
  MailboxAgentMap.erase(MailboxVariableMap[pSSVID]->GetOwnerAgentId());
  MailboxVariableMap.erase(pSSVID);
#ifdef SSV_LOCALISATION
  ACCalculator->RemoveSsvAccessRecord(pSSVID);
  ACCalculator->RemoveSsvHopRecord(pSSVID);
  ACCalculator->RemoveSsvFromList(pSSVID);
#endif
}


MailboxVariable MbSharedState::GetCopy(const SsvId &pSSVID) {
  if (!ContainsVariable(pSSVID)) {
    LOG(logERROR) << "MbSharedState::GetCopy# Trying to get a non-existant variable!";
    exit(1);
  }
  auto to_copy = *(MailboxVariableMap.find(pSSVID)->second);

  return MailboxVariable(to_copy);
}

SerialisableList<MbMail> MbSharedState::Read(const unsigned long pOwnerId, unsigned long pTime) {
  // unsigned long agentId = pAgent.GetId();
  SsvId fSsvId = MailboxAgentMap.find(pOwnerId)->second;
  return MailboxVariableMap.find(fSsvId)->second->ReadMb(pOwnerId, pTime);
}

void MbSharedState::RollbackRead(const unsigned long pOwnerId, unsigned long pTime) {
  // rollbacks reads that shouldn't happen
  SsvId mbvId = MailboxAgentMap.find(pOwnerId)->second;
  MailboxVariable *mbv = MailboxVariableMap.find(mbvId)->second;
  mbv->PerformReadAnti(pOwnerId, pTime);
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



int MbSharedState::GetRankFromAgentId(unsigned long agentId) {
  return fAgentIdToRankMap[agentId];
}

void MbSharedState::SetMailboxAgentMap(const map<unsigned long, SsvId> &maMap) {
  MailboxAgentMap = maMap;
}

void MbSharedState::RemoveOldMessages(unsigned long pTime) {
  for (auto i:MailboxVariableMap) {
    i.second->RemoveOldMessage(pTime);
  }
}

void MbSharedState::SetAgentIdToRankMap(map<unsigned long, int> agentIdToRankMap) {
  this->fAgentIdToRankMap = agentIdToRankMap;
}


void MbSharedState::RemoveMessageList(const SsvId &pSSVID, bool &rb_needed, int &rb_ts) {
  if (!ContainsVariable(pSSVID)) {
    LOG(logERROR) << "MbSharedState::RemoveMessageList# Trying to rmv msgList on non-existant variable!";
    exit(1);
  }
  auto mbvMapIter = MailboxVariableMap.find(pSSVID);
  auto msgList = mbvMapIter->second->GetMessageList();
  auto msgListIter = msgList.rbegin();
  while (msgListIter != msgList.rend()) {
    bool rb = false;
    int ts = msgListIter->GetTime() - 1;
    mbvMapIter->second->PerformWriteAnti(msgListIter->GetSender(), msgListIter->GetTime(), rb);
    if (rb && ts < rb_ts) {
      rb_ts = ts;
    }
    rb_needed = rb_needed || rb;

    ++msgListIter;
  }
}


unsigned long MbSharedState::GetOwnerId(const SsvId &pMbvId) {
  return MailboxVariableMap.find(pMbvId)->second->GetOwnerAgentId();
}

MbSharedState::MbSharedState() {

}
