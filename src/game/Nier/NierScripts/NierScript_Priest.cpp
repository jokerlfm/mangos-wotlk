#include "NierScript_Priest.h"
#include "../NierManager.h"

NierScript_Priest::NierScript_Priest(Player* pMe) :NierScript_Base(pMe)
{
	spell_Shoot = 0;

	spell_CircleOfHealing = 0;
	spell_MindFlay = 0;
	spell_DesperatePrayer = 0;

	spell_Smite = 0;
	spell_Renew = 0;
	spell_LesserHeal = 0;
	spell_ShadowWord_Pain = 0;
	spell_MindBlast = 0;
	spell_InnerFire = 0;
	spell_ApolishDisease = 0;
	spell_HolyFire = 0;
	spell_ShadowProtection = 0;
	spell_ShackleUndead = 0;
	spell_MindSoothe = 0;
	spell_PsychicScream = 0;
	spell_Levitate = 0;
	spell_DevouringPlague = 0;
	spell_Fade = 0;
	spell_Shadowfiend = 0;
	spell_MindVision = 0;
	spell_ShadowWord_Death = 0;
	spell_MindControl = 0;
	spell_PrayerOfMending = 0;
	spell_FearWard = 0;
	spell_PrayerOfHealing = 0;
	spell_PrayerOfShadowProtection = 0;
	spell_PrayerOfFortitude = 0;
	spell_PrayerOfSpirit = 0;

	spell_Heal = 0;
	spell_GreaterHeal = 0;
	spell_FlashHeal = 0;
	spell_Resurrection = 0;
	spell_CureDisease = 0;
	spell_DispelMagic = 0;
	spell_DivineSpirit = 0;
	spell_PowerWord_Fortitude = 0;
	spell_PowerWord_Shield = 0;
	spell_Penance = 0;
	spell_Weakened_Soul = 6788;
	spell_Prayer_of_Healing = 0;
	spell_Prayer_of_Spirit = 0;
	spell_Prayer_of_Fortitude = 0;
	spell_InnerFocus = 0;
	spell_PowerInfusion = 0;
	spell_PainSuppression = 0;
	spell_DesperatePrayer = 0;
	spell_HolyNova = 0;
	spell_Prayer_Of_Mending = 0;
	spell_GuardianSpirit = 0;
	aura_Surge_of_Light = 0;

	healDistance = 30.0f;
	followDistance = 20.0f;
	dpsDistance = 25.0f;
}

bool NierScript_Priest::Prepare()
{
	if (NierScript_Base::Prepare())
	{

	}

	return false;
}

void NierScript_Priest::Update(uint32 pDiff)
{
	NierScript_Base::Update(pDiff);
}

