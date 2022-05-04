#ifndef NINGER_ACTION_PRIEST_H
#define NINGER_ACTION_PRIEST_H

#include "NingerAction_Base.h"

class NingerAction_Priest :public NingerAction_Base
{
public:
    NingerAction_Priest();
    void InitializeCharacter(uint32 pmTargetLevel, uint32 pmSpecialtyTabIndex);
    void ResetTalent();
    bool InitializeEquipments(bool pmReset);
    void Prepare();
    bool Heal(Unit* pmTarget);
    bool GroupHeal(Unit* pmTarget);
    bool SimpleHeal(Unit* pmTarget);
    bool Cure(Unit* pmTarget);
    bool Buff(Unit* pmTarget);
    bool Assist(Unit* pmTarget);
    bool Revive(Player* pmTarget);

private:
    uint32 spell_Renew;
    uint32 spell_LesserHeal;
    uint32 spell_Heal;
    uint32 spell_GreaterHeal;
    uint32 spell_FlashHeal;
    uint32 spell_Resurrection;
    uint32 spell_CureDisease;
    uint32 spell_DispelMagic;
    uint32 spell_DivineSpirit;
    uint32 spell_PowerWord_Fortitude;
    uint32 spell_PowerWord_Shield;
    uint32 spell_Penance;
    uint32 spell_Weakened_Soul;
    uint32 spell_Prayer_of_Healing;
    uint32 spell_Prayer_of_Spirit;
    uint32 spell_Prayer_of_Fortitude;
};
#endif
