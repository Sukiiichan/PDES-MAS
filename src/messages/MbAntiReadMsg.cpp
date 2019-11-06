#include "MbAntiReadMsg.h"

using namespace pdesmas;

MbAntiReadMsg::MbAntiReadMsg() {
   RegisterClass(GetType(), &CreateInstance);
}

MbAntiReadMsg::~MbAntiReadMsg() {}

pdesmasType MbAntiReadMsg::GetType() const {
   return MBANTIREADMSG;
}

AbstractMessage* MbAntiReadMsg::CreateInstance() {
   return new MbAntiReadMsg;
}

void MbAntiReadMsg::Serialise(ostream &pOstream) const {
   pOstream << DELIM_LEFT << GetType();
   pOstream << DELIM_VAR_SEPARATOR << fOrigin;
   pOstream << DELIM_VAR_SEPARATOR << fDestination;
   pOstream << DELIM_VAR_SEPARATOR << fTimestamp;
   pOstream << DELIM_VAR_SEPARATOR << fMatternColour;
   pOstream << DELIM_VAR_SEPARATOR << fNumberOfHops;
   pOstream << DELIM_VAR_SEPARATOR << fRollbackTag;
   pOstream << DELIM_VAR_SEPARATOR << original_agent_;
   pOstream << DELIM_RIGHT;
}

void MbAntiReadMsg::Deserialise(istream &pIstream) {
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> fOrigin;
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> fDestination;
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> fTimestamp;
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> fMatternColour;
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> fNumberOfHops;
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> fRollbackTag;
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> original_agent_;
   IgnoreTo(pIstream, DELIM_RIGHT);
}