void NierScript_Priest::InitializeCharacter()
{
	NierScript_Base::InitializeCharacter();

	me->groupRole = GroupRole::GroupRole_Healer;

	uint32 myLevel = me->GetLevel();
	// stave 
	me->learnSpell(227, true);
	// wand
	me->learnSpell(5009, true);
	spell_Shoot = 5019;
	spell_Smite = 585;

	spell_LesserHeal = 2050;
	spell_PowerWord_Fortitude = 1243;
	if (myLevel >= 4)
	{
		spell_LesserHeal = 2052;
	}
	if (myLevel >= 6)
	{
		spell_PowerWord_Shield = 17;
	}
	if (myLevel >= 8)
	{
		spell_Renew = 139;
	}
	if (myLevel >= 10)
	{
		spell_LesserHeal = 2053;
		spell_Resurrection = 2006;
	}
	if (myLevel >= 12)
	{
		spell_PowerWord_Fortitude = 1244;
		spell_PowerWord_Shield = 592;
	}
	if (myLevel >= 14)
	{
		spell_Renew = 6074;
		spell_CureDisease = 528;
	}
	if (myLevel >= 16)
	{
		spell_Heal = 2054;
	}
	if (myLevel >= 18)
	{
		spell_DispelMagic = 527;
		spell_PowerWord_Shield = 600;
	}
	if (myLevel >= 20)
	{
		spell_InnerFocus = 14751;
		spell_Renew = 6075;
		spell_FlashHeal = 2061;
		spell_HolyNova = 15237;
	}
	if (myLevel >= 22)
	{
		spell_Heal = 2055;
		spell_Resurrection = 2010;
	}
	if (myLevel >= 24)
	{
		spell_PowerWord_Fortitude = 1245;
		spell_PowerWord_Shield = 3747;
	}
	if (myLevel >= 26)
	{
		spell_Renew = 6076;
		spell_FlashHeal = 9472;
	}
	if (myLevel >= 28)
	{
		spell_Heal = 6063;
		spell_HolyNova = 15430;
	}
	if (myLevel >= 30)
	{
		spell_DivineSpirit = 14752;
		spell_PowerWord_Shield = 6065;
		spell_Prayer_of_Healing = 596;
	}
	if (myLevel >= 32)
	{
		spell_Renew = 6077;
		spell_FlashHeal = 9473;
	}
	if (myLevel >= 34)
	{
		spell_Heal = 6064;
		spell_Resurrection = 10880;
	}
	if (myLevel >= 36)
	{
		spell_PowerWord_Fortitude = 2791;
		spell_PowerWord_Shield = 6066;
		aura_Surge_of_Light = 33151;
		spell_HolyNova = 15431;
	}
	if (myLevel >= 38)
	{
		spell_Renew = 6078;
		spell_FlashHeal = 9474;
	}
	if (myLevel >= 40)
	{
		spell_GreaterHeal = 2060;
		spell_DivineSpirit = 14818;
		spell_Prayer_of_Healing = 996;
		spell_PowerInfusion = 10060;
	}
	if (myLevel >= 42)
	{
		spell_PowerWord_Shield = 10898;
	}
	if (myLevel >= 44)
	{
		spell_Renew = 10927;
		spell_FlashHeal = 10915;
		spell_HolyNova = 27799;
	}
	if (myLevel >= 46)
	{
		spell_GreaterHeal = 10963;
		spell_Resurrection = 10881;
	}
	if (myLevel >= 48)
	{
		spell_PowerWord_Fortitude = 10937;
		spell_PowerWord_Shield = 10899;
		spell_Prayer_of_Fortitude = 21562;
	}
	if (myLevel >= 50)
	{
		spell_Renew = 10928;
		spell_FlashHeal = 10916;
		spell_DivineSpirit = 14819;
		spell_Prayer_of_Healing = 10960;
		spell_PainSuppression = 33206;
	}
	if (myLevel >= 52)
	{
		spell_GreaterHeal = 10964;
		spell_HolyNova = 27800;
	}
	if (myLevel >= 54)
	{
		spell_PowerWord_Shield = 10900;
	}
	if (myLevel >= 56)
	{
		spell_Renew = 10929;
		spell_FlashHeal = 10917;
	}
	if (myLevel >= 58)
	{
		spell_GreaterHeal = 10965;
		spell_Resurrection = 20770;
	}
	if (myLevel >= 60)
	{
		spell_Renew = 25315;
		spell_GreaterHeal = 25314;
		spell_DivineSpirit = 27841;
		spell_PowerWord_Fortitude = 10938;
		spell_Penance = 47540;
		spell_PowerWord_Shield = 10901;
		spell_Prayer_of_Spirit = 27681;
		spell_Prayer_of_Fortitude = 21564;
		spell_Prayer_of_Healing = 10961;
		spell_GuardianSpirit = 47788;
		spell_HolyNova = 27801;
	}
	if (myLevel >= 61)
	{
		spell_FlashHeal = 25233;
	}
	if (myLevel >= 63)
	{
		spell_GreaterHeal = 25210;
	}
	if (myLevel >= 65)
	{
		spell_Renew = 25221;
		spell_PowerWord_Shield = 25217;
	}
	if (myLevel >= 67)
	{
		spell_FlashHeal = 25235;
	}
	if (myLevel >= 68)
	{
		spell_GreaterHeal = 25213;
		spell_Resurrection = 25435;
		spell_Prayer_of_Healing = 25308;
		spell_HolyNova = 25331;
		spell_Prayer_Of_Mending = 33076;
	}
	if (myLevel >= 70)
	{
		spell_Renew = 25222;
		spell_DivineSpirit = 25312;
		spell_PowerWord_Fortitude = 25389;
		spell_Penance = 53005;
		spell_PowerWord_Shield = 25218;
		spell_Prayer_of_Spirit = 32999;
		spell_Prayer_of_Fortitude = 25392;
	}
	if (myLevel >= 73)
	{
		spell_GreaterHeal = 48062;
		spell_FlashHeal = 48070;
	}
	if (myLevel >= 74)
	{
		spell_InnerFocus = 14751;
		spell_Prayer_Of_Mending = 48112;
	}
	if (myLevel >= 75)
	{
		spell_Renew = 48067;
		spell_Penance = 53006;
		spell_PowerWord_Shield = 48065;
		spell_HolyNova = 48007;
	}
	if (myLevel >= 76)
	{
		spell_Prayer_of_Healing = 48072;
	}
	if (myLevel >= 78)
	{
		spell_GreaterHeal = 48063;
		spell_Resurrection = 48171;
	}
	if (myLevel >= 79)
	{
		spell_FlashHeal = 48071;
		spell_Prayer_Of_Mending = 48113;
	}
	if (myLevel >= 80)
	{
		spell_Renew = 48068;
		spell_DivineSpirit = 48073;
		spell_PowerWord_Fortitude = 48161;
		spell_Penance = 53007;
		spell_PowerWord_Shield = 48066;
		spell_Prayer_of_Spirit = 48074;
		spell_Prayer_of_Fortitude = 48162;
		spell_HolyNova = 48078;
	}
	me->UpdateSkillsForLevel(true);
}

