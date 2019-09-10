#ifndef PDES_MAS_MBANTIWRITEMSG_H
#define PDES_MAS_MBANTIWRITEMSG_H

#include "AntiMessage.h"
#include "HasReceiver.h"

namespace pdesmas{
   class MbAntiWriteMsg:public AntiMessage,public HasReceiver{
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
