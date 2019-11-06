#include "MbMail.h"

using namespace pdesmas;

MbMail::MbMail() {
    fTime = 0;
    fValue = NULL;
    fSender = LpId(0, 0);
}

MbMail::MbMail(unsigned long pTime, const AbstractValue* pValue, const LpId& pSender){
    // SetTime(pTime);
    fTime = pTime;
    SetValue(pValue);
    fSender = pSender;
}

MbMail::MbMail(const MbMail & pMbMessage){
    fTime = pMbMessage.fTime;
    SetValue(pMbMessage.fValue);
    fSender = LpId(pMbMessage.fSender);
}

MbMail::~MbMail() { //
    if (fValue != NULL) {
        delete fValue;
        fValue = NULL;
    }
}

// operations on single msg

AbstractValue* MbMail::Read(unsigned long pTime) {
    return GetValueCopy();
}


void MbMail::SetTime(unsigned long pTime) {
    fTime = pTime;
}

unsigned long MbMail::GetTime() const {
    return fTime;
}

void MbMail::SetValue(const AbstractValue* pValue) {
    if (pValue == NULL) {
        return;
    }
    fValue = pValue->Clone();
}

AbstractValue* MbMail::GetValueCopy() const {
    if (fValue == NULL)
        return NULL;
    return fValue->Clone();
}

void MbMail::SetSender(const LpId& pSender) {
    fSender = pSender;
}

const LpId& MbMail::GetSender() const {
    return fSender;
}

void MbMail::Serialise(ostream & pOstream) const {
    pOstream << DELIM_LEFT << fTime;
    pOstream << DELIM_VAR_SEPARATOR << fSender;
    pOstream << DELIM_VAR_SEPARATOR << *fValue;
    pOstream << DELIM_RIGHT;
}

void MbMail::Deserialise(istream & pIstream) {
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