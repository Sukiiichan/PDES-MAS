//
// Created by Suki on 11/7/2019.
//

#ifndef PDES_MAS_HASMAILLIST_H
#define PDES_MAS_HASMAILLIST_H

#include "MbMail.h"
#include "SerialisableList.h"

namespace pdesmas {
   class HasMailList {
   protected:
      SerialisableList<MbMail> fMailList;
   public:
      const SerialisableList<MbMail> &GetMailList() const;

      void SetMailList(SerialisableList<MbMail> &mailList);

      void ClearMailList();
   };
}

#endif //PDES_MAS_HASMAILLIST_H