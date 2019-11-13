#ifndef PDES_MAS_MBWRITEANTIMSG_H
#define PDES_MAS_MBWRITEANTIMSG_H

#include "AntiMessage.h"
#include "HasMBOwnerID.h"

namespace pdesmas{
   class MbWriteAntiMsg: public AntiMessage, public HasMBOwnerID{
   private:
      static AbstractMessage* CreateInstance();

   public:
      MbWriteAntiMsg();
      virtual ~MbWriteAntiMsg();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
   };
}

#endif //PDES_MAS_MBWRITEANTIMSG_H
