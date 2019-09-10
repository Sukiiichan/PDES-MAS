#ifndef PDES_MAS_MAILBOXVARIABLE_H
#define PDES_MAS_MAILBOXVARIABLE_H

#include <util/Serialisable.h>
#include <state/SsvId.h>
#include <state/AbstractValue.h>
#include <types/SerialisableList.h>
#include <state/MbMessage.h>

using namespace std;
namespace pdesmas {
    class MailboxVariable : public Serialisable {
    private:
        SsvId mbVariableID;
        LpId ownerAgent;
        SerialisableList<MbMessage> messageList;
        unsigned long readUntil;

    public:
        MailboxVariable();

        MailboxVariable(const SsvId &, const LpId &);

        MailboxVariable(const MailboxVariable &);

        ~MailboxVariable();

        const SsvId &GetVariableId() const;

        unsigned long GetReadUntil() const;

        const LpId & GetOwnerAgent() const;

        const SerialisableList<MbMessage> &GetMessageList() const;

        bool AddMbMessage(const AbstractValue *, unsigned long, const LpId &);

        void RemoveMbMessage(unsigned long);

        void RemoveOldMessage(unsigned long);

        AbstractValue *ReadMb(const LpId &, unsigned long);

        // TODO rollback functions

        void Serialise(ostream &) const;

        void Deserialise(istream &);
    };
}


#endif
