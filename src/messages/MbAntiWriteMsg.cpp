#include "MbAntiWriteMsg.h"

using namespace pdesmas;

MbAntiWriteMsg::MbAntiWriteMsg() {
   RegisterClass(GetType(), &CreateInstance);
}

MbAntiWriteMsg::~MbAntiWriteMsg() {}

pdesmasType MbAntiWriteMsg::GetType() const {
   return MBANTIWRITEMSG;
}

AbstractMessage* MbAntiWriteMsg::CreateInstance() {
   return new MbAntiWriteMsg;
}

void MbAntiWriteMsg::Serialise(ostream &pOstream) const {
   pOstream << DELIM_LEFT << GetType();
   pOstream << DELIM_VAR_SEPARATOR << fOrigin;
   pOstream << DELIM_VAR_SEPARATOR << fDestination;
   pOstream << DELIM_VAR_SEPARATOR << fReceiver;
   pOstream << DELIM_VAR_SEPARATOR << fTimestamp;
   pOstream << DELIM_VAR_SEPARATOR << fMatternColour;
   pOstream << DELIM_VAR_SEPARATOR << fNumberOfHops;
   pOstream << DELIM_VAR_SEPARATOR << fRollbackTag;
   pOstream << DELIM_VAR_SEPARATOR << original_agent_;
   pOstream << DELIM_RIGHT;
}

void MbAntiWriteMsg::Deserialise(istream &pIstream) {
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> fOrigin;
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> fDestination;
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> fReceiver;
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