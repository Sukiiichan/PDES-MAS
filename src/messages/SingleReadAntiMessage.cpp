#include "SingleReadAntiMessage.h"

using namespace pdesmas;

SingleReadAntiMessage::SingleReadAntiMessage() {
  RegisterClass(GetType(), &CreateInstance);
}

SingleReadAntiMessage::~SingleReadAntiMessage() {
  // Empty deconstructor
}

pdesmasType SingleReadAntiMessage::GetType() const {
  return SINGLEREADANTIMESSAGE;
}

AbstractMessage* SingleReadAntiMessage::CreateInstance() {
  return new SingleReadAntiMessage;
}

void SingleReadAntiMessage::Serialise(ostream& pOstream) const {
  pOstream << DELIM_LEFT << GetType();
  pOstream << DELIM_VAR_SEPARATOR << fOrigin;
  pOstream << DELIM_VAR_SEPARATOR << fDestination;
  pOstream << DELIM_VAR_SEPARATOR << fTimestamp;
  pOstream << DELIM_VAR_SEPARATOR << fMatternColour;
  pOstream << DELIM_VAR_SEPARATOR << fNumberOfHops;
  pOstream << DELIM_VAR_SEPARATOR << fRollbackTag;
  pOstream << DELIM_VAR_SEPARATOR << fOriginalAlp;
  pOstream << DELIM_VAR_SEPARATOR << fSsvId;
  pOstream << DELIM_RIGHT;
}

void SingleReadAntiMessage::Deserialise(istream& pIstream) {
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
  pIstream >> fOriginalAlp;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fSsvId;
  IgnoreTo(pIstream, DELIM_RIGHT);
}