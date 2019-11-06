#ifndef PDES_MAS_MBAGENT_H
#define PDES_MAS_MBAGENT_H

#include <interface/Agent.h>
#include <random>
#include <ctime>

using namespace pdesmas;
using namespace std;

class MbAgent : public Agent {
public:


    MbAgent(const unsigned long startTime1, const unsigned long endTime1, unsigned long agentId1,
            const unsigned long startTime, const unsigned long endTime, unsigned long agentId);

    void Cycle() override;

    void SetFrequency(unsigned int);

    void InitSendList(unsigned int totalNum, unsigned int listLen,unsigned int seed);


private:
    unsigned long mbcId;
    unsigned int commFrequency;
    unsigned int agentId;
    list<unsigned long> sendList;
};


#endif //PDES_MAS_MBAGENT_H