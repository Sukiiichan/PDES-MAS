#ifndef PDES_MAS_MBANTIREADMSG_H
#define PDES_MAS_MBANTIREADMSG_H

#include "AntiMessage.h"
#include "HasSSVID.h"

namespace pdesmas{
   class MbAntiReadMsg:public AntiMessage,public HasSSVID{
   private:
      static AbstractMessage* CreateInstance();

   public:
      MbAntiReadMsg();
      virtual ~MbAntiReadMsg();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
   };
}

#endif //PDES_MAS_MBANTIREADMSG_H
