//
// Created by Suki on 5/20/2019.
//

#ifndef PDES_MAS_MAILBOXSHAREDSTATE_H
#define PDES_MAS_MAILBOXSHAREDSTATE_H

#include <util/Serialisable.h>
#include <state/AbstractValue.h>
#include <state/SsvId.h>
#include <lp/LpId.h>
#include <map>
#include <lp/AccessCostCalculator.h>
#include "MailboxVariable.h"

using namespace std;
namespace pdesmas {
   class MailboxSharedState : public Serialisable {
   private:
      map<SsvId, MailboxVariable> MailboxVariableMap;
      map<SsvId, unsigned long> MailboxAgentMap; // map agentid with mailbox variable
      AccessCostCalculator *AccessCostCalculator;

      bool ContainsVariable(const SsvId &);

   public:
      AbstractValue *ReadMb(const SsvId &, const LpId &, unsigned long);
   };
};


#endif //PDES_MAS_MAILBOXSHAREDSTATE_H
