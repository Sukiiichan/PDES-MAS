#ifndef PDES_MAS_MBREADRESPONSEMSG_H
#define PDES_MAS_MBREADRESPONSEMSG_H

#include "ResponseMessage.h"
#include "MailboxReadMessage.h"
#include "HasMailList.h"

namespace pdesmas{
   class MbReadResponseMsg:public ResponseMessage,
         public HasMailList{
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
