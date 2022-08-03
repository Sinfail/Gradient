#pragma once
#include "gta\joaat.hpp"

namespace big
{
	struct rewards
	{
		using pickup_map = std::map<const std::string_view, const Hash>;

		inline static const pickup_map misc = {
			{ "ARMOUR", RAGE_JOAAT("REWARD_ARMOUR") },
			{ "HEALTH", RAGE_JOAAT("REWARD_HEALTH") },
			{ "HEALTH_ENTER_VEHICLE", RAGE_JOAAT("REWARD_HEALTH_ENTER_VEHICLE") },
			{ "HEALTH_VARIABLE", RAGE_JOAAT("REWARD_HEALTH_VARIABLE") },
			{ "MONEY_VARIABLE", RAGE_JOAAT("REWARD_MONEY_VARIABLE") },
			{ "PARACHUTE", RAGE_JOAAT("REWARD_PARACHUTE") },
			{ "STAT_HEALTH", RAGE_JOAAT("REWARD_STAT_HEALTH") },
			{ "STAT_HEALTH_VARIABLE", RAGE_JOAAT("REWARD_STAT_HEALTH_VARIABLE") },
			{ "STAT_WEAPON", RAGE_JOAAT("REWARD_STAT_WEAPON") },
			{ "VEHICLE_FIX", RAGE_JOAAT("REWARD_VEHICLE_FIX") }
		};

