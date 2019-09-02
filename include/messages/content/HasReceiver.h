#ifndef PDES_MAS_HASRECEIVER_H
#define PDES_MAS_HASRECEIVER_H

#include <lp/LpId.h>

namespace pdesmas{
   class HasReceiver{
   protected:
      LpId fReceiver;
   public:
      const LpId& GetReceiver() const;
      void SetReceiver(const LpId&);
   };
}

#endif //PDES_MAS_HASRECEIVER_H
