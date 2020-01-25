#include <utility>

#include "MbAgent.h"
#include <spdlog/spdlog.h>
#include <unistd.h>
#include <random>

MbAgent::MbAgent(const unsigned long startTime, const unsigned long endTime, unsigned long agentId) : Agent(
    startTime, endTime, agentId) {
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
  struct timeval start;
  struct timeval end;

  srand(agent_id() * GetLVT());

  for (auto &i:sendList) {
    int msgSerial = rand() % 100000;
    spdlog::debug("Msg serial of agent {0} is {1}", agent_id(), msgSerial);
    //tv_iter++;
    // spdlog::debug("send to {0}", i);
    // auto msgSerial = distribution(eg);
    string mailContent = "0-" + to_string(msgSerial);
    // i is receiver id, 0 means direction
    // bool tie = this->WriteMbString(i, mailContent, this->GetLVT() + 1);
    gettimeofday(&start, NULL);
    bool tie = this->SendMail(i, this->GetLVT() + 1, mailContent);
    gettimeofday(&end, NULL);
    // call ALP to send msg
    if (tie) {
      // received response msg
      spdlog::warn("LOGSEND {},{},{}", agent_id(), msgSerial,
                   1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec);
      spdlog::debug("Agent{0}, Agent{1}, MsgID{2}, at ts = {3}, sent", this->agent_id(), i, msgSerial, this->GetLVT());
      spdlog::debug("Agent{0}, Agent{1}, MsgID{2}, write success", this->agent_id(), i, msgSerial);
    } else {
      spdlog::debug("Agent{0}, Agent{1}, MsgID{2}, at ts = {3}, sent", this->agent_id(), i, msgSerial, this->GetLVT());
      spdlog::debug("Agent{0}, Agent{1}, MsgID{2}, write failed", this->agent_id(), i, msgSerial);
    }
  }

  spdlog::debug("Agent{0}, request to read mailbox", this->agent_id());
  gettimeofday(&start, NULL);
  list<Mail> newMails = ReadMail(this->GetLVT() + 1);
  gettimeofday(&end, NULL);
  spdlog::warn("LOGREAD {},{}", agent_id(),
               1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec);
  string delimiter = "-";
  spdlog::debug("Agent{0}, read mailbox success, size {1}", this->agent_id(), newMails.size());
  auto newMailIterator = newMails.begin();
  while (newMailIterator != newMails.end()) {
    unsigned long sender = newMailIterator->sender_;
    string mailContent = newMailIterator->message_;
    auto reply_tie = mailContent.substr(0, mailContent.find(delimiter));
    string serial_str = mailContent.erase(0, mailContent.find(delimiter) + delimiter.length());

    if (reply_tie == "0") {
      // generate reply
      string replyMsgContent = "1-" + serial_str;
      spdlog::debug("Agent{0},request to reply Agent{1},serial{2}", agent_id(), sender, stoi(serial_str));
      gettimeofday(&start, NULL);
      bool replyTie = this->SendMail(sender, this->GetLVT() + 1, replyMsgContent);
      gettimeofday(&end, NULL);

      if (replyTie) {
        // received response msg
        spdlog::warn("LOGSEND {},{},{}", agent_id(), serial_str,
                     1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec);
        spdlog::debug("Agent{0}, Agent{1}, MsgID{2}, write success", this->agent_id(), sender, stoi(serial_str));
      } else {
        spdlog::debug("Agent{0}, Agent{1}, MsgID{2}, write failed", this->agent_id(), sender, stoi(serial_str));
      }
    } else if (reply_tie == "1") {
      // nothing
    }
    newMailIterator++;
  }
}


MbAgent::~MbAgent() {

}


bool MbAgent::CheckSyncGVT() {
  return true;
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