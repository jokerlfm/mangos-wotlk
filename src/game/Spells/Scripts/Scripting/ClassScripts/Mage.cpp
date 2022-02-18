/*
* This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "Spells/Scripts/SpellScript.h"
#include "Spells/SpellMgr.h"

struct ArcaneConcentration : public AuraScript
{
    bool OnCheckProc(Aura* aura, ProcExecutionData& procData) const override
    {
        if (Spell* spell = procData.spell)
        {
            if (IsChanneledSpell(spell->m_spellInfo))
                return false; // these never proc
            if (SpellEntry const* spellInfo = spell->GetTriggeredByAuraSpellInfo())
            {
                if (IsChanneledSpell(spellInfo))
                {
                    if (Spell* channeledSpell = spell->GetCaster()->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                    {
                        if (channeledSpell->IsAuraProcced(aura))
                            return false;

                        channeledSpell->RegisterAuraProc(aura);
                    }
                }
            }
            spell->RegisterAuraProc(aura);
        }
        return true;
    }
};

struct MageIgnite : public AuraScript
{
    // implemented this way because we do not support proccing in spellscript on empty aura slot
    void OnPeriodicTickEnd(Aura* aura) const override
    {
        Unit* caster = aura->GetCaster();
        if (!caster)
            return;

        SpellAuraHolder* holder = caster->GetSpellAuraHolder(31658);
        if (!holder)
            holder = caster->GetSpellAuraHolder(31657);
        if (!holder)
            holder = caster->GetSpellAuraHolder(31656);
        if (!holder)
            return;

        uint32 chance = holder->GetSpellProto()->procChance;
        if (roll_chance_f(chance))
        {
            uint32 baseMana = caster->GetCreateMana();
            int32 gainedMana = baseMana * 2 / 100;
            caster->CastCustomSpell(nullptr, 67545, &gainedMana, nullptr, nullptr, TRIGGERED_OLD_TRIGGERED);
        }
    }
};

struct FingersOfFrostProc : public AuraScript
{
    bool OnCheckProc(Aura* aura, ProcExecutionData& /*data*/) const override
    {
        return roll_chance_i(aura->GetAmount());
    }
};

struct FingersOfFrostIgnore : public SpellScript
{
    void OnCast(Spell* spell) const override
    {
        spell->AddPrecastSpell(74396);
    }
};

struct FingersOfFrostDummy : public AuraScript
{
    void OnHolderInit(SpellAuraHolder* holder, WorldObject* /*caster*/) const override
    {
        holder->PresetAuraStacks(holder->GetSpellProto()->StackAmount);
    }

    void OnApply(Aura* aura, bool apply) const override
    {
        if (!apply && aura->GetRemoveMode() != AURA_REMOVE_BY_GAINED_STACK)
            aura->GetTarget()->RemoveAurasDueToSpell(44544);
    }

    SpellAuraProcResult OnProc(Aura* aura, ProcExecutionData& /*procData*/) const override
    {
        aura->GetTarget()->RemoveAuraStack(aura->GetId());
        return SPELL_AURA_PROC_OK;
    }
};

struct DeepFreezeImmunityState : public AuraScript
{
    bool OnCheckProc(Aura* /*aura*/, ProcExecutionData& data) const override
    {
        if (data.victim->IsCreature())
            if (static_cast<Creature*>(data.victim)->GetCreatureInfo()->MechanicImmuneMask & (1 << (MECHANIC_STUN - 1)))
                return true;
        return false;
    }
};

void LoadMageScripts()
{
    RegisterSpellScript<ArcaneConcentration>("spell_arcane_concentration");
    RegisterSpellScript<MageIgnite>("spell_mage_ignite");
    RegisterSpellScript<FingersOfFrostProc>("spell_fingers_of_frost_proc");
    RegisterSpellScript<FingersOfFrostIgnore>("spell_fingers_of_frost_ignore");
    RegisterSpellScript<FingersOfFrostDummy>("spell_fingers_of_frost_dummy");
    RegisterSpellScript<DeepFreezeImmunityState>("spell_deep_freeze_immunity_state");
}