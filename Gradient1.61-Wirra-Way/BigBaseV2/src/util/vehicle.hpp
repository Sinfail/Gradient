#pragma once
#include "entity.hpp"
#include "gta/joaat.hpp"
#include "math.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "script.hpp"
#include "teleport.hpp"
#include "script_global.hpp"
#include "gta\VehicleValues.h"
#include "services/vehicle_helper/vehicle_helper.hpp"

namespace big::vehicle
{
	inline auto spawn_global = script_global(2725439);

	inline void go_into_personal_vehicle()
	{
		*script_global(2671449).at(8).as<int*>() = 1;
	}

	inline float mps_to_speed(float mps, SpeedUnit speed_unit)
	{
		switch (speed_unit)
		{
		case SpeedUnit::KMPH:
			return mps * 3.6f;
			break;
		case SpeedUnit::MIPH:
			return mps * 2.2369f;
			break;
		}

		return mps;
	}

	inline float speed_to_mps(float speed, SpeedUnit speed_unit)
	{
		switch (speed_unit)
		{
		case SpeedUnit::KMPH:
			return speed / 3.6f;
			break;
		case SpeedUnit::MIPH:
			return speed / 2.2369f;
			break;
		}

		return speed;
	}

	inline Vector3 get_spawn_location(bool spawn_inside)
	{
		float y_offset = 0;

		if (self::veh != 0)
		{
			y_offset = 10.f;
		}
		else if (!spawn_inside)
		{
			y_offset = 5.f;
		}

		return ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(self::ped, 0.f, y_offset, 0.f);
	}

