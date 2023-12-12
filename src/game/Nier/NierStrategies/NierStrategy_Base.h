#ifndef NIER_STRATEGY_BASE_H
#define NIER_STRATEGY_BASE_H

#include "Nier/Nier_Base.h"

class NierStrategy_Base
{
public:
    NierStrategy_Base();

    virtual void Update(uint32 pDiff, Player* pMaster, std::unordered_set<Nier_Base*> pNierSet);

public:
    bool rushing;
};
#endif
