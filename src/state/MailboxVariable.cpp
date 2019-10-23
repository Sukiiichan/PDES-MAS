#include "MailboxVariable.h"

using namespace pdesmas;

MailboxVariable::MailboxVariable() {}

MailboxVariable::MailboxVariable(const SsvId &pSsvId, const LpId &pAgent) {
   mbVariableID = pSsvId;
   readUntil = 0;
   ownerAgent = pAgent;
}

MailboxVariable::MailboxVariable(const MailboxVariable &pMbVariable) {
   mbVariableID = SsvId(pMbVariable.mbVariableID);
   messageList = pMbVariable.messageList;
   readUntil = pMbVariable.readUntil;
   ownerAgent = pMbVariable.ownerAgent;
}

MailboxVariable::~MailboxVariable() {}


const SsvId &MailboxVariable::GetVariableId() const {
   return mbVariableID;
}

unsigned long MailboxVariable::GetReadUntil() const {
   return readUntil;
}

const LpId &MailboxVariable::GetOwnerAgent() const {
   return ownerAgent;
}

const SerialisableList<MbMessage> &MailboxVariable::GetMessageList() const {
   return messageList;
}

// AddMsgList

void MailboxVariable::PeformReadRB(const LpId & pSender, unsigned long pTime) {
   // TODO del read list & modify mails in mbv
   if(pTime > readUntil){
      LOG(logERROR) << "";
      exit(0);
   }
   auto MbvIterator = messageList.begin();
   while(MbvIterator != messageList.end()){
      if(MbvIterator->GetTime()>=pTime){
         // ? may no need to perform actions
      } else{
         MbvIterator++;
      }
   }
   readUntil = pTime;
}

void MailboxVariable::PerformWriteRB(const LpId &) {

}

bool MailboxVariable::AddMbMessage(const AbstractValue *pValue, unsigned long pTime, const LpId &pSender) {
   if (pTime >= readUntil) {
      auto mbMessageIterator = messageList.begin();
      auto newMsg = MbMessage(pTime, pValue, pSender);
      while (mbMessageIterator != messageList.end()) {
         if (mbMessageIterator->GetTime() > pTime) {
            messageList.insert(mbMessageIterator, newMsg);
            // adding of write records done in MbSS
            break;
         }else if(mbMessageIterator->GetTime() == pTime){
            // TODO might no need to RB
         }
         else {
            ++mbMessageIterator;
         }
      }
      if (messageList.end()->GetTime() < pTime) {
         messageList.push_back(newMsg);
      }
      return true;
   } else {
      LOG(logERROR) << "Rollback case 1 occurs";
      readUntil = pTime;
      return false;
   }
}

void MailboxVariable::RemoveMbMessage(unsigned long pTime) {
   LOG(logFINEST) << "MailboxVariable::RemoveMbMessage# Remove message sent at " << pTime;
   SerialisableList<MbMessage>::iterator mbMessageIterator = messageList.begin();
   while (mbMessageIterator != messageList.end()) {
      if (mbMessageIterator->GetTime() == pTime) {
         if (pTime < readUntil) {
            // TODO possible RB
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
}

void MailboxVariable::RemoveOldMessage(unsigned long pTime) {
   // delete read message received before pTime to save memory space
   LOG(logFINEST) << "";
   if (pTime >= messageList.begin()->GetTime() && pTime < readUntil) {
      auto mbMessageIterator = messageList.begin();
      while (mbMessageIterator->GetTime() <= pTime) {
         mbMessageIterator = messageList.erase(mbMessageIterator);
      }
   }
}

vector<pair<LpId, unsigned long>> MailboxVariable::GetRbList(unsigned long pTime) {
   vector<pair<LpId, unsigned long>> result;
   if(pTime >= readUntil){
      LOG(logERROR) << "no need to perform rollback";
      exit(0);
   }
   auto mbMessageIterator = messageList.begin();
   while(mbMessageIterator->GetTime()!=readUntil){
      if(mbMessageIterator->GetTime()<pTime){
         mbMessageIterator ++;
      } else{
         result.emplace_back(mbMessageIterator->GetSender(),mbMessageIterator->GetTime());
      }
   }
   return result;
}

AbstractValue *MailboxVariable::ReadMb(const LpId &reqAgent, unsigned long reqTime) {
   if (reqAgent.GetId() == ownerAgent.GetId()) {
      // or check map
      if (reqTime >= readUntil && reqTime <= messageList.end()->GetTime()) {
         auto mbMessageIterator = messageList.begin();
         while (mbMessageIterator != messageList.end()) {
            if (mbMessageIterator->GetTime() < reqTime) {
               ++mbMessageIterator;
            } else {
               auto msgContent = mbMessageIterator->GetValueCopy();
               readUntil = mbMessageIterator->GetTime();
               return msgContent;
            }
         }
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
   pOstream << DELIM_VAR_SEPARATOR << ownerAgent;
   pOstream << DELIM_VAR_SEPARATOR << messageList;
   pOstream << DELIM_VAR_SEPARATOR << readUntil;
   pOstream << DELIM_RIGHT;
}

void MailboxVariable::Deserialise(istream &pIstream) {
   IgnoreTo(pIstream, DELIM_LEFT);
   pIstream >> mbVariableID;
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> ownerAgent;
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> messageList;
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> readUntil;
   IgnoreTo(pIstream, DELIM_RIGHT);
}