void NierScript_Priest::LearnTalents()
{
	NierScript_Base::LearnTalents();

	specialty = 1;
	// talent tab : 201 - discipline, 202 - holy, 203 - shadow
	switch (specialty)
	{
	case 0:
	{
		// discipline
		LearnTalent(1898);

		LearnTalent(344);
		LearnTalent(352);

		LearnTalent(348);
		LearnTalent(343);
		LearnTalent(347);

		LearnTalent(341);

		LearnTalent(351);
		LearnTalent(1201);

		LearnTalent(1771);
		LearnTalent(1772, 2);

		LearnTalent(322);
		LearnTalent(1772);
		LearnTalent(1773);

		LearnTalent(2235);
		LearnTalent(1896);

		LearnTalent(1774);
		LearnTalent(1894);
		LearnTalent(1901);

		LearnTalent(1202);

		LearnTalent(1897);
		LearnTalent(1895);

		LearnTalent(1858);

		LearnTalent(406);
		LearnTalent(401);

		LearnTalent(1181);

		LearnTalent(442);

		break;
	}
	case 1:
	{
		// holy
		LearnTalent(406);
		LearnTalent(401);
		LearnTalent(1181);
		LearnTalent(361);
		LearnTalent(408);
		LearnTalent(442);
		LearnTalent(1561);
		LearnTalent(402);
		LearnTalent(1766);
		LearnTalent(404);
		LearnTalent(1768);
		LearnTalent(2279);
		LearnTalent(1767);
		LearnTalent(1902);
		LearnTalent(1815);
		LearnTalent(1903);
		LearnTalent(1911);
		break;
	}
	case 2:
	{
		// shadow
		LearnTalent(462);
		LearnTalent(466);
		LearnTalent(482);
		LearnTalent(463);
		LearnTalent(501);
		LearnTalent(542);
		LearnTalent(461);
		LearnTalent(881);
		LearnTalent(541);
		LearnTalent(484);
		LearnTalent(1777);
		LearnTalent(1781);
		LearnTalent(521);
		LearnTalent(2267);
		LearnTalent(1778);
		LearnTalent(1816);
		LearnTalent(1779);
		LearnTalent(1908);
		LearnTalent(1909);
		LearnTalent(1907);
		LearnTalent(1910);

		break;
	}
	default:
	{
		break;
	}
	}

	me->SendTalentsInfoData(false);

}

