#ifndef PDES_MAS_MBREADANTIMSG_H
#define PDES_MAS_MBREADANTIMSG_H

#include "AntiMessage.h"
#include "HasMBOwnerID.h"

namespace pdesmas {
  class MbReadAntiMsg : public AntiMessage, public HasMBOwnerID {
  private:
    static AbstractMessage *CreateInstance();

  public:
    MbReadAntiMsg();

    virtual ~MbReadAntiMsg();

    pdesmasType GetType() const;

    void Serialise(ostream &) const;

    void Deserialise(istream &);
  };
}

#endif //PDES_MAS_MBREADANTIMSG_H
