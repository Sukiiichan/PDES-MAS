//
// Created by Suki on 11/7/2019.
//
#include "HasMailList.h"

using namespace pdesmas;

const SerialisableList<MbMail>& HasMailList::GetMailList() const {
   return fMailList;
}

void HasMailList::SetMailList(SerialisableList<MbMail> & pMailList) {
   fMailList = pMailList;
}

void HasMailList::ClearMailList() {
   fMailList.clear();
}

