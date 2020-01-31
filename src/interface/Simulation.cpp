//
// Created by pill on 19-4-23.
//

#include <parse/Initialisor.h>
#include "Simulation.h"
#include <spdlog/spdlog.h>

void Simulation::InitMPI() {
  int flag;
  MPI_Initialized(&flag);
  if (!flag) {
    int providedThreadSupport;
    MPI_Init_thread(nullptr, nullptr, MPI_THREAD_SERIALIZED, &providedThreadSupport);
    assert(providedThreadSupport == MPI_THREAD_SERIALIZED);
  }
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size_);
  MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank_);
  spdlog::debug("Process rank {}, pid {}", comm_rank_, getpid());
}

void Simulation::Construct(int number_of_clp, int number_of_alp, unsigned long start_time, unsigned long end_time) {
  InitMPI();
  number_of_clp_ = number_of_clp;
  number_of_alp_ = number_of_alp;
  start_time_ = start_time;
  end_time_ = end_time;
  for (int i = 0; i < number_of_alp + number_of_clp; ++i) {
    topology_[i] = new DummyNode();
  }
  initialisor_ = new Initialisor();
  initialisor_->InitEverything();
}

Simulation &Simulation::set_topology(const string &topo) {
  return *this;
}

Simulation &Simulation::attach_node(int from_rank, Port from_port, int to_rank, Port to_port) {
  auto from_dummynode = topology_[from_rank];
  auto to_dummynode = topology_[to_rank];
  if ((*from_dummynode)[from_port] == nullptr && (*to_dummynode)[to_port] == nullptr) {
    // ports are not connected
    from_dummynode->set(from_port, to_dummynode);
    to_dummynode->set(to_port, from_dummynode);
  }
  return *this;
}

Simulation &Simulation::attach_alp_to_clp(int alp_rank, int clp_rank) {
  initialisor_->attach_alp_to_clp(alp_rank, clp_rank);
  return *this;
}


void Simulation::Initialise() {
  MPI_Barrier(MPI_COMM_WORLD);

  int clp_max_rank = number_of_clp_ - 1;
  int alp_max_rank = clp_max_rank + number_of_alp_;

  if (comm_rank_ <= clp_max_rank) { // this instance is CLP
    clp_ = new Clp(comm_rank_, comm_size_, number_of_clp_, number_of_alp_, start_time_, end_time_, initialisor_);
    MPI_Barrier(MPI_COMM_WORLD);
  } else if (comm_rank_ <= alp_max_rank) { // is alp
    MPI_Barrier(MPI_COMM_WORLD);
    alp_ = new Alp(comm_rank_, comm_size_, number_of_clp_, number_of_alp_, start_time_, end_time_, initialisor_);
  }






//  if (comm_rank_ <= clp_max_rank) { // this instance is CLP
//    clp_ = new Clp(comm_rank_, comm_size_, number_of_clp_, number_of_alp_, start_time_, end_time_, initialisor_);
//  } else if (comm_rank_ <= alp_max_rank) { // is alp
//    alp_ = new Alp(comm_rank_, comm_size_, number_of_clp_, number_of_alp_, start_time_, end_time_, initialisor_);
//  } else {
//    spdlog::warn("Unused process, rank={0}", comm_rank_);
//    Finalise();
//    exit(0);
//  }
}

void Simulation::Run() {
  MPI_Barrier(MPI_COMM_WORLD);

  if (this->type() == "ALP" && this->alp_ != nullptr) {

    for (auto a:alp_to_agent_list_map_[this->rank()]) {
      this->alp_->AddAgent(a);
      spdlog::debug("ALP {} adding agent {}", this->rank(), a->agent_id());
    }

    spdlog::info("ALP {}, {} agents attached", this->rank(), this->alp_->GetNumOfAttachedAgents());

    this->alp_->StartAllAgents();
    spdlog::debug("All agents started");
    spdlog::debug("ALP running {0}", this->alp_->GetRank());
    alp_->Run();


  } else if (this->type() == "CLP" && this->clp_ != nullptr) {
    spdlog::debug("CLP running {0}", this->clp_->GetRank());

    clp_->Run();
  }
}

void Simulation::Finalise() {
  MPI_Finalize();
}


unsigned long Simulation::GVT() {
  if (this->clp_ != nullptr) {
    return this->clp_->GetGvt();
  } else if (this->alp_ != nullptr) {
    return this->alp_->GetGvt();
  }
  return ULONG_MAX;
}

Simulation::Simulation() {

}

Simulation::~Simulation() {

}

string Simulation::type() {
  if (this->alp_ != nullptr) {
    return "ALP";
  } else if (this->clp_ != nullptr) {
    return "CLP";
  }
  return "NONE";
}

Simulation Simulation::add_agent(Agent *agent, int toAlp) {
  // record info first

  if (alp_to_agent_list_map_.find(toAlp) == alp_to_agent_list_map_.end()) {
    alp_to_agent_list_map_[toAlp] = list<Agent *>();
  }
  alp_to_agent_list_map_[toAlp].push_back(agent);
  initialisor_->load_agent_info(agent->agent_id(), toAlp);

  return *this;

}


Simulation &Simulation::init_mailbox(unsigned long agentId, int alpRank, int clpId) {
  initialisor_->preload_mailbox(agentId, alpRank, clpId);
  return *this;
}


Simulation &Simulation::preload_variable(unsigned long ssvId, int v, int clpId) {

  initialisor_->preload_variable("INT", ssvId, to_string(v), clpId);
  return *this;
}

Simulation &Simulation::preload_variable(unsigned long ssvId, double v, int clpId) {

  initialisor_->preload_variable("DOUBLE", ssvId, to_string(v), clpId);
  return *this;
}

Simulation &Simulation::preload_variable(unsigned long ssvId, Point v, int clpId) {

  std::ostringstream stream;
  v.Serialise(stream);
  initialisor_->preload_variable("POINT", ssvId, stream.str(), clpId);
  return *this;
}

Simulation &Simulation::preload_variable(unsigned long ssvId, string v, int clpId) {

  initialisor_->preload_variable("STRING", ssvId, v, clpId);
  return *this;
}

//Simulation &Simulation::preload_variable(unsigned long agentId, int clpId) {
//
//  // TODO complete preload without mbvId
//    initialisor_->preload_variable();
//}