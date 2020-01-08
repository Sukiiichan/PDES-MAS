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
    sim.attach_alp_to_clp(i, (i - 1) / 2);

    spdlog::info("attached alp{0} to clp{1}", i, (i - 1) / 2);
    for (uint64_t j = 0; j < numAgents / numALP; ++j) {
      sim.init_mailbox(1000000 + i * 100 + 1 + j, i, 0);
      agIdList.push_back(1000000 + i * 100 + 1 + j);
    }
  }

//  for(auto v:agIdList){
//    // std::cout << v << " ";
//    //spdlog::info("Id list {0}", v);
//  }



  spdlog::info("MPI process up, rank {0}, size {1}", sim.rank(), sim.size());

  sim.Initialise();

  spdlog::info("Initialized, rank {0}, is {1}", sim.rank(), sim.type());
  if (sim.type() == "ALP") {
    spdlog::debug("ag initing");
    for (uint64_t i = 0; i < numAgents / numALP; ++i) {
      MbAgent *mbAg = new MbAgent(0, 10000, 1000000 + sim.rank() * 100 + 1 + i);
      mbAg->InitSendList(agIdList, 5,114514);
      sim.add_agent(mbAg);
    }
  }


  sim.Run();

  spdlog::info("LP exit, rank {0}", sim.rank());

  sim.Finalise();
}