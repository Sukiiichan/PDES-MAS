#include "MbWriteResponseMsg.h"
#include "MailboxWriteMessage.h"

using namespace pdesmas;

MbWriteResponseMsg::MbWriteResponseMsg() {
   RegisterClass(GetType(), &CreateInstance);
}

MbWriteResponseMsg::~MbWriteResponseMsg() {}

pdesmasType MbWriteResponseMsg::GetType() const {
   return MBWRITERESPONSEMSG;
}

AbstractMessage* MbWriteResponseMsg::CreateInstance() {
   return new MbWriteResponseMsg;
}

void MbWriteResponseMsg::Serialise(ostream &pOstream) const {
   pOstream << DELIM_LEFT << GetType();
   pOstream << DELIM_VAR_SEPARATOR << fOrigin;
   pOstream << DELIM_VAR_SEPARATOR << fDestination;
   pOstream << DELIM_VAR_SEPARATOR << fTimestamp;
   pOstream << DELIM_VAR_SEPARATOR << fMatternColour;
   pOstream << DELIM_VAR_SEPARATOR << fIdentifier;
   pOstream << DELIM_VAR_SEPARATOR << fOriginalAlp;
   pOstream << DELIM_VAR_SEPARATOR << fWriteStatus;
   pOstream << DELIM_RIGHT;
}

void MbWriteResponseMsg::Deserialise(istream &pIstream) {
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
   pIstream >> fOriginalAlp;
   IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
   pIstream >> fWriteStatus;
   IgnoreTo(pIstream, DELIM_RIGHT);
}