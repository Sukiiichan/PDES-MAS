#ifndef PDES_MAS_MBANTIWRITEMSG_H
#define PDES_MAS_MBANTIWRITEMSG_H

#include "AntiMessage.h"
#include "HasMBOwnerID.h"

namespace pdesmas{
   class MbAntiWriteMsg:public AntiMessage,public HasMBOwnerID{
   private:
      static AbstractMessage* CreateInstance();

   public:
      MbAntiWriteMsg();
      virtual ~MbAntiWriteMsg();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
   };
}

#endif //PDES_MAS_MBANTIWRITEMSG_H
