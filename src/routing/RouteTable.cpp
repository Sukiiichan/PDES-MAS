#include "RouteTable.h"
#include "Initialisor.h"
#include <math.h>
#include "Log.h"
#include "spdlog/spdlog.h"
using namespace pdesmas;
using namespace std;

RouteTable::RouteTable() {
  // Nothing to do
}

RouteTable::~RouteTable() {
  fRankToDirectionMap.clear();
  fDirectionToRankMap.clear();
  fSSVIDToDirectionMap.clear();
  fMbIdToDirectionMap.clear();
}

RouteTable::RouteTable(unsigned int pLpRank, unsigned int pNumberOfClps, const Initialisor* pInitialisor) {
  //spdlog::info("Route table init");
  ForwardingTable forwardingTable = ForwardingTable(pLpRank, pNumberOfClps);
  map<unsigned int, list<SsvId> > clpToSsvMap = pInitialisor->GetClpToSsvMap();
  map<unsigned int, list<unsigned long>> clpIdAgentMbIdMap=pInitialisor->GetClpToMbMap();
  //map<unsigned int, list<SsvId> >::iterator clpIdSsvIdMapIterator;
  //map<unsigned int, list<unsigned long>>::iterator clpIdAgentMbIdMapIterator;
  //spdlog::info("clpToSsvMap {0}",clpToSsvMap.size());
  for(auto clpIdSsvIdMapIterator = clpToSsvMap.begin(); clpIdSsvIdMapIterator != clpToSsvMap.end(); ++clpIdSsvIdMapIterator) {
    list<SsvId>::iterator ssvIdListIterator;
    for(ssvIdListIterator = clpIdSsvIdMapIterator->second.begin(); ssvIdListIterator != clpIdSsvIdMapIterator->second.end(); ++ssvIdListIterator) {
      if (clpIdSsvIdMapIterator->first == pLpRank) {
        fSSVIDToDirectionMap.insert(make_pair(*ssvIdListIterator, HERE));
      } else {
        RoutingInfo routingInfo = forwardingTable.GetRoutingInfo(clpIdSsvIdMapIterator->first);
        unsigned int nextClp = routingInfo.GetNextNodeRank();
        if (nextClp == ((pLpRank - 1) / 2)) {
          fSSVIDToDirectionMap.insert(make_pair(*ssvIdListIterator, UP));
        } else if (nextClp == ((pLpRank * 2) + 1)) {
          fSSVIDToDirectionMap.insert(make_pair(*ssvIdListIterator, LEFT));
        } else if (nextClp == ((pLpRank * 2) + 2)) {
          fSSVIDToDirectionMap.insert(make_pair(*ssvIdListIterator, RIGHT));
        } else {
          LOG(logWARNING) << "RouteTable::RouteTable# Could not find direction for SSVID while initialising: SSVID: " << *ssvIdListIterator << ", next CLP: " << nextClp << ", this LP: " << pLpRank;
        }
      }
    }
  }

  for(auto clpIdAgentMbIdMapIterator = clpIdAgentMbIdMap.begin(); clpIdAgentMbIdMapIterator != clpIdAgentMbIdMap.end(); ++clpIdAgentMbIdMapIterator) {
    list<unsigned long>::iterator agentIdListIterator;
    for(agentIdListIterator = clpIdAgentMbIdMapIterator->second.begin(); agentIdListIterator != clpIdAgentMbIdMapIterator->second.end(); ++agentIdListIterator) {
      if (clpIdAgentMbIdMapIterator->first == pLpRank) {
        fMbIdToDirectionMap.insert(make_pair(*agentIdListIterator, HERE));
      } else {
        RoutingInfo routingInfo = forwardingTable.GetRoutingInfo(clpIdAgentMbIdMapIterator->first);
        unsigned int nextClp = routingInfo.GetNextNodeRank();
        if (nextClp == ((pLpRank - 1) / 2)) {
          fMbIdToDirectionMap.insert(make_pair(*agentIdListIterator, UP));
        } else if (nextClp == ((pLpRank * 2) + 1)) {
          fMbIdToDirectionMap.insert(make_pair(*agentIdListIterator, LEFT));
        } else if (nextClp == ((pLpRank * 2) + 2)) {
          fMbIdToDirectionMap.insert(make_pair(*agentIdListIterator, RIGHT));
        } else {
          LOG(logWARNING) << "RouteTable::RouteTable# Could not find direction for SSVID while initialising: SSVID: " << *agentIdListIterator << ", next CLP: " << nextClp << ", this LP: " << pLpRank;
        }
      }
    }
  }



  fRankToDirectionMap[pLpRank] = HERE;

  Direction direction;
  if (pLpRank == 0) direction = LEFT;
  else direction = UP;
  // For the CLPs, add the neighbours to this LP to both tables.
  for (unsigned int rank = 0; rank < pNumberOfClps; ++rank) {
    if (rank == pLpRank) continue;
    RoutingInfo routingTable = forwardingTable.GetRoutingInfo(rank);
    int hops = routingTable.GetHops();
    if (hops <= 1) {
      fRankToDirectionMap[rank] = direction;
      fDirectionToRankMap[direction] = rank;
      ++direction;
    }
  }
  // For the remaining CLPs, add the proper direction to both tables.
  for (unsigned int rank = 0; rank < pNumberOfClps; ++rank) {
    if (fRankToDirectionMap.find(rank) == fRankToDirectionMap.end()) {
      RoutingInfo routingTable = forwardingTable.GetRoutingInfo(rank);
      int hops = routingTable.GetHops();
      if (hops > 1) {
        Direction newDirection = fRankToDirectionMap[routingTable.GetNextNodeRank()];
        fRankToDirectionMap[rank] = newDirection;
      }
    }
  }
  // For the ALPs, add the proper direction to both tables.
  map<unsigned int, unsigned int> alpToClpMap = pInitialisor->GetAlpToClpMap();
  for (unsigned int rank = pNumberOfClps; rank < (pNumberOfClps + alpToClpMap.size()); ++rank) {
    if (alpToClpMap[rank] == pLpRank) {
      fRankToDirectionMap[rank] = direction;
      fDirectionToRankMap[direction] = rank;
      ++direction;
    } else {
      Direction newDirection = fRankToDirectionMap[alpToClpMap[rank]];
      fRankToDirectionMap[rank] = newDirection;
    }
  }
}