void NierScript_Priest::InitializeEquipments(bool pReset)
{
	NierScript_Base::InitializeEquipments(pReset);

	int requiredLevel = me->GetLevel();

	uint32 equipSlot = 0;
	uint32 inventoryType = 0;
	uint32 itemClass = 0;
	uint32 itemSubClass = ItemSubclassArmor::ITEM_SUBCLASS_ARMOR_CLOTH;
	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_CHEST;
	inventoryType = InventoryType::INVTYPE_CHEST;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_FEET;
	inventoryType = InventoryType::INVTYPE_FEET;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_HANDS;
	inventoryType = InventoryType::INVTYPE_HANDS;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_HEAD;
	inventoryType = InventoryType::INVTYPE_HEAD;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_LEGS;
	inventoryType = InventoryType::INVTYPE_LEGS;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_SHOULDERS;
	inventoryType = InventoryType::INVTYPE_SHOULDERS;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_WAIST;
	inventoryType = InventoryType::INVTYPE_WAIST;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_WRISTS;
	inventoryType = InventoryType::INVTYPE_WRISTS;
	itemClass = ItemClass::ITEM_CLASS_ARMOR;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);

	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_MAINHAND;
	itemClass = ItemClass::ITEM_CLASS_WEAPON;
	inventoryType = InventoryType::INVTYPE_2HWEAPON;
	itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_STAFF;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);

	equipSlot = EquipmentSlots::EQUIPMENT_SLOT_RANGED;
	itemClass = ItemClass::ITEM_CLASS_WEAPON;
	inventoryType = InventoryType::INVTYPE_RANGEDRIGHT;
	itemSubClass = ItemSubclassWeapon::ITEM_SUBCLASS_WEAPON_WAND;
	EuipRandom(equipSlot, inventoryType, itemClass, itemSubClass, requiredLevel);
}

bool NierScript_Priest::Tank(Unit* pTarget)
{
	return NierScript_Base::Tank(pTarget);
}

bool NierScript_Priest::Heal(Unit* pTarget)
{
	if (me->IsNonMeleeSpellCasted(false, false, true))
	{
		return true;
	}
	float targetHealthPct = pTarget->GetHealthPercent();
	if (targetHealthPct > 90.0f)
	{
		return false;
	}
	if (Player* targetPlayer = (Player*)pTarget)
	{
		if (targetPlayer->groupRole != GroupRole::GroupRole_Tank)
		{
			if (targetHealthPct > 40.0f)
			{
				return false;
			}
		}
	}
	if (aura_Surge_of_Light > 0)
	{
		if (me->HasAura(aura_Surge_of_Light))
		{
			if (CastSpell(pTarget, spell_FlashHeal))
			{
				return true;
			}
		}
	}
	if (targetHealthPct < 30.0f)
	{
		if (spell_PowerWord_Shield > 0)
		{
			if (!pTarget->HasAura(spell_Weakened_Soul))
			{
				if (CastSpell(pTarget, spell_PowerWord_Shield))
				{
					return true;
				}
			}
		}
		if (spell_GuardianSpirit > 0)
		{
			if (CastSpell(pTarget, spell_GuardianSpirit))
			{
				return true;
			}
		}
		if (spell_FlashHeal > 0)
		{
			if (CastSpell(pTarget, spell_FlashHeal))
			{
				return true;
			}
		}
	}
	if (spell_Renew > 0)
	{
		if (CastSpell(pTarget, spell_Renew, true, true))
		{
			return true;
		}
		if (spell_Prayer_Of_Mending > 0)
		{
			if (CastSpell(pTarget, spell_Prayer_Of_Mending, true))
			{
				return true;
			}
		}
	}
	if (targetHealthPct < 70.0f)
	{
		if (spell_PowerWord_Shield > 0)
		{
			if (!pTarget->HasAura(spell_Weakened_Soul))
			{
				if (CastSpell(pTarget, spell_PowerWord_Shield))
				{
					return true;
				}
			}
		}
		if (spell_GreaterHeal > 0)
		{
			if (spell_InnerFocus > 0)
			{
				CastSpell(me, spell_InnerFocus);
			}
			if (CastSpell(pTarget, spell_GreaterHeal))
			{
				return true;
			}
		}
		if (spell_Heal > 0)
		{
			if (CastSpell(pTarget, spell_Heal))
			{
				return true;
			}
		}
		if (spell_LesserHeal > 0)
		{
			if (CastSpell(pTarget, spell_LesserHeal))
			{
				return true;
			}
		}
	}

	return false;
}

