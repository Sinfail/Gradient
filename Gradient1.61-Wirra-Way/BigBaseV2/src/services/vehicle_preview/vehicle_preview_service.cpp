#include "fiber_pool.hpp"
#include "gui.hpp"
#include "thread_pool.hpp"
#include "util/entity.hpp"
#include "util/vehicle.hpp"
#include "vehicle_preview_service.hpp"

namespace big
{
	vehicle_preview_service::vehicle_preview_service()
	{
		g_vehicle_preview_service = this;
	}

	vehicle_preview_service::~vehicle_preview_service()
	{
		g_vehicle_preview_service = nullptr;
	}

	void vehicle_preview_service::set_preview_vehicle(const vehicle_item& item, bool spawn_max)
	{
		if (item.hash != 0)
		{
			m_owned_mods.clear();

			if (m_model_hash != item.hash || m_spawn_max != spawn_max)
			{
				m_spawn_max = spawn_max;
				m_model_hash = item.hash;
				m_new_model = true;
			}

			preview_loop();
		}
	}

	void vehicle_preview_service::set_preview_vehicle(const std::map<int, int32_t>& owned_mods, bool spawn_max)
	{
		auto hash_item = owned_mods.find(MOD_MODEL_HASH);

		if (
			m_spawn_max != spawn_max ||
			m_owned_mods.size() != owned_mods.size() ||
			!std::equal(m_owned_mods.begin(), m_owned_mods.end(), owned_mods.begin())
		) {
			m_owned_mods.clear();
			m_owned_mods.insert(owned_mods.begin(), owned_mods.end());

			auto hash_item = owned_mods.find(MOD_MODEL_HASH);

			m_spawn_max = spawn_max;
			m_model_hash = hash_item->second;
			m_new_model = true;

			preview_loop();
		}
	}

	void vehicle_preview_service::preview_loop()
	{
		if (m_running || m_loop_running)
		{
			return;
		}

		m_running = true;

		g_fiber_pool->queue_job([this] {
			m_loop_running = true;

			while (
				g_running && m_running && g_gui.m_opened &&
				(g->spawn.preview_vehicle || g->clone_pv.preview_vehicle)
			) {
				auto location = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(self::ped, 0.f, 10.f, .5f);
				if (m_current_veh == 0)
				{
					m_new_model = false;
					location.z = -10.f;

					if (m_owned_mods.empty())
					{
						m_current_veh = vehicle::spawn(m_model_hash, location, 0.f, false);
					}
					else
					{
						m_current_veh = vehicle::clone_from_owned_mods(m_owned_mods, location, 0.f, false);
					}

					if (m_current_veh)
					{
						if (m_spawn_max)
						{
							vehicle::max_vehicle(m_current_veh);
						}

						ENTITY::FREEZE_ENTITY_POSITION(m_current_veh, true);
						ENTITY::SET_ENTITY_ALPHA(m_current_veh, 0, 0);
						ENTITY::SET_ENTITY_COLLISION(m_current_veh, false, false);
						ENTITY::SET_CAN_CLIMB_ON_ENTITY(m_current_veh, false);
						OBJECT::SET_OBJECT_ALLOW_LOW_LOD_BUOYANCY(m_current_veh, false);
					}
				}
				else if (m_new_model)
				{
					ENTITY::DETACH_ENTITY(m_current_veh, 1, 1);
					ENTITY::DELETE_ENTITY(&m_current_veh);
				}
				else
				{
					if (const int alpha = ENTITY::GET_ENTITY_ALPHA(m_current_veh); alpha < 250)
					{
						ENTITY::SET_ENTITY_ALPHA(m_current_veh, std::min<int>(255, alpha + 10), 0);
					}

					ENTITY::SET_ENTITY_HEADING(m_current_veh, m_heading);
					ENTITY::SET_ENTITY_COORDS(m_current_veh, location.x, location.y, location.z, 0, 0, 0, 0);
				}

				if (m_heading += 0.5f; m_heading > 359)
				{
					m_heading = 0;
				}

				script::get_current()->yield(15ms);
			}

			entity::delete_entity(m_current_veh);
			m_current_veh = 0;
			m_model_hash = 0;
			m_owned_mods.clear();
			m_running = false;
			m_loop_running = false;
		});
	}

	void vehicle_preview_service::stop_preview()
	{
		m_owned_mods.clear();
		m_running = false;
	}
}
