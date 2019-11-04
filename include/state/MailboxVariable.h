#ifndef PDES_MAS_MAILBOXVARIABLE_H
#define PDES_MAS_MAILBOXVARIABLE_H

#include <util/Serialisable.h>
#include <state/SsvId.h>
#include <state/AbstractValue.h>
#include <types/SerialisableList.h>
#include <state/MbMessage.h>
#include <vector>

using namespace std;
namespace pdesmas {
    class MailboxVariable : public Serialisable {
    private:
        SsvId mbVariableID;
        unsigned long  ownerAgentId;
        LpId ownerAgent;
        SerialisableList<MbMessage> messageList;
        unsigned long readUntil;

    public:
        MailboxVariable();

        MailboxVariable(const SsvId &pSsvId, const LpId &pAgent);

        MailboxVariable(const MailboxVariable &);

        ~MailboxVariable();

        const SsvId &GetVariableId() const;

        unsigned long GetReadUntil() const;

        const unsigned long GetOwnerAgentId() const;

        const SerialisableList<MbMessage> &GetMessageList() const;

        bool AddMbMessage(const AbstractValue *, unsigned long, const LpId &);

        bool RemoveMbMessage(const LpId&, unsigned long);

        void RemoveOldMessage(unsigned long);

        void PeformReadRB(const unsigned long pOwnerId, unsigned long pTime);

        void PerformWriteRB(const LpId &pSender, unsigned long pTime, RollbackList pRollbackList);

        vector<pair<LpId, unsigned long>> GetRbList(unsigned long);

        AbstractValue *ReadMb(const unsigned long reqAgentId, unsigned long reqTime);

        void Serialise(ostream &) const;

        void Deserialise(istream &);
    };
}


#endif
