#include "MailboxWriteMessage.h"
#include "ObjectMgr.h"

using namespace pdesmas;

MailboxWriteMessage::MailboxWriteMessage() {
  RegisterClass(GetType(), &CreateInstance);
}

MailboxWriteMessage::~MailboxWriteMessage() {}

pdesmasType MailboxWriteMessage::GetType() const {
  return MAILBOXWRITEMESSAGE;
}

AbstractMessage *MailboxWriteMessage::CreateInstance() {
  return new MailboxWriteMessage;
}

void MailboxWriteMessage::Serialise(ostream &pOstream) const {
  pOstream << DELIM_LEFT << GetType();
  pOstream << DELIM_VAR_SEPARATOR << fOrigin;
  pOstream << DELIM_VAR_SEPARATOR << fDestination; // Routing info
  pOstream << DELIM_VAR_SEPARATOR << fReceiver;
  pOstream << DELIM_VAR_SEPARATOR << fTimestamp;
  pOstream << DELIM_VAR_SEPARATOR << fMatternColour;
  pOstream << DELIM_VAR_SEPARATOR << fNumberOfHops;
  pOstream << DELIM_VAR_SEPARATOR << fIdentifier;
  pOstream << DELIM_VAR_SEPARATOR << original_agent_;
  pOstream << DELIM_VAR_SEPARATOR << *fValue;
  pOstream << DELIM_RIGHT;
}

void MailboxWriteMessage::Deserialise(istream &pIstream) {
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
  pIstream >> fIdentifier;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> original_agent_;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  //pIstream >> *fValue;
  string valueString;
  pIstream >> valueString;
  string value = GetValueString(valueString);
  fValue = valueClassMap->CreateObject(GetTypeID(valueString));
  fValue->SetValue(value);
  IgnoreTo(pIstream, DELIM_RIGHT);
}
