#ifndef PDES_MAS_MAILBOXVARIABLE_H
#define PDES_MAS_MAILBOXVARIABLE_H

#include <util/Serialisable.h>
#include <state/SsvId.h>
#include <state/AbstractValue.h>
#include <types/SerialisableList.h>
#include <state/MbMail.h>
#include <vector>
#include <Log.h>
#include <spdlog/spdlog.h>

using namespace std;
namespace pdesmas {
    class MailboxVariable : public Serialisable {
    private:
        SsvId mbVariableID;
        unsigned long  ownerAgentId;
        //LpId ownerAgent;
        SerialisableList<MbMail> messageList;
        unsigned long readUntil;

    public:
        MailboxVariable();

        MailboxVariable(const SsvId &pSsvId, const unsigned long pAgentId);

        MailboxVariable(const MailboxVariable &);

        ~MailboxVariable();

        const SsvId &GetVariableId() const;

        unsigned long GetReadUntil() const;

        const unsigned long GetOwnerAgentId() const;

        const SerialisableList<MbMail> &GetMessageList() const;

        bool AddMbMessage(const AbstractValue *, unsigned long pTime, const LpId &pSender);

        bool RemoveMbMessage(const LpId&, unsigned long);

        void RemoveOldMessage(unsigned long);

        void PeformReadAnti(const unsigned long pOwnerId, unsigned long pTime);

        void PerformWriteAnti(const LpId &pSender, unsigned long pTime, bool &rb_needed);

        vector<pair<LpId, unsigned long>> GetRbList(unsigned long);

        SerialisableList<MbMail> ReadMb(const unsigned long reqAgentId, unsigned long reqTime);

        void Serialise(ostream &) const;

        void Deserialise(istream &);
    };
}


#endif
