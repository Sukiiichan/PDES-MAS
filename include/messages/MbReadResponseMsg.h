#ifndef PDES_MAS_MBREADRESPONSEMSG_H
#define PDES_MAS_MBREADRESPONSEMSG_H

#include "ResponseMessage.h"
#include "HasValue.h"
#include "MailboxReadMessage.h"

namespace pdesmas{
   class MbReadResponseMsg:public ResponseMessage,
         public HasValue{
   private:
      static AbstractMessage* CreateInstance();

   public:
      MbReadResponseMsg();
      virtual ~MbReadResponseMsg();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
   };
}

#endif
