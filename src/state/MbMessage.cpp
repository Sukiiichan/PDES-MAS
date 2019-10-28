#include "MbMessage.h"

using namespace pdesmas;

MbMessage::MbMessage() {
    fTime = 0;
    fValue = NULL;
    fSender = LpId(0, 0);
}

MbMessage::MbMessage(unsigned long pTime, const AbstractValue* pValue, const LpId& pSender){
    // SetTime(pTime);
    fTime = pTime;
    SetValue(pValue);
    fSender = pSender;
}

MbMessage::MbMessage(const MbMessage & pMbMessage){
    fTime = pMbMessage.fTime;
    SetValue(pMbMessage.fValue);
    fSender = LpId(pMbMessage.fSender);
}

MbMessage::~MbMessage() { //
    if (fValue != NULL) {
        delete fValue;
        fValue = NULL;
    }
}

// operations on single msg

AbstractValue* MbMessage::Read(unsigned long pTime) {
    return GetValueCopy();
}


void MbMessage::SetTime(unsigned long pTime) {
    fTime = pTime;
}

unsigned long MbMessage::GetTime() const {
    return fTime;
}

void MbMessage::SetValue(const AbstractValue* pValue) {
    if (pValue == NULL) {
        return;
    }
    fValue = pValue->Clone();
}

AbstractValue* MbMessage::GetValueCopy() const {
    if (fValue == NULL)
        return NULL;
    return fValue->Clone();
}

void MbMessage::SetSender(const LpId& pSender) {
    fSender = pSender;
}

const LpId& MbMessage::GetSender() const {
    return fSender;
}

void MbMessage::Serialise(ostream & pOstream) const {
    pOstream << DELIM_LEFT << fTime;
    pOstream << DELIM_VAR_SEPARATOR << fSender;
    pOstream << DELIM_VAR_SEPARATOR << *fValue;
    pOstream << DELIM_RIGHT;
}

void MbMessage::Deserialise(istream & pIstream) {
    IgnoreTo(pIstream, DELIM_LEFT);
    pIstream >> fTime;
    IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
    pIstream >> fSender;
    IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
    IgnoreTo(pIstream, DELIM_LEFT);
    string valueString;
    getline(pIstream, valueString, DELIM_LIST_RIGHT);
    string value = GetValueString(valueString);
    fValue = valueClassMap->CreateObject(GetTypeID(valueString));
    fValue->SetValue(value);
    pIstream.unget();
}