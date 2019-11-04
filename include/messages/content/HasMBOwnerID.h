#ifndef PDES_MAS_HASMBOWNERID_H
#define PDES_MAS_HASMBOWNERID_H

#include <lp/LpId.h>

namespace pdesmas{
   class HasMBOwnerID{
   protected:
       unsigned long  fReceiver;
   public:
      const unsigned long GetMbOwnerId() const;
      void SetMbOwnerId(const unsigned long pReceiverId);
   };
}

#endif //PDES_MAS_HASMBOWNERID_H
