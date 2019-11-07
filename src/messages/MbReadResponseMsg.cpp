#include "MbReadResponseMsg.h"
#include "ObjectMgr.h"
#include "Log.h"

using namespace pdesmas;

MbReadResponseMsg::MbReadResponseMsg() {
   RegisterClass(GetType(), &CreateInstance);
}

MbReadResponseMsg::~MbReadResponseMsg() {}

pdesmasType MbReadResponseMsg::GetType() const {
   return MBREADRESPONSEMSG;
}

AbstractMessage *MbReadResponseMsg::CreateInstance() {
   return new MbReadResponseMsg;
}

void MbReadResponseMsg::Serialise(ostream &pOstream) const {
   pOstream << DELIM_LEFT << GetType();
   pOstream << DELIM_VAR_SEPARATOR << fOrigin;
   pOstream << DELIM_VAR_SEPARATOR << fDestination;
   pOstream << DELIM_VAR_SEPARATOR << fTimestamp;
   pOstream << DELIM_VAR_SEPARATOR << fMatternColour;
   pOstream << DELIM_VAR_SEPARATOR << fIdentifier;
   pOstream << DELIM_VAR_SEPARATOR << original_agent_;
   pOstream << DELIM_VAR_SEPARATOR << fMailList;
   pOstream << DELIM_RIGHT;
}

void MbReadResponseMsg::Deserialise(istream &pIstream) {
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> fOrigin;
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> fDestination;
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> fTimestamp;
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> fMatternColour;
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> fIdentifier;
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> original_agent_;
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> fMailList;
   IgnoreTo(pIstream, DELIM_RIGHT);
}

