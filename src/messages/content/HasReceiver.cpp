#include "HasReceiver.h"

using namespace pdesmas;

const LpId& HasReceiver::GetReceiver() const {
   return fReceiver;
}

void HasReceiver::SetReceiver(const pdesmas::LpId &pReceiver) {
   fReceiver = pReceiver;
}