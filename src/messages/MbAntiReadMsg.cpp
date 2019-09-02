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

void MbAntiReadMsg::Serialise(ostream &) const {}

void MbAntiReadMsg::Deserialise(istream &) {}
