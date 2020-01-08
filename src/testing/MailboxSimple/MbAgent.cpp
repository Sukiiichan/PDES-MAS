#include "MbAgent.h"
#include <spdlog/spdlog.h>
#include <unistd.h>
#include <random>

MbAgent::MbAgent(const unsigned long startTime, const unsigned long endTime, unsigned long agentId) : Agent(
    startTime, endTime, agentId) {
  freqCounter = 0;
  frequency = 1;
}


//void MbAgent::SetFrequency(unsigned int pFrequency) {
//   commFrequency = pFrequency;
//}

void MbAgent::InitSendList(unsigned int totalNum, unsigned int listLen, unsigned int seed = 114514) {
  list<unsigned long> rList;
  srand(seed * agent_id());
  vector<unsigned int> temp;
  for (unsigned int i = 0; i < totalNum; ++i) { temp.push_back(i); }
  random_shuffle(temp.begin(), temp.end());
  for (unsigned int j = 0; j < listLen; ++j) {
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
    string msgContent = to_string(agent_id()) + "-0-" + to_string(msgSerial);
    // i is receiver id, 0 means direction
    bool tie = this->WriteMbString(i, msgContent, this->GetLVT() + 1);
    // call ALP to send msg
    spdlog::debug("Agent{0}, Agent{1}, MsgID{3}, sent", this->agent_id(), i, msgSerial);
    if (tie) {
      // received response msg
      spdlog::debug("Agent{0}, Agent{1}, MsgID{3}, write success", this->agent_id(), i, msgSerial);
    } else {
      spdlog::debug("Agent{0}, Agent{1}, MsgID{3}, write failed", this->agent_id(), i, msgSerial);
    }
  }
  if (freqCounter < frequency) {
    freqCounter++;
  } else {
    freqCounter = 0;
}
  // spdlog::debug("Agent{0}, Agent{1}, MsgID{3}, replied", this->agentId, i, msgSerial);
  // read msg from mailbox, and see if received reply msg
  if (freqCounter == frequency) {
    spdlog::debug("Agent{0}, request to read mailbox", this->agent_id());
    SerialisableList<MbMail> newMails = this->RequestNewMails(agent_id(), this->GetLVT() + 1);
    // can modify RequestNewMails() to check type
    spdlog::debug("Agent{0}, read mailbox success", this->agent_id());
    auto newMailIterator = newMails.begin();
    while (newMailIterator != newMails.end()) {
      unsigned long sender = newMailIterator->GetSender().GetId();
      //unsigned long timestamp = newMailIterator->GetTime();

      auto msgContentType = newMailIterator->GetValue()->GetType();
      assert(msgContentType == VALUESTRING);
      auto msgContent = ((const Value<string> *) (newMailIterator->GetValue()))->GetValueString();

      string delimiter = "-";
      auto token = msgContent.substr(0, msgContent.find(delimiter));
      msgContent.erase(0, msgContent.find(delimiter) + delimiter.length());

      string reply_tie = msgContent.substr(0, msgContent.find(delimiter));

      unsigned int messageSerial = stoi(msgContent.erase(0, msgContent.find(delimiter) + delimiter.length()));

      if (reply_tie == "0") {
        spdlog::debug("Agent{0}, recv reply from Agent{1},serial{2}", agent_id(), sender, messageSerial);
        // generate reply
        string replyMsgContent = to_string(agent_id()) + "-1-" + to_string(messageSerial);
        bool replyTie = this->WriteMbString(sender, replyMsgContent, this->GetLVT() + 1);
        spdlog::debug("Agent{0},request to reply Agent{1},serial{2}", agent_id(), sender, messageSerial);
        if (replyTie) {
          // received response msg
          spdlog::debug("Agent{0}, Agent{1}, MsgID{3}, write success", this->agent_id(), sender, messageSerial);
        } else {
          spdlog::debug("Agent{0}, Agent{1}, MsgID{3}, write failed", this->agent_id(), sender, messageSerial);
        }
      } else if (reply_tie == "1") {
        // no action
        spdlog::debug("Agent{0}, recv new msg from Agent{1},serial{2}", agent_id(), sender, messageSerial);
      }
    }
  }
}