	inline void set_mp_bitset(Vehicle veh)
	{
		DECORATOR::DECOR_SET_INT(veh, "MPBitset", 0);
		auto networkId = NETWORK::VEH_TO_NET(veh);
		if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(veh))
			NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(networkId, true);
		VEHICLE::SET_VEHICLE_IS_STOLEN(veh, FALSE);
	}

	inline void bring(Vehicle veh, Vector3 location, bool put_in = true, int seatIdx = -1)
	{
		if (!ENTITY::IS_ENTITY_A_VEHICLE(veh)) return g_notification_service->push_error("Vehicle", "Invalid handle");

		auto vecVehicleLocation = ENTITY::GET_ENTITY_COORDS(veh, true);
		teleport::load_ground_at_3dcoord(vecVehicleLocation);

		if (!entity::take_control_of(veh))
			return g_notification_service->push_warning("Vehicle", "Failed to take control of remote vehicle.");
		auto ped = self::ped;

		ENTITY::SET_ENTITY_COORDS(veh, location.x, location.y, location.z + 1.f, 0, 0, 0, 0);
		ENTITY::SET_ENTITY_HEADING(veh, ENTITY::GET_ENTITY_HEADING(ped));

		if (put_in)
		{
			for (size_t i = 0; i < 100 && math::distance_between_vectors(location, ENTITY::GET_ENTITY_COORDS(veh, true)) > 10; i++)
				script::get_current()->yield();

			auto driver_ped = VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, -1, false);

			if (driver_ped != 0)
			{
				if (PED::GET_PED_TYPE(driver_ped) == ePedType::PED_TYPE_NETWORK_PLAYER)
				{
					TASK::CLEAR_PED_TASKS_IMMEDIATELY(driver_ped);
				}
				else
				{
					entity::delete_entity(driver_ped);
				}
			}

			PED::SET_PED_INTO_VEHICLE(ped, veh, seatIdx);
		}
	}

	inline Vehicle get_closest_to_location(Vector3 location, float range)
	{
		if (const auto replay = *g_pointers->m_replay_interface; replay)
		{
			if (const auto veh_interface = replay->m_vehicle_interface; veh_interface)
			{
				const auto veh_interface_size = veh_interface->m_max_vehicles;

				float min_dist = FLT_MAX;
				int32_t m_handle = 0;

				for (int32_t i = 0; i < veh_interface_size; i++)
				{
					auto veh_entity = veh_interface->m_vehicle_list->m_vehicles[i];
					auto veh_ptr = veh_entity.m_entity_ptr;

					if (!veh_ptr || !veh_ptr->m_navigation)
					{
						continue;
					}

					auto veh_pos_arr = veh_ptr->m_navigation->m_position;
					Vector3 veh_pos(veh_pos_arr.x, veh_pos_arr.y, veh_pos_arr.z);

					float dist = math::distance_between_vectors(veh_pos, location);

					if (dist < min_dist)
					{
						int32_t tmp_handle = g_pointers->m_ptr_to_handle(veh_ptr);

						if (entity::take_control_of(tmp_handle))
						{
							min_dist = dist;
							m_handle = tmp_handle;
						}
					}
				}

				return m_handle;
			}
		}

		return 0;
	}

	inline bool set_plate(Vehicle veh, const char* plate)
	{
		if (!ENTITY::IS_ENTITY_A_VEHICLE(veh) || !entity::take_control_of(veh))
		{
			return false;
		}

		if (plate != nullptr && plate[0] != 0)
		{
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, plate);
		}

		return true;
	}

	inline bool repair(Vehicle veh)
	{
		if (!ENTITY::IS_ENTITY_A_VEHICLE(veh) || !entity::take_control_of(veh))
		{
			return false;
		}

		VEHICLE::SET_VEHICLE_FIXED(veh);
		VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(veh);
		VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, 0.f);

		return true;
	}

	inline Vehicle spawn(Hash hash, Vector3 location, float heading, bool is_networked = true)
	{
		for (uint8_t i = 0; !STREAMING::HAS_MODEL_LOADED(hash) && i < 100; i++)
		{
			STREAMING::REQUEST_MODEL(hash);
			script::get_current()->yield();
		}

		if (!STREAMING::HAS_MODEL_LOADED(hash))
		{
			return 0;
		}

		*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
		auto veh = VEHICLE::CREATE_VEHICLE(hash, location.x, location.y, location.z, heading, is_networked, false, false);
		*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;

		script::get_current()->yield();

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

		if (*g_pointers->m_is_session_started)
		{
			set_mp_bitset(veh);
		}

		return veh;
	}

	inline Vehicle clone_from_vehicle_data(std::map<int, int32_t>& data, Vector3 location, float heading)
	{
		Vector3 tmpLocation = { location.x, location.y, 1200.0f };
		if (location.z > 1000.0f && location.z < 1400.0)
		{
			tmpLocation.z = 800.0f;
		}

		// vehicle data
		for (const auto& [idx, val] : data)
		{
			if (idx >= 0 && idx < 142)
			{
				*spawn_global.at(27).at(idx).as<int32_t*>() = val;
			}
		}

		// permission fix
		*spawn_global.at(27).at(1).as<int32_t*>() = 0;

		// personal car flag
		*spawn_global.at(27).at(94).as<int32_t*>() = 14;
		*spawn_global.at(27).at(95).as<int32_t*>() = 2;

		// mmi
		*spawn_global.at(27).at(103).as<int32_t*>() = 0;

		// spawn location
		*spawn_global.at(7).at(0).as<float*>() = tmpLocation.x;
		*spawn_global.at(7).at(1).as<float*>() = tmpLocation.y;
		*spawn_global.at(7).at(2).as<float*>() = tmpLocation.z;

		// spawn non pegasus
		*spawn_global.at(3).as<int*>() = 0;

		// spawn signal
		int* spawn_signal = spawn_global.at(2).as<int32_t*>();
		*spawn_global.at(5).as<int32_t*>() = 1;
		*spawn_signal = 1;

		// wait until the vehicle is spawned
		for (size_t retry = 0; *spawn_signal != 0 && retry < 200; retry++)
		{
			script::get_current()->yield(10ms);
		}

		if (*spawn_signal == 1)
		{
			return 0;
		}

		auto veh = vehicle::get_closest_to_location(tmpLocation, 200);
		if (veh == 0)
		{
			return 0;
		}

		ENTITY::SET_ENTITY_COORDS(veh, location.x, location.y, location.z + 1.f, 0, 0, 0, 0);
		ENTITY::SET_ENTITY_HEADING(veh, heading);

		return veh;
	}

	inline std::map<int, int32_t> get_vehicle_data_from_vehicle_idx(script_global vehicle_idx)
	{
		std::map<int, int32_t> veh_data;

		for (int i = 0; i < 142; i++)
		{
			veh_data[i] = *vehicle_idx.at(i).as<int32_t*>();
		}

		veh_data.erase(1); veh_data.erase(94); veh_data.erase(95); veh_data.erase(103);

		return veh_data;
	}

	inline std::map<int, int32_t> get_owned_mods_from_vehicle_idx(script_global vehicle_idx)
	{
		std::map<int, int32_t> owned_mods;

		for (int i = MOD_SECONDARY_CUSTOM; i <= MOD_MODEL_HASH; i++)
		{
			owned_mods[i] = 0;
		}

		int32_t val_32 = *vehicle_idx.at(32).as<int32_t*>();
		int32_t val_77 = *vehicle_idx.at(77).as<int32_t*>();
		int32_t val_102 = *vehicle_idx.at(102).as<int32_t*>();


		owned_mods[MOD_MODEL_HASH] = *vehicle_idx.at(66).as<int32_t*>();

		owned_mods[MOD_PLATE_STYLE] = *vehicle_idx.at(0).as<int32_t*>();
		owned_mods[MOD_WINDOW_TINT] = *vehicle_idx.at(65).as<int32_t*>();
		owned_mods[MOD_WHEEL_TYPE] = *vehicle_idx.at(69).as<int32_t*>();


		owned_mods[MOD_PRIMARY_COL] = *vehicle_idx.at(5).as<int32_t*>();
		owned_mods[MOD_SECONDARY_COL] = *vehicle_idx.at(6).as<int32_t*>();
		owned_mods[MOD_PEARLESCENT_COL] = *vehicle_idx.at(7).as<int32_t*>();
		owned_mods[MOD_WHEEL_COL] = *vehicle_idx.at(8).as<int32_t*>();
		owned_mods[MOD_INTERIOR_COL] = *vehicle_idx.at(97).as<int32_t*>();
		owned_mods[MOD_DASHBOARD_COL] = *vehicle_idx.at(99).as<int32_t*>();


		//CUSTOM PRIMARY
		owned_mods[MOD_PRIMARY_CUSTOM] = (val_77 & (1 << 13)) != 0;
		if (owned_mods[MOD_PRIMARY_CUSTOM])
		{
			owned_mods[MOD_PRIMARY_COL_R] = *vehicle_idx.at(71).as<int32_t*>();
			owned_mods[MOD_PRIMARY_COL_G] = *vehicle_idx.at(72).as<int32_t*>();
			owned_mods[MOD_PRIMARY_COL_B] = *vehicle_idx.at(73).as<int32_t*>();
		}


		//CUSTOM SECONDARY
		owned_mods[MOD_SECONDARY_CUSTOM] = (val_77 & (1 << 12)) != 0;
		if (owned_mods[MOD_SECONDARY_CUSTOM])
		{
			owned_mods[MOD_SECONDARY_COL_R] = *vehicle_idx.at(71).as<int32_t*>();
			owned_mods[MOD_SECONDARY_COL_G] = *vehicle_idx.at(72).as<int32_t*>();
			owned_mods[MOD_SECONDARY_COL_B] = *vehicle_idx.at(73).as<int32_t*>();
		}


		// TIRE SMOKE
		owned_mods[MOD_TIRESMOKE_COL_R] = *vehicle_idx.at(62).as<int32_t*>();
		owned_mods[MOD_TIRESMOKE_COL_G] = *vehicle_idx.at(63).as<int32_t*>();
		owned_mods[MOD_TIRESMOKE_COL_B] = *vehicle_idx.at(64).as<int32_t*>();
		owned_mods[MOD_TYRE_SMOKE] = !(
			owned_mods[MOD_TIRESMOKE_COL_R] == 255 &&
			owned_mods[MOD_TIRESMOKE_COL_G] == 255 &&
			owned_mods[MOD_TIRESMOKE_COL_B] == 255
		);


		// XENON
		if (val_32 > 0)
		{
			owned_mods[MOD_XENON_LIGHTS] = 1;
			owned_mods[MOD_XENON_COL] = val_32 - 2;
		}
		else
		{
			owned_mods[MOD_XENON_LIGHTS] = 0;
		}


		// NEON
		owned_mods[MOD_NEON_LEFT_ON] = (val_77 & (1 << 30)) != 0;
		owned_mods[MOD_NEON_RIGHT_ON] = (val_77 & (1 << 31)) != 0;
		owned_mods[MOD_NEON_FRONT_ON] = (val_77 & (1 << 28)) != 0;
		owned_mods[MOD_NEON_BACK_ON] = (val_77 & (1 << 29)) != 0;
		owned_mods[MOD_NEON_COL_R] = *vehicle_idx.at(74).as<int32_t*>();
		owned_mods[MOD_NEON_COL_G] = *vehicle_idx.at(75).as<int32_t*>();
		owned_mods[MOD_NEON_COL_B] = *vehicle_idx.at(76).as<int32_t*>();

		// TIRE OPTIONS
		owned_mods[MOD_TIRE_CAN_BURST] = (val_77 & (1 << 9)) == 0;
		if ((val_102 & 0b11) == 0b11)
		{
			owned_mods[MOD_DRIFT_TIRE] = 1;
		}

		owned_mods[MOD_TURBO] = *vehicle_idx.at(28).as<int32_t*>() != 0;

		owned_mods[MOD_FRONTWHEEL_VAR] = *vehicle_idx.at(60).as<int32_t*>() != 0;
		owned_mods[MOD_REARWHEEL_VAR] = *vehicle_idx.at(61).as<int32_t*>() != 0;


		// OTHER MODS
		for (int slot = MOD_SPOILERS; slot <= MOD_LIGHTBAR; slot++)
		{
			if (slot == MOD_TURBO || slot == MOD_TYRE_SMOKE || slot == MOD_XENON_LIGHTS)
			{
				continue;
			}

			int32_t val = *vehicle_idx.at(10 + slot).as<int32_t*>() - 1;
			if (val != -1)
			{
				owned_mods[slot] = val;
			}
		}

		// EXTRA
		for (int extra = MOD_EXTRA_9; extra <= MOD_EXTRA_1; extra++)
		{
			int gta_extra_id = (extra - MOD_EXTRA_0) * -1;
			owned_mods[extra] = val_77 >> (gta_extra_id - 1) & 1;
		}

		return owned_mods;
	}


	inline Vehicle clone_from_owned_mods(std::map<int, int32_t> owned_mods, Vector3 location, float heading, bool is_networked = true)
	{
		auto vehicle = spawn(owned_mods[MOD_MODEL_HASH], location, heading, is_networked);
		if (vehicle == 0)
		{
			return 0;
		}

		for (int i = MOD_NEON_COL_B; i <= MOD_MODEL_HASH; i++)
		{
			if (owned_mods.count(i) == 0)
			{
				owned_mods[i] = 0;
			}
		}

		VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
		script::get_current()->yield(10ms);

		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(vehicle, owned_mods[MOD_PLATE_STYLE]);
		VEHICLE::SET_VEHICLE_WINDOW_TINT(vehicle, owned_mods[MOD_WINDOW_TINT]);
		VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, owned_mods[MOD_WHEEL_TYPE]);
		script::get_current()->yield(10ms);

		VEHICLE::SET_VEHICLE_COLOURS(vehicle, owned_mods[MOD_PRIMARY_COL], owned_mods[MOD_SECONDARY_COL]);
		VEHICLE::SET_VEHICLE_EXTRA_COLOURS(vehicle, owned_mods[MOD_PEARLESCENT_COL], owned_mods[MOD_WHEEL_COL]);
		VEHICLE::SET_VEHICLE_INTERIOR_COLOR_(vehicle, owned_mods[MOD_INTERIOR_COL]);
		VEHICLE::SET_VEHICLE_DASHBOARD_COLOR_(vehicle, owned_mods[MOD_DASHBOARD_COL]);

		if (owned_mods[MOD_PRIMARY_CUSTOM])
		{
			VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(
				vehicle, owned_mods[MOD_PRIMARY_COL_R],
				owned_mods[MOD_PRIMARY_COL_G], owned_mods[MOD_PRIMARY_COL_B]
			);
		}

		if (owned_mods[MOD_SECONDARY_CUSTOM])
		{
			VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(
				vehicle, owned_mods[MOD_SECONDARY_COL_R],
				owned_mods[MOD_SECONDARY_COL_G], owned_mods[MOD_SECONDARY_COL_B]
			);
		}

		if (owned_mods[MOD_TYRE_SMOKE])
		{
			VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(
				vehicle, owned_mods[MOD_TIRESMOKE_COL_R],
				owned_mods[MOD_TIRESMOKE_COL_G], owned_mods[MOD_TIRESMOKE_COL_B]
			);
			VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, MOD_TYRE_SMOKE, owned_mods[MOD_TYRE_SMOKE]);
		}

		if (owned_mods[MOD_XENON_LIGHTS])
		{
			VEHICLE::SET_VEHICLE_XENON_LIGHTS_COLOR_(vehicle, owned_mods[MOD_XENON_COL]);
			VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, MOD_XENON_LIGHTS, owned_mods[MOD_XENON_LIGHTS]);
		}

		VEHICLE::SET_VEHICLE_NEON_LIGHTS_COLOUR_(
			vehicle, owned_mods[MOD_NEON_COL_R],
			owned_mods[MOD_NEON_COL_G], owned_mods[MOD_NEON_COL_B]
		);
		VEHICLE::SET_VEHICLE_NEON_LIGHT_ENABLED_(vehicle, NEON_LEFT, owned_mods[MOD_NEON_LEFT_ON]);
		VEHICLE::SET_VEHICLE_NEON_LIGHT_ENABLED_(vehicle, NEON_RIGHT, owned_mods[MOD_NEON_RIGHT_ON]);
		VEHICLE::SET_VEHICLE_NEON_LIGHT_ENABLED_(vehicle, NEON_FRONT, owned_mods[MOD_NEON_FRONT_ON]);
		VEHICLE::SET_VEHICLE_NEON_LIGHT_ENABLED_(vehicle, NEON_BACK, owned_mods[MOD_NEON_BACK_ON]);


		VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(vehicle, owned_mods[MOD_TIRE_CAN_BURST]);
		VEHICLE::SET_DRIFT_TYRES_ENABLED_(vehicle, owned_mods[MOD_DRIFT_TIRE]);
		VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, MOD_TURBO, owned_mods[MOD_TURBO]);

		for (int slot = MOD_SPOILERS; slot <= MOD_LIGHTBAR; slot++)
		{
			if (owned_mods.count(slot) && owned_mods[slot] != -1)
			{
				bool custom_tire = false;

				if (slot == MOD_FRONTWHEEL)
				{
					custom_tire = owned_mods[MOD_FRONTWHEEL_VAR];
				}
				else if (slot == MOD_REARWHEEL)
				{
					custom_tire = owned_mods[MOD_REARWHEEL_VAR];
				}

				VEHICLE::SET_VEHICLE_MOD(vehicle, slot, owned_mods[slot], custom_tire);
			}
		}

		for (int extra = MOD_EXTRA_14; extra <= MOD_EXTRA_1; extra++)
		{
			int gta_extra_id = (extra - MOD_EXTRA_0) * -1;
			if (owned_mods.count(extra) && VEHICLE::DOES_EXTRA_EXIST(vehicle, gta_extra_id))
			{
				VEHICLE::SET_VEHICLE_EXTRA(vehicle, gta_extra_id, owned_mods[extra] == 0);
			}
		}

		return vehicle;
	}

	inline std::map<int, int32_t> get_owned_mods_from_vehicle(Vehicle vehicle)
	{
		std::map<int, int32_t> owned_mods;

		for (int i = MOD_SECONDARY_CUSTOM; i <= MOD_MODEL_HASH; i++)
		{
			owned_mods[i] = 0;
		}

		owned_mods[MOD_MODEL_HASH] = ENTITY::GET_ENTITY_MODEL(vehicle);

		owned_mods[MOD_PLATE_STYLE] = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(vehicle);
		owned_mods[MOD_WINDOW_TINT] = VEHICLE::GET_VEHICLE_WINDOW_TINT(vehicle);
		owned_mods[MOD_WHEEL_TYPE] = VEHICLE::GET_VEHICLE_WHEEL_TYPE(vehicle);

		VEHICLE::GET_VEHICLE_COLOURS(vehicle, &owned_mods[MOD_PRIMARY_COL], &owned_mods[MOD_SECONDARY_COL]);
		VEHICLE::GET_VEHICLE_EXTRA_COLOURS(vehicle, &owned_mods[MOD_PEARLESCENT_COL], &owned_mods[MOD_WHEEL_COL]);
		VEHICLE::GET_VEHICLE_INTERIOR_COLOR_(vehicle, &owned_mods[MOD_INTERIOR_COL]);
		VEHICLE::GET_VEHICLE_DASHBOARD_COLOR_(vehicle, &owned_mods[MOD_DASHBOARD_COL]);

		owned_mods[MOD_PRIMARY_CUSTOM] = VEHICLE::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(vehicle);
		if (owned_mods[MOD_PRIMARY_CUSTOM])
		{
			VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(
				vehicle, &owned_mods[MOD_PRIMARY_COL_R],
				&owned_mods[MOD_PRIMARY_COL_G], &owned_mods[MOD_PRIMARY_COL_B]
			);
		}

		owned_mods[MOD_SECONDARY_CUSTOM] = VEHICLE::GET_IS_VEHICLE_SECONDARY_COLOUR_CUSTOM(vehicle);
		if (owned_mods[MOD_SECONDARY_CUSTOM])
		{
			VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(
				vehicle, &owned_mods[MOD_SECONDARY_COL_R],
				&owned_mods[MOD_SECONDARY_COL_G], &owned_mods[MOD_SECONDARY_COL_B]
			);
		}

		owned_mods[MOD_TYRE_SMOKE] = VEHICLE::IS_TOGGLE_MOD_ON(vehicle, MOD_TYRE_SMOKE);
		if (owned_mods[MOD_TYRE_SMOKE])
		{
			VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(
				vehicle, &owned_mods[MOD_TIRESMOKE_COL_R],
				&owned_mods[MOD_TIRESMOKE_COL_G], &owned_mods[MOD_TIRESMOKE_COL_B]
			);
		}

		owned_mods[MOD_XENON_LIGHTS] = VEHICLE::IS_TOGGLE_MOD_ON(vehicle, MOD_XENON_LIGHTS);
		if (owned_mods[MOD_XENON_LIGHTS])
		{
			owned_mods[MOD_XENON_COL] = (int8_t)VEHICLE::GET_VEHICLE_XENON_LIGHTS_COLOR_(vehicle);
		}

		owned_mods[MOD_NEON_LEFT_ON] = VEHICLE::IS_VEHICLE_NEON_LIGHT_ENABLED_(vehicle, NEON_LEFT);
		owned_mods[MOD_NEON_RIGHT_ON] = VEHICLE::IS_VEHICLE_NEON_LIGHT_ENABLED_(vehicle, NEON_RIGHT);
		owned_mods[MOD_NEON_FRONT_ON] = VEHICLE::IS_VEHICLE_NEON_LIGHT_ENABLED_(vehicle, NEON_FRONT);
		owned_mods[MOD_NEON_BACK_ON] = VEHICLE::IS_VEHICLE_NEON_LIGHT_ENABLED_(vehicle, NEON_BACK);
		VEHICLE::GET_VEHICLE_NEON_LIGHTS_COLOUR_(
			vehicle, &owned_mods[MOD_NEON_COL_R],
			&owned_mods[MOD_NEON_COL_G], &owned_mods[MOD_NEON_COL_B]
		);

		owned_mods[MOD_TIRE_CAN_BURST] = VEHICLE::GET_VEHICLE_TYRES_CAN_BURST(vehicle);
		owned_mods[MOD_DRIFT_TIRE] = VEHICLE::GET_DRIFT_TYRES_ENABLED_(vehicle);
		owned_mods[MOD_TURBO] = VEHICLE::IS_TOGGLE_MOD_ON(vehicle, MOD_TURBO);

		owned_mods[MOD_FRONTWHEEL_VAR] = VEHICLE::GET_VEHICLE_MOD_VARIATION(vehicle, MOD_FRONTWHEEL);
		owned_mods[MOD_REARWHEEL_VAR] = VEHICLE::GET_VEHICLE_MOD_VARIATION(vehicle, MOD_REARWHEEL);

		for (int slot = MOD_SPOILERS; slot <= MOD_LIGHTBAR; slot++)
		{
			int count = VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, slot);
			if (count > 0)
			{
				int32_t val = VEHICLE::GET_VEHICLE_MOD(vehicle, slot);

				if (val != -1)
				{
					owned_mods[slot] = val;
				}
			}
		}

		for (int extra = MOD_EXTRA_14; extra <= MOD_EXTRA_1; extra++)
		{
			int gta_extra_id = (extra - MOD_EXTRA_0) * -1;
			if (VEHICLE::DOES_EXTRA_EXIST(vehicle, gta_extra_id))
			{
				owned_mods[extra] = VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(vehicle, gta_extra_id);
			}
		}

		return owned_mods;
	}

	inline void teleport_into_vehicle(Vehicle veh)
	{
		PED::SET_PED_INTO_VEHICLE(self::ped, veh, -1);
	}

	inline void max_vehicle(Vehicle veh)
	{
		Hash model = ENTITY::GET_ENTITY_MODEL(veh);

		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);

		VEHICLE::TOGGLE_VEHICLE_MOD(veh, MOD_TURBO, TRUE);
		VEHICLE::TOGGLE_VEHICLE_MOD(veh, MOD_TYRE_SMOKE, TRUE);
		VEHICLE::TOGGLE_VEHICLE_MOD(veh, MOD_XENON_LIGHTS, TRUE);
		VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, 1);
		VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, false);

		for (int slot = MOD_SPOILERS; slot <= MOD_LIGHTBAR; slot++)
		{
			if (slot == MOD_LIVERY) {
				continue;
			}

			int count = VEHICLE::GET_NUM_VEHICLE_MODS(veh, slot);
			if (count > 0)
			{
				int selected_mod = -1;

				for (int mod = count - 1; mod >= -1; mod--)
				{

					if (vehicle_helper::check_mod_blacklist(model, slot, mod))
					{
						continue;
					}

					selected_mod = mod;
					break;
				}

				if (selected_mod != -1)
				{
					VEHICLE::SET_VEHICLE_MOD(veh, slot, selected_mod, true);
				}
			}
		}
	}



	static constexpr char const* rgb_types[] = { "Off", "Fade", "Spasm" };

	static constexpr int driving_styles[] = { 443, 524861 };
	static constexpr char const* driving_style_names[] = { "Law-Abiding", "The Road Is Yours" };
}
