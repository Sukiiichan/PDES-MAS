#include "MailboxVariable.h"

using namespace pdesmas;

MailboxVariable::MailboxVariable() {}

MailboxVariable::MailboxVariable(const SsvId &pSsvId, const unsigned long pAgentId) {
  mbVariableID = pSsvId;
  readUntil = 0;
  ownerAgentId = pAgentId;
  messageList = SerialisableList<MbMail>();
  //ownerAgent = pAgent;
  //spdlog::warn("Constructor of MBV called");
}

MailboxVariable::MailboxVariable(const MailboxVariable &pMbVariable) {
  mbVariableID = SsvId(pMbVariable.mbVariableID);
  messageList = pMbVariable.messageList;
  readUntil = pMbVariable.readUntil;
  messageList = pMbVariable.messageList;
  ownerAgentId = pMbVariable.ownerAgentId;
}

MailboxVariable::~MailboxVariable() {}


const SsvId &MailboxVariable::GetVariableId() const {
  return mbVariableID;
}

unsigned long MailboxVariable::GetReadUntil() const {
  return readUntil;
}

const unsigned long MailboxVariable::GetOwnerAgentId() const {
  return ownerAgentId;
}

const SerialisableList<MbMail> &MailboxVariable::GetMessageList() const {
  return messageList;
}

// AddMsgList

void MailboxVariable::PerformReadAnti(const unsigned long pOwnerId, unsigned long pTime) {

  if (pOwnerId != ownerAgentId) {
    spdlog::critical("MailboxVariable::PeformReadAnti(): pOwnerId({0}) != ownerAgentId({1})", pOwnerId, ownerAgentId);
    exit(1);
  }

  if (readUntil > pTime) {
    spdlog::debug("Agent {} readUntil rollback to {}", pOwnerId, pTime);
    readUntil = pTime;
  }
}

void MailboxVariable::PerformWriteAnti(const LpId &pSender, unsigned long pTime, bool &rb_needed) {
  spdlog::debug("MailboxVariable::PerformWriteAnti: Agent {0} retracting message sent to {1} at {2}", pSender.GetId(),
                ownerAgentId, pTime);
  RemoveMbMessage(pSender, pTime);

  rb_needed = pTime <= readUntil;
}

void MailboxVariable::AddMessageToMessageList(const MbMail &mail) {
  if (messageList.empty()) {
    messageList.push_back(mail);
    return;
  }
  for (auto i = messageList.begin(); i != messageList.end(); i++) {
    if (i->GetTime() >= mail.GetTime()) {
      messageList.insert(i, mail);
      spdlog::warn("LOGMEM ssv {} time {} LEN {}", this->mbVariableID.id(), mail.GetTime(), messageList.size() * 40);
      // spdlog::debug("agent {0}, number of entries {1} at time {2}", ownerAgentId, messageList.size(),mail.GetTime());
      return;
    }
  }
  messageList.push_back(mail);

}

bool
MailboxVariable::InsertMbMessageWithRollback(const AbstractValue *pValue, unsigned long pTime, const LpId &pSender) {

  auto newMsg = MbMail(pTime, pValue, pSender);
  this->AddMessageToMessageList(newMsg);
  bool rb_needed = pTime <= readUntil;
  if (rb_needed) {
    spdlog::debug("Rollback caused by write in MBV {0}, readuntil {1}, time {2}", this->mbVariableID.id(), readUntil,
                  pTime);
    this->PerformReadAnti(this->ownerAgentId, pTime - 1);
  }
  spdlog::warn("LOGMEM ssv {} time {} LEN {}", this->mbVariableID.id(), pTime, messageList.size() * 40);
  return !rb_needed;
}

bool MailboxVariable::RemoveMbMessage(const LpId &pSender, unsigned long pTime) {
  spdlog::debug("MailboxVariable::RemoveMbMessage: Remove message sent at {0}, agent {1}", pTime, ownerAgentId);
  auto mbMessageIterator = messageList.begin();
  bool no_rollback = pTime > readUntil;
  while (mbMessageIterator != messageList.end()) {
    if (mbMessageIterator->GetTime() == pTime) {
      if (mbMessageIterator->GetSender() == pSender) {
        messageList.erase(mbMessageIterator);
        break;
      }
    }
    ++mbMessageIterator;
  }
  spdlog::warn("LOGMEM ssv {} time {} LEN {}", this->mbVariableID.id(), pTime, messageList.size() * 40);

  return no_rollback;
}

void MailboxVariable::RemoveOldMessage(unsigned long pTime) {
  // delete read message received before pTime to save memory space
  // spdlog::debug("MailboxVariable::RemoveOldMessage# Remove message sent at {0}, MBV {1}", pTime, mbVariableID.id());

  if (pTime >= messageList.begin()->GetTime() && pTime < readUntil) {
    auto mbMessageIterator = messageList.begin();
    while (mbMessageIterator->GetTime() <= pTime) {
      ++mbMessageIterator;
    }
    messageList.erase(messageList.begin(), mbMessageIterator);
    spdlog::warn("LOGMEM ssv {} time {} LEN {}", this->mbVariableID.id(), pTime, messageList.size() * 40);
  }
}

SerialisableList<MbMail> MailboxVariable::ReadMb(const unsigned long reqAgentId, unsigned long reqTime) {
  SerialisableList<MbMail> mailList;
  if (reqAgentId == ownerAgentId) {
    // or check map
    //if (reqTime >= readUntil && reqTime <= messageList.end()->GetTime()) {
    if (reqTime > readUntil) {
      auto mbMessageIterator = messageList.begin();
      while (mbMessageIterator != messageList.end()) {
        if (mbMessageIterator->GetTime() >= reqTime) {
//               MbMail temp;
//               temp.SetTime(mbMessageIterator->GetTime());
//               temp.SetSender(mbMessageIterator->GetSender());
//               temp.SetValue(mbMessageIterator->GetValue());
          mailList.push_back(*mbMessageIterator);
        }
        ++mbMessageIterator;
      }

      readUntil = reqTime;
      spdlog::debug("MailboxVariable::ReadMb: Agent {0} read mailbox, set readUntil to {1} ", ownerAgentId, readUntil);

      return mailList;
    } else {
      spdlog::critical("MailboxVariable::ReadMb: Read request at t={0} is not later than readUntil({1}), agent {2}",
                       reqTime,
                       readUntil, ownerAgentId);
      exit(1);
    }
  }
  spdlog::critical("MailboxVariable::ReadMb: Agent {0} not allowed to read mailbox of {1}", reqAgentId, ownerAgentId);
  exit(1);


}

void MailboxVariable::Serialise(ostream &pOstream) const {
  pOstream << DELIM_LEFT << mbVariableID;
  pOstream << DELIM_VAR_SEPARATOR << ownerAgentId;
  pOstream << DELIM_VAR_SEPARATOR << messageList;
  pOstream << DELIM_VAR_SEPARATOR << readUntil;
  pOstream << DELIM_RIGHT;
}

void MailboxVariable::Deserialise(istream &pIstream) {
  IgnoreTo(pIstream, DELIM_LEFT);
  pIstream >> mbVariableID;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> ownerAgentId;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> messageList;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> readUntil;
  IgnoreTo(pIstream, DELIM_RIGHT);
}



