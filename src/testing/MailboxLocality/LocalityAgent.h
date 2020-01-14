#ifndef PDES_MAS_LOCALITYAGENT_H
#define PDES_MAS_LOCALITYAGENT_H

#include <interface/Agent.h>
#include <random>
#include <ctime>
#include <string>
#include "MbSharedState.h"
#include "RouteTable.h"

class LocalityAgent:public Agent{
public:
  LocalityAgent(const unsigned long startTime, const unsigned long endTime, unsigned long agentId);

  ~LocalityAgent();

  void Cycle() override;
};

#endif //PDES_MAS_LOCALITYAGENT_H
