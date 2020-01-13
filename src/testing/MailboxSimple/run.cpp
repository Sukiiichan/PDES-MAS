#include "MbAgent.h"
#include "Simulation.h"

using namespace std;
using namespace pdesmas;

int main(int argc, char **argv) {
  //spdlog::set_level(spdlog::level::debug);
  spdlog::set_level(spdlog::level::debug);

  Simulation sim = Simulation();
  sim.InitMPI();

  uint64_t numAgents = std::atoll(argv[1]);
  //uint64_t numAgents = 8;
  uint64_t numMPI = sim.size();
  // numMPI -> CLP and ALP
  uint64_t numALP = (numMPI + 1) / 2;
  uint64_t numCLP = numALP - 1;

  // construct system tree; execution time
  sim.Construct(numCLP, numALP, 0, 10000);
  list<unsigned long> agIdList;
  for (uint64_t i = numCLP; i < numCLP + numALP; ++i) {
    // attach alp tp clp
    // i is the rank of clp
    sim.attach_alp_to_clp(i, (i - 1) / 2);

    //spdlog::info("attached alp{0} to clp{1}", i, (i - 1) / 2);
    for (uint64_t j = 0; j < numAgents / numALP; ++j) {
      // here j is the agent index in one alp
      unsigned long agentId = 1000000 + i * 100 + 1 + j;
      sim.init_mailbox(agentId, i, 0);
      agIdList.push_back(agentId);
      MbAgent *mbAg = new MbAgent(0, 10000, agentId);
      mbAg->InitSendList(agIdList, 5);
      sim.add_agent(mbAg, i);
    }
  }


//  for(auto v:agIdList){
//    // std::cout << v << " ";
//    //spdlog::info("Id list {0}", v);
//  }



  spdlog::info("MPI process up, rank {0}, size {1}", sim.rank(), sim.size());

  sim.Initialise();

  spdlog::info("Initialized, rank {0}, is {1}", sim.rank(), sim.type());


  sim.Run();

  spdlog::info("LP exit, rank {0}", sim.rank());

  sim.Finalise();
}