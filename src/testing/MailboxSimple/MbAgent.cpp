#include "MbAgent.h"
#include <spdlog/spdlog.h>
#include <unistd.h>
#include <random>

MbAgent::MbAgent(const unsigned long startTime, const unsigned long endTime, unsigned long agentId,
                 const unsigned long start_Time, const unsigned long end_Time, unsigned long agent_Id) : Agent(
      startTime, endTime, agentId) {

}


void MbAgent::SetFrequency(unsigned int pFrequency) {
   commFrequency = pFrequency;
}

void MbAgent::InitSendList(unsigned int totalNum, unsigned int listLen, unsigned int seed = 114514) {
   list<unsigned long> rList;
   srand(seed * agentId);
   vector<unsigned int> temp;
   for (int i = 0; i < totalNum; ++i) { temp.push_back(i); }
   random_shuffle(temp.begin(), temp.end());
   for (int j = 0; j < listLen; ++j) {
      rList.push_back(temp[j]);
   }
   sendList = rList;
}


void MbAgent::Cycle() {
   spdlog::warn("Cycle begin");
   srand(time(NULL));
   default_random_engine eg;
   uniform_int_distribution<int> distribution(100000, 999999);
   for (auto &i:sendList) {
      auto msgSerial = distribution(eg);
      string msgContent = to_string(agentId) + "-0-" + to_string(msgSerial);
      // i is receiver id, 0 means direction
      bool tie = this->WriteMbString(i, msgContent, this->GetLVT() + 1);
      // call ALP to send msg
      spdlog::debug("Agent{0}, Agent{1}, MsgID{3}, sent", this->agentId, i, msgSerial);
      if (tie) {
         // received response msg
         spdlog::debug("Agent{0}, Agent{1}, MsgID{3}, write success", this->agentId, i, msgSerial);
      } else {
         spdlog::debug("Agent{0}, Agent{1}, MsgID{3}, write failed", this->agentId, i, msgSerial);
      }
   }

   // spdlog::debug("Agent{0}, Agent{1}, MsgID{3}, replied", this->agentId, i, msgSerial);

   // TODO read msg from mailbox, and if received reply msg
   spdlog::debug("Agent{0}, request to read mailbox", this->agentId);
   SerialisableMap<unsigned long, string> newMails = this->RequestNewMails<string>(agentId, this->GetAlpLVT() + 1);
   spdlog::debug("Agent{0}, read mailbox success", this->agentId);
   // FIXME may need to print log in Agent.cpp instead

   // TODO internal counter for frequency
}

