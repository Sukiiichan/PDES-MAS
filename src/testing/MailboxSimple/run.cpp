#include "MbAgent.h"
#include "Simulation.h"

using namespace std;
using namespace pdesmas;

int main(int argc, char **argv) {
   spdlog::set_level(spdlog::level::debug);
   Simulation sim = Simulation();
   // construct system tree
   sim.Construct(15, 8, 0, 10000);
   spdlog::info("MPI process up, rank {0}, size {1}", sim.rank(), sim.size());

   sim
         .attach_alp_to_clp(7, 3)
         .attach_alp_to_clp(8, 3)
         .attach_alp_to_clp(9, 4)
         .attach_alp_to_clp(10, 4)
         .attach_alp_to_clp(11, 5)
         .attach_alp_to_clp(12, 5)
         .attach_alp_to_clp(13, 6)
         .attach_alp_to_clp(14, 6)
         .attach_alp_to_clp(15, 7)
         .attach_alp_to_clp(15, 7)
         .attach_alp_to_clp(16, 8)
         .attach_alp_to_clp(17, 8)
         .attach_alp_to_clp(18, 9)
         .attach_alp_to_clp(19, 9)
         .attach_alp_to_clp(20, 10)
         .attach_alp_to_clp(21, 10)

         .preload_variable(1,1,1)

         .Initialise();




   // TODO attach agent to ALP, attach ALP to CLP
}