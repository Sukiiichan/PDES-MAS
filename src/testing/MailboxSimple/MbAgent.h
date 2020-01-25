#ifndef PDES_MAS_MBAGENT_H
#define PDES_MAS_MBAGENT_H

#include <interface/Agent.h>
#include <random>
#include <ctime>
#include <string>
#include "MbSharedState.h"
#include "RouteTable.h"

using namespace pdesmas;
using namespace std;

class Mail {
public:
  unsigned long sender_;
  unsigned long timestamp_;
  string message_;

  Mail(unsigned long sender, unsigned long timestamp, string message) : sender_(sender), timestamp_(timestamp),
                                                                        message_(std::move(message)) {};
};

class MbAgent : public Agent {
public:


  MbAgent(const unsigned long startTime, const unsigned long endTime, unsigned long agentId);

  ~MbAgent() override;

  void Cycle() override;

  // void SetFrequency(unsigned int);

  void InitSendList(list<unsigned long> agList, unsigned int listLen);


private:


  list<unsigned long> sendList;

  // MA INTERFACE: if true, initiate GVT sync. Flag will be automatically cleared
  bool CheckSyncGVT();

  // MA INTERFACE: send a mail (string content)
  bool SendMail(unsigned long agentId, unsigned long timestamp, string mailContent);

  // MA INTERFACE: receive mail
  list <Mail> ReadMail(unsigned long timestamp);
};


#endif //PDES_MAS_MBAGENT_H