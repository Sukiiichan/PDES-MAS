//
// Created by ChenS on 2019/5/4.
//

#ifndef MAILBOXWRITEMESSAGE_H
#define MAILBOXWRITEMESSAGE_H

namespace pdesmas {
   class MailboxWriteMessage: public SharedStateMessage,
                              public HasSSVID,
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