Direction RouteTable::GetDirectionFromSsvId(const SsvId& pSSVID) const {
  map<SsvId, Direction>::const_iterator iter = fSSVIDToDirectionMap.find(pSSVID);
  if (iter != fSSVIDToDirectionMap.end()) return iter->second;
  LOG(logERROR) << "RouteTable::GetDirectionFromSsvId# Could not find direction for SsvId: " << pSSVID;
  return DIRECTION_SIZE;
}

Direction RouteTable::GetDirectionFromRank(unsigned int pRank) const {
  map<unsigned int, Direction>::const_iterator iter = fRankToDirectionMap.find(pRank);
  if (iter != fRankToDirectionMap.end()) return iter->second;
  LOG(logERROR) << "RouteTable::GetDirectionFromRank# Could not find direction for rank: " << pRank;
  exit(1);
}

unsigned int RouteTable::GetRankFromDirection(const Direction pDirection) const {
  map<Direction, unsigned int>::const_iterator iter = fDirectionToRankMap.find(pDirection);
  if (iter != fDirectionToRankMap.end()) return iter->second;
  LOG(logERROR) << "RouteTable::GetRankFromDirection# Could not find rank for direction" << pDirection;
  exit(1);
}

void RouteTable::SetSsvIdHost(SsvId pSSVID, Direction pDirection) {
  fSSVIDToDirectionMap[pSSVID] = pDirection;
}
void RouteTable::SetMbSsvIdHost(SsvId pSSVID, Direction pDirection) {
  fMbIdToDirectionMap[pSSVID.id()] = pDirection;
}
Direction RouteTable::GetDirectionFromMbOwnerId(const unsigned long pOwnerId) const {
   //SsvId mbvId = MailboxAgentMap.find(pOwnerId)->second;
//  SsvId mbvId = SsvId(pOwnerId);
//  spdlog::debug("--------size of fMbIdToDirectionMap: {}",fMbIdToDirectionMap.size());
//  for(auto it:fMbIdToDirectionMap){
//    spdlog::debug("--------{},{}",it.first,it.second);
//  }

  auto directionMapIterator = fMbIdToDirectionMap.find(pOwnerId);
   if(directionMapIterator != fMbIdToDirectionMap.end()){
      return directionMapIterator->second;
   }
   LOG(logERROR) <<"GetDirectionFromMbOwnerId("<<pOwnerId<<") no direction found";
   return DIRECTION_SIZE;
}

void RouteTable::SetMailboxAgentMap(const map<unsigned long, SsvId> &maMap) {
  MailboxAgentMap = maMap;
}