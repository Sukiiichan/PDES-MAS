#include <spdlog/spdlog.h>
#include "MailboxVariable.h"

using namespace pdesmas;

MailboxVariable::MailboxVariable() {}

MailboxVariable::MailboxVariable(const SsvId &pSsvId, const LpId &pAgent) {
  mbVariableID = pSsvId;
  readUntil = 0;
  ownerAgentId = pAgent.GetId();
  messageList = SerialisableList<MbMail>();
  ownerAgent = pAgent;
  spdlog::warn("Constructor of MBV called");
}

MailboxVariable::MailboxVariable(const MailboxVariable &pMbVariable) {
  mbVariableID = SsvId(pMbVariable.mbVariableID);
  messageList = pMbVariable.messageList;
  readUntil = pMbVariable.readUntil;
  messageList = pMbVariable.messageList;
  ownerAgentId = pMbVariable.ownerAgentId;
  spdlog::warn("Copy constructor of MBV called");

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

void MailboxVariable::PeformReadAnti(const unsigned long pOwnerId, unsigned long pTime) {
  if (pTime >= readUntil) {
    LOG(logERROR) << "";
    exit(0);
  } else if (pOwnerId != ownerAgentId) {
    LOG(logERROR) << "";
    exit(0);
  }
//   auto MbvIterator = messageList.begin();
//   while(MbvIterator != messageList.end()){
//      if(MbvIterator->GetTime()>=pTime){
//         // ? may no need to perform actions
//      } else{
//         MbvIterator++;
//      }
//   }
  auto mbMessageIterator = messageList.begin();
  while(mbMessageIterator->GetTime()<readUntil){
    if(mbMessageIterator->GetTime()>=pTime){
      break;
    }
    mbMessageIterator ++;
  }
  mbMessageIterator --;
  readUntil = mbMessageIterator->GetTime();
  // readUntil = pTime;
}

void MailboxVariable::PerformWriteAnti(const LpId &pSender, unsigned long pTime, bool &rb_needed) {
  RemoveMbMessage(pSender, pTime);
  if (pTime <= readUntil) {
    // TODO call RB, get RB timestamp
    rb_needed = true;
//    auto mbMessageIterator = messageList.begin();
//    while(mbMessageIterator->GetTime()<readUntil){
//      mbMessageIterator ++;
//    }
//    mbMessageIterator --;
//    rb_time = mbMessageIterator->GetTime();
    // pRollbackList.AddLp(this->ownerAgent, pTime);
  }
}

bool MailboxVariable::AddMbMessage(const AbstractValue *pValue, unsigned long pTime, const LpId &pSender) {

  if (pTime >= readUntil) {
    auto mbMessageIterator = messageList.begin();
    auto newMsg = MbMail(pTime, pValue, pSender);
    if (messageList.end()->GetTime() <= pTime) {
      messageList.push_back(std::move(newMsg));
    } else {
      while (mbMessageIterator != messageList.end()) {
        if (mbMessageIterator->GetTime() > pTime) {
          messageList.insert(mbMessageIterator, std::move(newMsg));
          // adding of write records done in MbSS
          break;
        }

        ++mbMessageIterator;
      }
    }
    return true;
  } else {
    // TODO RB mailbox owner to pTime
    auto mlIter = messageList.begin();
    auto newMsg = MbMail(pTime, pValue, pSender);
    while (mlIter->GetTime()<=readUntil){
      if(mlIter->GetTime()>pTime){
        messageList.insert(mlIter, newMsg);
        mlIter --;
        readUntil = mlIter->GetTime();
        // TODO may move to antiread processing
        //readUntil = 1 msg before pTime
        break;
      }
      ++ mlIter;
    }
    return false;
  }
}

bool MailboxVariable::RemoveMbMessage(const LpId &pSender, unsigned long pTime) {
  spdlog::debug("MailboxVariable::RemoveMbMessage# Remove message sent at {}", pTime);
  SerialisableList<MbMail>::iterator mbMessageIterator = messageList.begin();
  while (mbMessageIterator != messageList.end()) {
    if (mbMessageIterator->GetTime() == pTime) {
      if (pTime < readUntil) {
        return false;
      } else if (mbMessageIterator->GetSender() != pSender) {
        ++mbMessageIterator;
      }
      mbMessageIterator = messageList.erase(mbMessageIterator);
      break;
    } else if (mbMessageIterator->GetTime() < pTime) {
      ++mbMessageIterator;
    } else {
      break;
    }
  }
  if (messageList.end()->GetTime() == pTime) {
    messageList.erase(messageList.end());
  }
  return true;
}

void MailboxVariable::RemoveOldMessage(unsigned long pTime) {
  // delete read message received before pTime to save memory space
  spdlog::debug("MailboxVariable::RemoveOldMessage# Remove message sent at {}", pTime);
  if (pTime >= messageList.begin()->GetTime() && pTime < readUntil) {
    auto mbMessageIterator = messageList.begin();
    while (mbMessageIterator->GetTime() <= pTime) {
      mbMessageIterator = messageList.erase(mbMessageIterator);
    }
  }
}

//vector<pair<LpId, unsigned long>> MailboxVariable::GetRbList(unsigned long pTime) {
//   vector<pair<LpId, unsigned long>> result;
//   if(pTime >= readUntil){
//      LOG(logERROR) << "no need to perform rollback";
//      exit(0);
//   }
//   auto mbMessageIterator = messageList.begin();
//   while(mbMessageIterator->GetTime()!=readUntil){
//      if(mbMessageIterator->GetTime()<pTime){
//         mbMessageIterator ++;
//      } else{
//         result.emplace_back(mbMessageIterator->GetSender(),mbMessageIterator->GetTime());
//      }
//   }
//   return result;
//}

SerialisableList<MbMail> MailboxVariable::ReadMb(const unsigned long reqAgentId, unsigned long reqTime) {
  SerialisableList<MbMail> mailList;
  if (reqAgentId == ownerAgentId) {
    // or check map
    //if (reqTime >= readUntil && reqTime <= messageList.end()->GetTime()) {
    if (reqTime >= readUntil) {
      auto mbMessageIterator = messageList.begin();
      while (mbMessageIterator != messageList.end()) {
        if (mbMessageIterator->GetTime() > reqTime) {
//               MbMail temp;
//               temp.SetTime(mbMessageIterator->GetTime());
//               temp.SetSender(mbMessageIterator->GetSender());
//               temp.SetValue(mbMessageIterator->GetValue());
          mailList.push_back(*mbMessageIterator);
          readUntil = mbMessageIterator->GetTime();
        }
        ++mbMessageIterator;
      }
      return mailList;
    } else {
      LOG(logERROR) << "former read request arrives";
    }
  } else {
    LOG(logERROR) << "not allowed to read";
  }
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

