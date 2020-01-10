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

class MbAgent : public Agent {
public:


  MbAgent(const unsigned long startTime, const unsigned long endTime, unsigned long agentId);

  ~MbAgent() override;

  void Cycle() override;

  // void SetFrequency(unsigned int);

  void InitSendList(list<unsigned long> agList, unsigned int listLen, unsigned int seed);


private:
  unsigned int freqCounter;
  unsigned int frequency;
  list<unsigned long> sendList;
};


#endif //PDES_MAS_MBAGENT_H