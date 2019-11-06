#ifndef PDES_MAS_MBMAIL_H
#define PDES_MAS_MBMAIL_H


using namespace std;

#include "Serialisable.h"
#include "ObjectMgr.h"
#include "SerialisableMultiMap.h"
#include "Log.h"
#include "RollbackList.h"

namespace pdesmas {
    class MbMail : public Serialisable {
    private:
        unsigned long fTime;
        LpId fSender;
        AbstractValue *fValue;

    public:
        MbMail();

        MbMail(unsigned long, const AbstractValue *, const LpId &);

        MbMail(const MbMail &);

        ~ MbMail();

        AbstractValue *Read(unsigned long);
        // void DelOldMsg(unsigned long, unsigned long); // delete old message to recycle memory space
        // void RemoveMsg(unsigned long, const LpId &);


        void SetTime(unsigned long);

        unsigned long GetTime() const;

        void SetSender(const LpId &);

        const LpId &GetSender() const;

        void SetValue(const AbstractValue *);

        AbstractValue *GetValueCopy() const;

        void Serialise(ostream &) const;

        void Deserialise(istream &);
    };
}

#endif