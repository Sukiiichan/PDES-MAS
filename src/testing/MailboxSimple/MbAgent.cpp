#include <utility>

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


void MbAgent::InitSendList(list<unsigned long> agList, unsigned int listLen) {
  list<unsigned long> rList;
  //spdlog::warn("size of agList {}", agList.size());
  srand(114514 * agent_id());
  for (unsigned int i = 0; i < listLen; ++i) {
    auto random_index = random() % agList.size();
    auto agListIter = agList.begin();
    advance(agListIter, random_index);
    if (*agListIter != agent_id()) {
      rList.push_back(*agListIter);
    }
  }
//  for (auto v:rList) {
//    std::cout << v << "\n";
//  }
  sendList = rList;
//  srand(seed * agent_id());
//  vector<unsigned int> temp;
//  for (unsigned int i = 0; i < totalNum; ++i) { temp.push_back(i); }
//  random_shuffle(temp.begin(), temp.end());
//  for (unsigned int j = 0; j < listLen; ++j) {
//    rList.push_back(temp[j]);
//  }
}


void MbAgent::Cycle() {
  if (CheckSyncGVT()) {
    SendGVTMessage();
  }
  spdlog::warn("Cycle begin, Agent {}, LVT {}", agent_id(), GetLVT());

  srand(agent_id() * GetLVT());
//  vector<int> temp_vector;
//  for (int i = 100000; i < 999999; i++) {
//    temp_vector.push_back(i);
//  }
//  random_shuffle(temp_vector.begin(), temp_vector.end());
//  auto tv_iter = temp_vector.begin();
  // default_random_engine eg;
  // uniform_int_distribution<int> distribution(100000, 999999);

  for (auto &i:sendList) {
//    int msgSerial = *tv_iter;
    int msgSerial = rand() % 100000;
    spdlog::debug("Msg serial of agent {0} is {1}", agent_id(), msgSerial);
    //tv_iter++;
    // spdlog::debug("send to {0}", i);
    // auto msgSerial = distribution(eg);
    string mailContent = "0-" + to_string(msgSerial);
    // i is receiver id, 0 means direction
    // bool tie = this->WriteMbString(i, mailContent, this->GetLVT() + 1);
    bool tie = this->SendMail(i, this->GetLVT() + 1, mailContent);
    // call ALP to send msg
    spdlog::debug("Agent{0}, Agent{1}, MsgID{2}, at ts = {3}, sent", this->agent_id(), i, msgSerial, this->GetLVT());
    if (tie) {
      // received response msg
      spdlog::debug("Agent{0}, Agent{1}, MsgID{2}, write success", this->agent_id(), i, msgSerial);
    } else {
      spdlog::debug("Agent{0}, Agent{1}, MsgID{2}, write failed", this->agent_id(), i, msgSerial);
    }
  }
//
  if (freqCounter < frequency) {
    freqCounter++;
    spdlog::debug("round {0}", freqCounter);
  } else {
    freqCounter = 0;
  }
  // spdlog::debug("Agent{0}, Agent{1}, MsgID{3}, replied", this->agentId, i, msgSerial);
  // read msg from mailbox, and see if received reply msg
  //SendGVTMessage();

  if (freqCounter == frequency) {
    spdlog::debug("Agent{0}, request to read mailbox", this->agent_id());
    //--SerialisableList<MbMail> newMails = this->RequestNewMails(agent_id(), this->GetLVT() + 1);
    list<Mail> newMails = ReadMail(this->GetLVT() + 1);
    string delimiter = "-";
    // can modify RequestNewMails() to check type
    spdlog::debug("Agent{0}, read mailbox success, size {1}", this->agent_id(), newMails.size());
    auto newMailIterator = newMails.begin();
    while (newMailIterator != newMails.end()) {
      //--unsigned long sender = newMailIterator->GetSender().GetId();
      unsigned long sender = newMailIterator->sender_;
      string mailContent = newMailIterator->message_;
      auto reply_tie = mailContent.substr(0, mailContent.find(delimiter));
      string serial_str = mailContent.erase(0, mailContent.find(delimiter) + delimiter.length());
      // unsigned long timestamp = newMailIterator->GetTime();
      //--auto msgContentType = newMailIterator->GetValue()->GetType();
      //--assert(msgContentType == VALUESTRING);
      //--auto msgContent = ((const Value<string> *) (newMailIterator->GetValue()))->GetValueString();
      //--spdlog::debug("mail content {0}", msgContent);

//    auto token = msgContent.substr(0, msgContent.find(delimiter));
//    msgContent.erase(0, msgContent.find(delimiter) + delimiter.length());
//
      //--string reply_tie = msgContent.substr(0, msgContent.find(delimiter));

      //--string serial_str = msgContent.erase(0, msgContent.find(delimiter) + delimiter.length());
      if (reply_tie == "0") {
//        spdlog::debug("Agent{0}, recv reply from Agent{1},serial{2}", agent_id(), sender, messageSerial);
        // generate reply
        string replyMsgContent = "1-" + serial_str;
        bool replyTie = this->SendMail(sender, this->GetLVT() + 1, replyMsgContent);
        spdlog::debug("Agent{0},request to reply Agent{1},serial{2}", agent_id(), sender, stoi(serial_str));
        if (replyTie) {
          // received response msg
          spdlog::debug("Agent{0}, Agent{1}, MsgID{2}, write success", this->agent_id(), sender, stoi(serial_str));
        } else {
          spdlog::debug("Agent{0}, Agent{1}, MsgID{2}, write failed", this->agent_id(), sender, stoi(serial_str));
        }
      } else if (reply_tie == "1") {
        // no action
//        spdlog::debug("Agent{0}, recv new msg from Agent{1},serial{2}", agent_id(), sender, messageSerial);
      }
      newMailIterator++;
    }
  }
}


MbAgent::~MbAgent() {

}


bool MbAgent::CheckSyncGVT() {
  return (GetLVT() % 5 == 0) || (GetLVT() > GetEndTime() - 100);
}


bool MbAgent::SendMail(unsigned long agentId, unsigned long timestamp, string mailContent) {
  return this->WriteMbString(agentId, std::move(mailContent), timestamp);
}


list<Mail> MbAgent::ReadMail(unsigned long timestamp) {
  auto newMails = this->RequestNewMails(this->agent_id(), timestamp);
  list<Mail> result;
  for (auto &i:newMails) {
    result.emplace_back(Mail(i.GetSender().GetId(), i.GetTime(), i.GetValue()->GetValueString()));
  }
  return result;
}