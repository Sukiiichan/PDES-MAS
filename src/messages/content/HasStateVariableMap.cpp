/*
 * HasStateVariableMap.cpp
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasStateVariableMap.h"

using namespace pdesmas;

const SerialisableMap<SsvId, StateVariable> &HasStateVariableMap::GetStateVariableMap() const {
  return fStateVariableMap;
}

void HasStateVariableMap::SetStateVariableMap(const SsvId &pSsvId, const StateVariable &pStateVariable) {
  fStateVariableMap.insert(make_pair(pSsvId, pStateVariable));
}

const SerialisableMap<SsvId, MailboxVariable> &HasStateVariableMap::GetMbStateVariableMap() const {
  return fMbStateVariableMap;
}

void HasStateVariableMap::SetMbStateVariableMap(const SsvId &pSsvId, const MailboxVariable &pMailboxVariable) {
  fMbStateVariableMap.insert(make_pair(pSsvId, pMailboxVariable));
}