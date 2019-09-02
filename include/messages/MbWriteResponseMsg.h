#ifndef PDES_MAS_MBWRITERESPONSEMSG_H
#define PDES_MAS_MBWRITERESPONSEMSG_H

#include "ResponseMessage.h"
#include "HasWriteStatus.h"
#include "WriteMessage.h"

namespace pdesmas{
   class MbWriteResponseMsg:public ResponseMessage, public HasWriteStatus{
   private:
      static AbstractMessage* CreateInstance();

   public:
      MbWriteResponseMsg();
      virtual ~MbWriteResponseMsg();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
   };
}

#endif //PDES_MAS_MBWRITERESPONSEMSG_H