		inline static const pickup_map ammo =
		{
			{ "ADVANCEDRIFLE", RAGE_JOAAT("REWARD_AMMO_ADVANCEDRIFLE") },
			{ "APPISTOL", RAGE_JOAAT("REWARD_AMMO_APPISTOL") },
			{ "ASSAULTRIFLE", RAGE_JOAAT("REWARD_AMMO_ASSAULTRIFLE") },
			{ "ASSAULTSHOTGUN", RAGE_JOAAT("REWARD_AMMO_ASSAULTSHOTGUN") },
			{ "ASSAULTSMG", RAGE_JOAAT("REWARD_AMMO_ASSAULTSMG") },
			{ "AUTOSHOTGUN", RAGE_JOAAT("REWARD_AMMO_AUTOSHOTGUN") },
			{ "BULLET_MP", RAGE_JOAAT("REWARD_AMMO_BULLET_MP") },
			{ "BULLPUPRIFLE", RAGE_JOAAT("REWARD_AMMO_BULLPUPRIFLE") },
			{ "BULLPUPSHOTGUN", RAGE_JOAAT("REWARD_AMMO_BULLPUPSHOTGUN") },
			{ "CARBINERIFLE", RAGE_JOAAT("REWARD_AMMO_CARBINERIFLE") },
			{ "COMBATMG", RAGE_JOAAT("REWARD_AMMO_COMBATMG") },
			{ "COMBATPDW", RAGE_JOAAT("REWARD_AMMO_COMBATPDW") },
			{ "COMBATPISTOL", RAGE_JOAAT("REWARD_AMMO_COMBATPISTOL") },
			{ "COMPACTLAUNCHER", RAGE_JOAAT("REWARD_AMMO_COMPACTLAUNCHER") },
			{ "COMPACTRIFLE", RAGE_JOAAT("REWARD_AMMO_COMPACTRIFLE") },
			{ "DBSHOTGUN", RAGE_JOAAT("REWARD_AMMO_DBSHOTGUN") },
			{ "DOUBLEACTION", RAGE_JOAAT("REWARD_AMMO_DOUBLEACTION") },
			{ "FIREWORK", RAGE_JOAAT("REWARD_AMMO_FIREWORK") },
			{ "FIREWORK_MP", RAGE_JOAAT("REWARD_AMMO_FIREWORK_MP") },
			{ "FLAREGUN", RAGE_JOAAT("REWARD_AMMO_FLAREGUN") },
			{ "GRENADE", RAGE_JOAAT("REWARD_AMMO_GRENADE") },
			{ "GRENADELAUNCHER", RAGE_JOAAT("REWARD_AMMO_GRENADELAUNCHER") },
			{ "GRENADELAUNCHER_MP", RAGE_JOAAT("REWARD_AMMO_GRENADELAUNCHER_MP") },
			{ "GUSENBERG", RAGE_JOAAT("REWARD_AMMO_GUSENBERG") },
			{ "HEAVYPISTOL", RAGE_JOAAT("REWARD_AMMO_HEAVYPISTOL") },
			{ "HEAVYSHOTGUN", RAGE_JOAAT("REWARD_AMMO_HEAVYSHOTGUN") },
			{ "HEAVYSNIPER", RAGE_JOAAT("REWARD_AMMO_HEAVYSNIPER") },
			{ "HOMINGLAUNCHER", RAGE_JOAAT("REWARD_AMMO_HOMINGLAUNCHER") },
			{ "MACHINEPISTOL", RAGE_JOAAT("REWARD_AMMO_MACHINEPISTOL") },
			{ "MARKSMANPISTOL", RAGE_JOAAT("REWARD_AMMO_MARKSMANPISTOL") },
			{ "MARKSMANRIFLE", RAGE_JOAAT("REWARD_AMMO_MARKSMANRIFLE") },
			{ "MG", RAGE_JOAAT("REWARD_AMMO_MG") },
			{ "MICROSMG", RAGE_JOAAT("REWARD_AMMO_MICROSMG") },
			{ "MINIGUN", RAGE_JOAAT("REWARD_AMMO_MINIGUN") },
			{ "MINISMG", RAGE_JOAAT("REWARD_AMMO_MINISMG") },
			{ "MISSILE_MP", RAGE_JOAAT("REWARD_AMMO_MISSILE_MP") },
			{ "MOLOTOV", RAGE_JOAAT("REWARD_AMMO_MOLOTOV") },
			{ "MUSKET", RAGE_JOAAT("REWARD_AMMO_MUSKET") },
			{ "PETROLCAN", RAGE_JOAAT("REWARD_AMMO_PETROLCAN") },
			{ "PIPEBOMB", RAGE_JOAAT("REWARD_AMMO_PIPEBOMB") },
			{ "PISTOL", RAGE_JOAAT("REWARD_AMMO_PISTOL") },
			{ "PISTOL50", RAGE_JOAAT("REWARD_AMMO_PISTOL50") },
			{ "PROXMINE", RAGE_JOAAT("REWARD_AMMO_PROXMINE") },
			{ "PUMPSHOTGUN", RAGE_JOAAT("REWARD_AMMO_PUMPSHOTGUN") },
			{ "PUMPSHOTGUN_ENTER_VEHICLE", RAGE_JOAAT("REWARD_AMMO_PUMPSHOTGUN_ENTER_VEHICLE") },
			{ "RAILGUN", RAGE_JOAAT("REWARD_AMMO_RAILGUN") },
			{ "RAYPISTOL", RAGE_JOAAT("REWARD_AMMO_RAYPISTOL") },
			{ "REVOLVER", RAGE_JOAAT("REWARD_AMMO_REVOLVER") },
			{ "RPG", RAGE_JOAAT("REWARD_AMMO_RPG") },
			{ "SAWNOFFSHOTGUN", RAGE_JOAAT("REWARD_AMMO_SAWNOFFSHOTGUN") },
			{ "SMG", RAGE_JOAAT("REWARD_AMMO_SMG") },
			{ "SMOKEGRENADE", RAGE_JOAAT("REWARD_AMMO_SMOKEGRENADE") },
			{ "SNIPERRIFLE", RAGE_JOAAT("REWARD_AMMO_SNIPERRIFLE") },
			{ "SNSPISTOL", RAGE_JOAAT("REWARD_AMMO_SNSPISTOL") },
			{ "SPECIALCARBINE", RAGE_JOAAT("REWARD_AMMO_SPECIALCARBINE") },
			{ "STICKYBOMB", RAGE_JOAAT("REWARD_AMMO_STICKYBOMB") },
			{ "STUNGUN", RAGE_JOAAT("REWARD_AMMO_STUNGUN") },
			{ "VINTAGEPISTOL", RAGE_JOAAT("REWARD_AMMO_VINTAGEPISTOL") }
		};

