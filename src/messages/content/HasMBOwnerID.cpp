#include "HasMBOwnerID.h"

using namespace pdesmas;

const unsigned long HasMBOwnerID::GetMbOwnerId() const {
   return fReceiver;
}

void HasMBOwnerID::SetMbOwnerId(const unsigned long pReceiverId) {
   fReceiver = pReceiverId;
}