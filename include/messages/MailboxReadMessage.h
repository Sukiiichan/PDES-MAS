#ifndef MAILBOXREADMESSAGE_H
#define MAILBOXREADMESSAGE_H

#include "HasMBOwnerID.h"
#include "SharedStateMessage.h"

namespace pdesmas {
   class MailboxReadMessage: public SharedStateMessage,
                             public HasMBOwnerID {
   private:
      static AbstractMessage* CreateInstance();

   public:
      MailboxReadMessage();
      virtual ~MailboxReadMessage();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
   };
}

#endif

// determine if has permission, only readable by owner agent