		inline static const pickup_map weapons =
		{
			{ "ADVANCEDRIFLE", RAGE_JOAAT("REWARD_WEAPON_ADVANCEDRIFLE") },
			{ "APPISTOL", RAGE_JOAAT("REWARD_WEAPON_APPISTOL") },
			{ "ASSAULTRIFLE", RAGE_JOAAT("REWARD_WEAPON_ASSAULTRIFLE") },
			{ "ASSAULTRIFLE_MK2", RAGE_JOAAT("REWARD_WEAPON_ASSAULTRIFLE_MK2") },
			{ "ASSAULTSHOTGUN", RAGE_JOAAT("REWARD_WEAPON_ASSAULTSHOTGUN") },
			{ "ASSAULTSMG", RAGE_JOAAT("REWARD_WEAPON_ASSAULTSMG") },
			{ "AUTOSHOTGUN", RAGE_JOAAT("REWARD_WEAPON_AUTOSHOTGUN") },
			{ "BAT", RAGE_JOAAT("REWARD_WEAPON_BAT") },
			{ "BATTLEAXE", RAGE_JOAAT("REWARD_WEAPON_BATTLEAXE") },
			{ "BOTTLE", RAGE_JOAAT("REWARD_WEAPON_BOTTLE") },
			{ "BULLPUPRIFLE", RAGE_JOAAT("REWARD_WEAPON_BULLPUPRIFLE") },
			{ "BULLPUPRIFLE_MK2", RAGE_JOAAT("REWARD_WEAPON_BULLPUPRIFLE_MK2") },
			{ "BULLPUPSHOTGUN", RAGE_JOAAT("REWARD_WEAPON_BULLPUPSHOTGUN") },
			{ "CARBINERIFLE", RAGE_JOAAT("REWARD_WEAPON_CARBINERIFLE") },
			{ "CARBINERIFLE_MK2", RAGE_JOAAT("REWARD_WEAPON_CARBINERIFLE_MK2") },
			{ "COMBATMG", RAGE_JOAAT("REWARD_WEAPON_COMBATMG") },
			{ "COMBATMG_MK2", RAGE_JOAAT("REWARD_WEAPON_COMBATMG_MK2") },
			{ "COMBATPDW", RAGE_JOAAT("REWARD_WEAPON_COMBATPDW") },
			{ "COMBATPISTOL", RAGE_JOAAT("REWARD_WEAPON_COMBATPISTOL") },
			{ "COMPACTLAUNCHER", RAGE_JOAAT("REWARD_WEAPON_COMPACTLAUNCHER") },
			{ "COMPACTRIFLE", RAGE_JOAAT("REWARD_WEAPON_COMPACTRIFLE") },
			{ "CROWBAR", RAGE_JOAAT("REWARD_WEAPON_CROWBAR") },
			{ "DAGGER", RAGE_JOAAT("REWARD_WEAPON_DAGGER") },
			{ "DBSHOTGUN", RAGE_JOAAT("REWARD_WEAPON_DBSHOTGUN") },
			{ "DOUBLEACTION", RAGE_JOAAT("REWARD_WEAPON_DOUBLEACTION") },
			{ "FIREWORK", RAGE_JOAAT("REWARD_WEAPON_FIREWORK") },
			{ "FLAREGUN", RAGE_JOAAT("REWARD_WEAPON_FLAREGUN") },
			{ "FLASHLIGHT", RAGE_JOAAT("REWARD_WEAPON_FLASHLIGHT") },
			{ "GOLFCLUB", RAGE_JOAAT("REWARD_WEAPON_GOLFCLUB") },
			{ "GRENADE", RAGE_JOAAT("REWARD_WEAPON_GRENADE") },
			{ "GRENADELAUNCHER", RAGE_JOAAT("REWARD_WEAPON_GRENADELAUNCHER") },
			{ "GUSENBERG", RAGE_JOAAT("REWARD_WEAPON_GUSENBERG") },
			{ "HAMMER", RAGE_JOAAT("REWARD_WEAPON_HAMMER") },
			{ "HATCHET", RAGE_JOAAT("REWARD_WEAPON_HATCHET") },
			{ "HEAVYPISTOL", RAGE_JOAAT("REWARD_WEAPON_HEAVYPISTOL") },
			{ "HEAVYSHOTGUN", RAGE_JOAAT("REWARD_WEAPON_HEAVYSHOTGUN") },
			{ "HEAVYSNIPER", RAGE_JOAAT("REWARD_WEAPON_HEAVYSNIPER") },
			{ "HEAVYSNIPER_MK2", RAGE_JOAAT("REWARD_WEAPON_HEAVYSNIPER_MK2") },
			{ "HOMINGLAUNCHER", RAGE_JOAAT("REWARD_WEAPON_HOMINGLAUNCHER") },
			{ "KNIFE", RAGE_JOAAT("REWARD_WEAPON_KNIFE") },
			{ "KNUCKLE", RAGE_JOAAT("REWARD_WEAPON_KNUCKLE") },
			{ "MACHETE", RAGE_JOAAT("REWARD_WEAPON_MACHETE") },
			{ "MACHINEPISTOL", RAGE_JOAAT("REWARD_WEAPON_MACHINEPISTOL") },
			{ "MARKSMANPISTOL", RAGE_JOAAT("REWARD_WEAPON_MARKSMANPISTOL") },
			{ "MARKSMANRIFLE", RAGE_JOAAT("REWARD_WEAPON_MARKSMANRIFLE") },
			{ "MARKSMANRIFLE_MK2", RAGE_JOAAT("REWARD_WEAPON_MARKSMANRIFLE_MK2") },
			{ "MG", RAGE_JOAAT("REWARD_WEAPON_MG") },
			{ "MICROSMG", RAGE_JOAAT("REWARD_WEAPON_MICROSMG") },
			{ "MINIGUN", RAGE_JOAAT("REWARD_WEAPON_MINIGUN") },
			{ "MINISMG", RAGE_JOAAT("REWARD_WEAPON_MINISMG") },
			{ "MOLOTOV", RAGE_JOAAT("REWARD_WEAPON_MOLOTOV") },
			{ "MUSKET", RAGE_JOAAT("REWARD_WEAPON_MUSKET") },
			{ "NIGHTSTICK", RAGE_JOAAT("REWARD_WEAPON_NIGHTSTICK") },
			{ "PETROLCAN", RAGE_JOAAT("REWARD_WEAPON_PETROLCAN") },
			{ "PIPEBOMB", RAGE_JOAAT("REWARD_WEAPON_PIPEBOMB") },
			{ "PISTOL", RAGE_JOAAT("REWARD_WEAPON_PISTOL") },
			{ "PISTOL50", RAGE_JOAAT("REWARD_WEAPON_PISTOL50") },
			{ "PISTOL_MK2", RAGE_JOAAT("REWARD_WEAPON_PISTOL_MK2") },
			{ "POOLCUE", RAGE_JOAAT("REWARD_WEAPON_POOLCUE") },
			{ "PROXMINE", RAGE_JOAAT("REWARD_WEAPON_PROXMINE") },
			{ "PUMPSHOTGUN", RAGE_JOAAT("REWARD_WEAPON_PUMPSHOTGUN") },
			{ "PUMPSHOTGUN_MK2", RAGE_JOAAT("REWARD_WEAPON_PUMPSHOTGUN_MK2") },
			{ "RAILGUN", RAGE_JOAAT("REWARD_WEAPON_RAILGUN") },
			{ "RAYCARBINE", RAGE_JOAAT("REWARD_WEAPON_RAYCARBINE") },
			{ "RAYMINIGUN", RAGE_JOAAT("REWARD_WEAPON_RAYMINIGUN") },
			{ "RAYPISTOL", RAGE_JOAAT("REWARD_WEAPON_RAYPISTOL") },
			{ "REVOLVER", RAGE_JOAAT("REWARD_WEAPON_REVOLVER") },
			{ "REVOLVER_MK2", RAGE_JOAAT("REWARD_WEAPON_REVOLVER_MK2") },
			{ "RPG", RAGE_JOAAT("REWARD_WEAPON_RPG") },
			{ "SAWNOFFSHOTGUN", RAGE_JOAAT("REWARD_WEAPON_SAWNOFFSHOTGUN") },
			{ "SMG", RAGE_JOAAT("REWARD_WEAPON_SMG") },
			{ "SMG_MK2", RAGE_JOAAT("REWARD_WEAPON_SMG_MK2") },
			{ "SMOKEGRENADE", RAGE_JOAAT("REWARD_WEAPON_SMOKEGRENADE") },
			{ "SNIPERRIFLE", RAGE_JOAAT("REWARD_WEAPON_SNIPERRIFLE") },
			{ "SNSPISTOL", RAGE_JOAAT("REWARD_WEAPON_SNSPISTOL") },
			{ "SNSPISTOL_MK2", RAGE_JOAAT("REWARD_WEAPON_SNSPISTOL_MK2") },
			{ "SPECIALCARBINE", RAGE_JOAAT("REWARD_WEAPON_SPECIALCARBINE") },
			{ "SPECIALCARBINE_MK2", RAGE_JOAAT("REWARD_WEAPON_SPECIALCARBINE_MK2") },
			{ "STICKYBOMB", RAGE_JOAAT("REWARD_WEAPON_STICKYBOMB") },
			{ "STONE_HATCHET", RAGE_JOAAT("REWARD_WEAPON_STONE_HATCHET") },
			{ "STUNGUN", RAGE_JOAAT("REWARD_WEAPON_STUNGUN") },
			{ "SWITCHBLADE", RAGE_JOAAT("REWARD_WEAPON_SWITCHBLADE") },
			{ "VINTAGEPISTOL", RAGE_JOAAT("REWARD_WEAPON_VINTAGEPISTOL") },
			{ "WRENCH", RAGE_JOAAT("REWARD_WEAPON_WRENCH") }
		};
	};
}
