#include "MailboxVariable.h"

using namespace pdesmas;

MailboxVariable::MailboxVariable() {}

MailboxVariable::MailboxVariable(const SsvId &pSsvId) {
    mbVariableID = pSsvId;
}

MailboxVariable::MailboxVariable(const MailboxVariable &pMbVariable) {
    mbVariableID = SsvId(pMbVariable.mbVariableID);
    messageList = pMbVariable.messageList;
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

void MailboxVariable::AddMbMessage(const AbstractValue *pValue, unsigned long pTime, const LpId &pSender) {
    if (pTime >= readUntil) {
        SerialisableList<MbMessage>::reverse_iterator reverseMbMessageIterator = messageList.rbegin();
        while (reverseMbMessageIterator != messageList.rend()) {
            if (reverseMbMessageIterator->GetTime() < pTime) {
                ++reverseMbMessageIterator;
            } else {
                messageList.insert(reverseMbMessageIterator.base(), MbMessage(pTime, pValue, pSender));
                break;
            }
        }
    } else {
        LOG(logERROR) << "Rollback case 1 occurs";
        // TODO call rollback
    }
}

void MailboxVariable::RemoveMbMessage(unsigned long pTime) {
    LOG(logFINEST) << "";
    SerialisableList<MbMessage>::reverse_iterator reverseMbMessageIterator = messageList.rbegin();
    while (reverseMbMessageIterator != messageList.rend()) {
        if (reverseMbMessageIterator->GetTime() < pTime) {
            ++reverseMbMessageIterator;
        } else if (reverseMbMessageIterator->GetTime() == pTime) {
            // delete
        } else { break; }
    }
}

void MailboxVariable::RemoveOldMessage(unsigned long) {
    // saves memory
}

AbstractValue *MailboxVariable::ReadMb(const LpId &, unsigned long reqTime) {
    // check map
    if (reqTime >= readUntil) {
        SerialisableList<MbMessage>::reverse_iterator reverseMbMessageIterator = messageList.rbegin();
        while (reverseMbMessageIterator != messageList.rend()) {
            if (reverseMbMessageIterator->GetTime()) {
            }
        }
        // read after ru, before ptime
        // check rollback
        readUntil =0;
    } else {
        LOG(logFINE) << "former read request arrives";
    }

}


void MailboxVariable::Serialise(ostream &) const {}

void MailboxVariable::Deserialise(istream &) {

}


