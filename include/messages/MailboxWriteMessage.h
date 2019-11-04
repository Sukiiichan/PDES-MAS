#ifndef MAILBOXWRITEMESSAGE_H
#define MAILBOXWRITEMESSAGE_H
#include "HasValue.h"
#include "HasMBOwnerID.h"
#include "SharedStateMessage.h"

namespace pdesmas {
   class MailboxWriteMessage: public SharedStateMessage,
                              public HasMBOwnerID,
                              public HasValue {
   private:
      static AbstractMessage* CreateInstance();

   public:
      MailboxWriteMessage();
      virtual ~MailboxWriteMessage();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
   };
}

#endif
