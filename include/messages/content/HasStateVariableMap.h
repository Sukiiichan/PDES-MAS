/*
 * HasStateVariableMap.h
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASSTATEVARIABLEMAP_H_
#define HASSTATEVARIABLEMAP_H_

#include "SerialisableMap.h"
#include "SsvId.h"
#include "StateVariable.h"
#include "MailboxVariable.h"

namespace pdesmas {
  class HasStateVariableMap {
  protected:
    SerialisableMap<SsvId, StateVariable> fStateVariableMap;
    SerialisableMap<SsvId, MailboxVariable> fMbStateVariableMap;
  public:
    const SerialisableMap<SsvId, StateVariable> &GetStateVariableMap() const;

    const SerialisableMap<SsvId, MailboxVariable> &GetMbStateVariableMap() const;

    void SetStateVariableMap(const SsvId &, const StateVariable &);

    void SetMbStateVariableMap(const SsvId &, const MailboxVariable &);

  };
}

#endif /* HASSTATEVARIABLEMAP_H_ */
