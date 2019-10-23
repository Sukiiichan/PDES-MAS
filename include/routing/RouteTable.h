#ifndef _ROUTERTABLE_H_
#define _ROUTERTABLE_H_
/*!
 \brief An intermediate route table class
 
 This class contains the information of forwarding table and routing
 table used both by the Router Class and the Load balancing
 algorithm. The class will get the initial routing information from the
 InitialStateParser instance. The class then present the map between
 CLP/SSV ID to the port it should be routed.
 */

#include "ForwardingTable.h"
#include "Direction.h"
#include "SsvId.h"
#include "LpId.h"
using namespace std;

namespace pdesmas {
  class Initialisor;

  class RouteTable {
    private:
      map<unsigned int, Direction> fRankToDirectionMap;
      map<Direction, unsigned int> fDirectionToRankMap;
      map<SsvId, Direction> fSSVIDToDirectionMap;
     map<LpId, SsvId> MailboxAgentMap; // map agentid with mailbox variable id, same as the map in mbsharedstate


  public:
      RouteTable();
      ~RouteTable();
      RouteTable(unsigned int, unsigned int, const Initialisor*);

      Direction GetDirectionFromMbOwner(const LpId&) const;
      Direction GetDirectionFromSsvId(const SsvId&) const;
      Direction GetDirectionFromRank(unsigned int) const;
      unsigned int GetRankFromDirection(const Direction) const;
      void SetSsvIdHost(SsvId, Direction);
  };
}
#endif
