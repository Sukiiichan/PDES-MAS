#include "MbAgent.h"
#include "Simulation.h"

using namespace std;
using namespace pdesmas;

int main(int argc, char **argv) {
  spdlog::set_level(spdlog::level::debug);
  Simulation sim = Simulation();
  sim.InitMPI();

  // uint64_t numAgents = std::atoll(argv[1]);
  uint64_t numAgents = 32;
  uint64_t numMPI = sim.size();
  // numMPI -> CLP and ALP
  uint64_t numALP = (numMPI + 1) / 2;
  uint64_t numCLP = numALP - 1;

  // construct system tree; execution time
  sim.Construct(numCLP, numALP, 0, 10000);

  for (int i = numCLP; i < numCLP + numALP; ++i) {
    // attach alp tp clp
    sim.attach_alp_to_clp(i, (i - 1) / 2);

    spdlog::info("attached alp{0} to clp{1}", i, (i - 1) / 2);

  }

  for (int i = 0; i < numAgents; ++i) {
    sim.init_mailbox(i, 0);
    // put all mb in top node
  }
  spdlog::info("MPI process up, rank {0}, size {1}", sim.rank(), sim.size());

  sim.Initialise();

  spdlog::info("Initialized, rank {0}, is {1}", sim.rank(), sim.type());
  if (sim.type() == "ALP") {
    for (int i = 0; i < numAgents; ++i) {
      MbAgent *mbAg = new MbAgent(0, 10000, 10000 + sim.rank() * 100 + 1 + i);
      sim.add_agent(mbAg);

    }
  }

  sim.Run();

  spdlog::info("LP exit, rank {0}", sim.rank());


}