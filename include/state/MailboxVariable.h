#ifndef PDES_MAS_MAILBOXVARIABLE_H
#define PDES_MAS_MAILBOXVARIABLE_H

#include <util/Serialisable.h>
#include <state/SsvId.h>
#include <state/AbstractValue.h>
#include <types/SerialisableList.h>

using namespace std;
namespace pdesmas {
   class MailboxVariable : public Serialisable {
   private:
      SsvId MbStateVariableID;
      SerialisableList<AbstractValue> MessageList;
      unsigned long ReadUntil; //null for normal SSVs

   public:
      MailboxVariable();

      MailboxVariable(const SsvId &);

      MailboxVariable(const MailboxVariable &);

      ~MailboxVariable();

      const SsvId &GetVariableId() const;

      unsigned long GetReadUntil() const;

      void AddMessageContent(const AbstractValue *, unsigned long, const LpId &);

      void RemoveWritePeriods(unsigned long);

      const SerialisableList<WritePeriod> &GetWritePeriodList() const;

      AbstractValue *ReadMb(const LpId &, unsigned long, unsigned long);

      pair<unsigned long, AbstractValue *> ReadWithoutRecord(unsigned long) const;

      void WriteWithRollback(const LpId &, const AbstractValue *, unsigned long, WriteStatus &, RollbackList &);

      void PerformReadRollback(const LpId &, unsigned long);

      void PerformWriteRollback(const LpId &, unsigned long, RollbackList &);

      void Serialise(ostream &) const;

      void Deserialise(istream &);
   };
}


#endif //PDES_MAS_MAILBOXVARIABLE_H