bool NierScript_Priest::DPS(Unit* pTarget)
{
	if (spell_Shoot > 0)
	{
		if (Spell* shooting = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_AUTOREPEAT_SPELL))
		{
			return true;
		}
		else
		{
			if (CastSpell(pTarget, spell_Shoot))
			{
				return true;
			}
		}
	}

	return true;
}

bool NierScript_Priest::PVP(Unit* pTarget)
{
	if (spell_Shoot > 0)
	{
		if (Spell* shooting = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_AUTOREPEAT_SPELL))
		{
			return true;
		}
		else
		{
			if (CastSpell(pTarget, spell_Shoot))
			{
				return true;
			}
		}
	}

	return true;
}

bool NierScript_Priest::Cure()
{
	if (spell_DispelMagic > 0 || spell_CureDisease > 0)
	{
		if (cureDelay > 0)
		{
			return false;
		}
		cureDelay = 2000;
		if (Group* group = me->GetGroup())
		{
			for (GroupReference* groupRef = group->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				if (Player* member = groupRef->getSource())
				{
					if (!member->IsAlive())
					{
						continue;
					}
					if (me->GetDistance(member) > VISIBILITY_DISTANCE_TINY)
					{
						continue;
					}

					if (spell_DispelMagic > 0)
					{
						uint32 dispelMask = GetDispellMask(DispelType::DISPEL_MAGIC);
						Unit::SpellAuraHolderMap const& auras = member->GetSpellAuraHolderMap();
						for (Unit::SpellAuraHolderMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
						{
							if (SpellAuraHolder* holder = itr->second)
							{
								if (!holder->IsPositive())
								{
									if (holder->GetAuraDuration() > 20000)
									{
										if (holder->IsDispellableByMask(dispelMask, holder->GetCaster(), holder->GetSpellProto()))
										{
											if (const SpellEntry* se = holder->GetSpellProto())
											{
												if (se->Dispel == DispelType::DISPEL_MAGIC)
												{
													if (CastSpell(member, spell_DispelMagic))
													{
														return true;
													}
													break;
												}
											}
										}
									}
								}
							}
						}
					}
					if (spell_CureDisease > 0)
					{
						uint32 dispelMask = GetDispellMask(DispelType::DISPEL_DISEASE);
						Unit::SpellAuraHolderMap const& auras = member->GetSpellAuraHolderMap();
						for (Unit::SpellAuraHolderMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
						{
							if (SpellAuraHolder* holder = itr->second)
							{
								if (!holder->IsPositive())
								{
									if (holder->GetAuraDuration() > 20000)
									{
										if (holder->IsDispellableByMask(dispelMask, holder->GetCaster(), holder->GetSpellProto()))
										{
											if (const SpellEntry* se = holder->GetSpellProto())
											{
												if (se->Dispel == DispelType::DISPEL_DISEASE)
												{
													if (CastSpell(member, spell_CureDisease))
													{
														return true;
													}
													break;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		else
		{
			if (spell_DispelMagic > 0)
			{
				uint32 dispelMask = GetDispellMask(DispelType::DISPEL_MAGIC);
				Unit::SpellAuraHolderMap const& auras = me->GetSpellAuraHolderMap();
				for (Unit::SpellAuraHolderMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
				{
					if (SpellAuraHolder* holder = itr->second)
					{
						if (!holder->IsPositive())
						{
							if (holder->GetAuraDuration() > 20000)
							{
								if (holder->IsDispellableByMask(dispelMask, holder->GetCaster(), holder->GetSpellProto()))
								{
									if (const SpellEntry* se = holder->GetSpellProto())
									{
										if (se->Dispel == DispelType::DISPEL_MAGIC)
										{
											if (CastSpell(me, spell_DispelMagic))
											{
												return true;
											}
											break;
										}
									}
								}
							}
						}
					}
				}
			}
			if (spell_CureDisease > 0)
			{
				uint32 dispelMask = GetDispellMask(DispelType::DISPEL_DISEASE);
				Unit::SpellAuraHolderMap const& auras = me->GetSpellAuraHolderMap();
				for (Unit::SpellAuraHolderMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
				{
					if (SpellAuraHolder* holder = itr->second)
					{
						if (!holder->IsPositive())
						{
							if (holder->GetAuraDuration() > 20000)
							{
								if (holder->IsDispellableByMask(dispelMask, holder->GetCaster(), holder->GetSpellProto()))
								{
									if (const SpellEntry* se = holder->GetSpellProto())
									{
										if (se->Dispel == DispelType::DISPEL_DISEASE)
										{
											if (CastSpell(me, spell_CureDisease))
											{
												return true;
											}
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return false;
}

bool NierScript_Priest::Buff()
{
	if (spell_Prayer_of_Fortitude > 0 || spell_PowerWord_Fortitude > 0)
	{
		if (Group* group = me->GetGroup())
		{
			for (GroupReference* groupRef = group->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				if (Player* member = groupRef->getSource())
				{
					if (!member->IsAlive())
					{
						continue;
					}
					if (me->GetDistance(member) > VISIBILITY_DISTANCE_TINY)
					{
						continue;
					}
					if (spell_Prayer_of_Fortitude > 0)
					{
						if (CastSpell(member, spell_Prayer_of_Fortitude, true))
						{
							return true;
						}
					}
					else if (spell_PowerWord_Fortitude > 0)
					{
						if (CastSpell(member, spell_PowerWord_Fortitude, true))
						{
							return true;
						}
					}
				}
			}
		}
		else
		{
			if (spell_Prayer_of_Fortitude > 0)
			{
				if (CastSpell(me, spell_Prayer_of_Fortitude, true))
				{
					return true;
				}
			}
			else if (spell_PowerWord_Fortitude > 0)
			{
				if (CastSpell(me, spell_PowerWord_Fortitude, true))
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool NierScript_Priest::Revive()
{
	if (helpDelay > 0)
	{
		return false;
	}
	if (spell_Resurrection > 0)
	{
		if (Group* group = me->GetGroup())
		{
			for (GroupReference* groupRef = group->GetFirstMember(); groupRef != nullptr; groupRef = groupRef->next())
			{
				if (Player* member = groupRef->getSource())
				{
					if (member->IsAlive())
					{
						helpDelay = 4000;
						continue;
					}
					if (me->GetDistance(member) > VISIBILITY_DISTANCE_TINY)
					{
						helpDelay = 4000;
						continue;
					}
					if (CastSpell(member, spell_Resurrection))
					{
						helpDelay = 12000;
						return true;
					}
				}
			}
		}
	}

	return false;
}
