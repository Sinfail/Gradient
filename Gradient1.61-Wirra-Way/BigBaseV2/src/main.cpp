#include "common.hpp"
#include "core/globals.hpp"
#include "fiber_pool.hpp"
#include "gui.hpp"
#include "logger.hpp"
#include "hooking.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "script_mgr.hpp"
#include "thread_pool.hpp"

#include "backend/backend.hpp"
#include "native_hooks/native_hooks.hpp"
#include "services/context_menu/context_menu_service.hpp"
#include "services/globals/globals_service.hpp"
#include "services/gui/gui_service.hpp"
#include "services/gta_data/gta_data_service.hpp"
#include "services/mobile/mobile_service.hpp"
#include "services/pickups/pickup_service.hpp"
#include "services/players/player_service.hpp"
#include "services/notifications/notification_service.hpp"
#include "services/vehicle_preview/vehicle_preview_service.hpp"
#include "services/vehicle/vehicle_service.hpp"

BOOL APIENTRY DllMain(HMODULE hmod, DWORD reason, PVOID)
{
	using namespace big;
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hmod);

		g_hmodule = hmod;
		g_main_thread = CreateThread(nullptr, 0, [](PVOID) -> DWORD
			{
				while (!FindWindow(L"grcWindow", L"Grand Theft Auto V"))
					std::this_thread::sleep_for(1s);

				std::filesystem::path base_dir = std::getenv("appdata");
				base_dir /= "4bazGTA";
				auto file_manager_instance = std::make_unique<file_manager>(base_dir);

				auto globals_instance = std::make_unique<menu_settings>(
					file_manager_instance->get_project_file("./settings.json")
					);

				auto logger_instance = std::make_unique<logger>(
					"Wirra-Way",
					file_manager_instance->get_project_file("./Log.log")
					);

				EnableMenuItem(GetSystemMenu(FindWindowA(NULL, "Wirra-Way"), 0), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

				try
				{
					LOG(INFO) << "Wirra-Way Initializing";
					auto pointers_instance = std::make_unique<pointers>();
					LOG(INFO) << "Pointers initialized.";

					auto renderer_instance = std::make_unique<renderer>();
					LOG(INFO) << "Renderer initialized.";

					auto fiber_pool_instance = std::make_unique<fiber_pool>(11);
					LOG(INFO) << "Fiber pool initialized.";

					auto hooking_instance = std::make_unique<hooking>();
					LOG(INFO) << "Hooking initialized.";

					g->load();
					LOG(INFO) << "Settings Loaded.";

					auto thread_pool_instance = std::make_unique<thread_pool>();
					LOG(INFO) << "Thread pool initialized.";

					auto context_menu_service_instance = std::make_unique<context_menu_service>();
					auto globals_service_instace = std::make_unique<globals_service>();
					auto mobile_service_instance = std::make_unique<mobile_service>();
					auto notification_service_instance = std::make_unique<notification_service>();
					auto pickup_service_instance = std::make_unique<pickup_service>();
					auto player_service_instance = std::make_unique<player_service>();
					//auto gta_data_service_instance = std::make_unique<gta_data_service>();
					auto vehicle_preview_service_instance = std::make_unique<vehicle_preview_service>();
					auto vehicle_service_instance = std::make_unique<vehicle_service>();
					auto gui_service_instance = std::make_unique<gui_service>();
					LOG(INFO) << "Registered service instances...";

					g_script_mgr.add_script(std::make_unique<script>(&gui::script_func, "GUI", false));
					
					g_script_mgr.add_script(std::make_unique<script>(&backend::loop, "Backend Loop", false));
					g_script_mgr.add_script(std::make_unique<script>(&backend::self_loop, "Self"));
					g_script_mgr.add_script(std::make_unique<script>(&backend::weapons_loop, "Weapon"));
					g_script_mgr.add_script(std::make_unique<script>(&backend::vehicles_loop, "Vehicle"));
					g_script_mgr.add_script(std::make_unique<script>(&backend::misc_loop, "Miscellaneous"));
					g_script_mgr.add_script(std::make_unique<script>(&backend::remote_loop, "Remote"));
					g_script_mgr.add_script(std::make_unique<script>(&backend::noclip_loop, "No Clip"));
					g_script_mgr.add_script(std::make_unique<script>(&backend::lscustoms_loop, "LS Customs"));
					g_script_mgr.add_script(std::make_unique<script>(&backend::vehiclefly_loop, "Vehicle Fly"));
					g_script_mgr.add_script(std::make_unique<script>(&backend::rgbrandomizer_loop, "RGB Randomizer"));
					g_script_mgr.add_script(std::make_unique<script>(&backend::turnsignal_loop, "Turn Signals"));
					g_script_mgr.add_script(std::make_unique<script>(&backend::disable_control_action_loop, "Disable Controls"));
					g_script_mgr.add_script(std::make_unique<script>(&context_menu_service::context_menu, "Context Menu"));
					LOG(INFO) << "Scripts registered.";

					auto native_hooks_instance = std::make_unique<native_hooks>();
					LOG(INFO) << "Dynamic native hooker initialized.";

					g_hooking->enable();
					LOG(INFO) << "Hooking enabled.";

					g_running = true;

					while (g_running)
						std::this_thread::sleep_for(500ms);

					g_hooking->disable();
					LOG(INFO) << "Hooking disabled.";

					native_hooks_instance.reset();
					LOG(INFO) << "Dynamic native hooker uninitialized.";

					g_script_mgr.remove_all_scripts();
					LOG(INFO) << "Scripts unregistered.";

					gui_service_instance.reset();
					LOG(INFO) << "Gui Service reset.";
				//	gta_data_service_instance.reset();
				//	LOG(INFO) << "GTA Data Service reset.";
					vehicle_service_instance.reset();
					LOG(INFO) << "Vehicle Service reset.";
					vehicle_preview_service_instance.reset();
					LOG(INFO) << "Vehicle Preview Service reset.";
					mobile_service_instance.reset();
					LOG(INFO) << "Mobile Service reset.";
					player_service_instance.reset();
					LOG(INFO) << "Player Service reset.";
					pickup_service_instance.reset();
					LOG(INFO) << "Pickup Service reset.";
					globals_service_instace.reset();
					LOG(INFO) << "Globals Service reset.";
					context_menu_service_instance.reset();
					LOG(INFO) << "Context Service reset.";
					LOG(INFO) << "Services uninitialized.";

					// Make sure that all threads created don't have any blocking loops
					// otherwise make sure that they have stopped executing
					thread_pool_instance->destroy();
					LOG(INFO) << "Destroyed thread pool.";

					thread_pool_instance.reset();
					LOG(INFO) << "Thread pool uninitialized.";

					hooking_instance.reset();
					LOG(INFO) << "Hooking uninitialized.";

					fiber_pool_instance.reset();
					LOG(INFO) << "Fiber pool uninitialized.";

					renderer_instance.reset();
					LOG(INFO) << "Renderer uninitialized.";

					pointers_instance.reset();
					LOG(INFO) << "Pointers uninitialized.";
				}
				catch (std::exception const& ex)
				{
					LOG(WARNING) << ex.what();
				}

				LOG(INFO) << "Cya!";
				logger_instance->destroy();
				logger_instance.reset();

				globals_instance.reset();

				file_manager_instance.reset();

				CloseHandle(g_main_thread);
				FreeLibraryAndExitThread(g_hmodule, 0);
			}, nullptr, 0, &g_main_thread_id);
	}

	return true;
}
