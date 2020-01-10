/*
 * Initialisor.h
 *
 *  Created on: 18 Aug 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef INITIALISOR_H_
#define INITIALISOR_H_

#include <string>
#include <list>
#include <lp/Clp.h>
#include "ForwardingTable.h"
#include "Direction.h"
#include "Range.h"

using namespace std;

namespace pdesmas {

#define MAX_LINE_LENGTH 80

  class Initialisor {
  private:
    bool fHasInitInt;
    bool fHasInitLong;
    bool fHasInitDouble;
    bool fHasInitPoint;
    bool fHasInitString;

    map<unsigned int, Range> fClpIdRangeMap;
    map<unsigned int, list<SsvId> > fClpIdSsvIdMap;
    map<unsigned int, list<unsigned long>> fClpIdAgentMbIdMap;
    map<unsigned int, unsigned int> fAlpToClpMap;
    map<SsvId, AbstractValue *> fClpSsvIdValueMap;
    map<unsigned long, int> fAgentIdAlpRankMap;

    void ParseMessage(const string) const;

    void ParseALP(const string);

    void ParseSSV(const string, int);

    void ParseSSVForALP(const string);

    void ParseCLP(const string);

    void InitType(const string);


  public:
    Initialisor();

    ~Initialisor();

    void ParseFileCLP(const string, int);

    void ParseFileALP(const string);

    void attach_alp_to_clp(int alp, int clp);

    void preload_mailbox(unsigned long agentId, int agentRank, int clpId);

    void preload_variable(const string &type, unsigned long variable_id, const string &value, unsigned int clpId);

    void load_agent_info(unsigned long agentId, int attachedAlpRank);

    void InitEverything();

    const map<unsigned int, Range> &GetClpToRangeMap() const;

    const map<unsigned int, list<SsvId> > &GetClpToSsvMap() const;

    const map<unsigned int, unsigned int> &GetAlpToClpMap() const;

    const map<SsvId, AbstractValue *> &GetClpSsvIdValueMap() const;

    const map<unsigned int, list<unsigned long>> &GetClpToMbMap() const;

    const map<unsigned long, int> &GetAgentIdAlpRankMap() const;
  };
}

#endif /* INITIALISOR_H_ */
