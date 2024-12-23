#include "../renderer_backend.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_debug_callbck(
	VkDebugUtilsMessageSeverityFlagBitsEXT msg_severity,
	VkDebugUtilsMessageTypeFlagsEXT msg_flags,
	const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
	void* p_user_data
) {
	printf("--------------------------------------------------------------------------\n\nValidation Error: %s\n\n\n\n", p_callback_data->pMessage);
	return 0;
}


uint32_t renderer_backend_list_gpus(struct game_client* game) {

	struct rendering_window_state rws;
	rendering_window_get_data(game->renderer_state.backend.dummy_window, &rws);

	uint32_t gpu_count = 0;
	VKCall(vkEnumeratePhysicalDevices(game->renderer_state.backend.instance, &gpu_count, 0));
	VKCall(vkEnumeratePhysicalDevices(game->renderer_state.backend.instance, &gpu_count, game->renderer_state.backend.gpus));

	for (uint32_t i = 0; i < gpu_count; i++) {

		VkPhysicalDeviceProperties dev_prop;
		vkGetPhysicalDeviceProperties(game->renderer_state.backend.gpus[i], &dev_prop);

		memcpy(game->application_state.machine_info.gpus[i].name, dev_prop.deviceName, 256);
		game->application_state.machine_info.gpus[i].usable = 0;
		game->application_state.machine_info.gpus[i].gpu_type = dev_prop.deviceType;
		game->application_state.machine_info.gpus[i].vulkan_version_major = VK_API_VERSION_MAJOR(dev_prop.apiVersion);
		game->application_state.machine_info.gpus[i].vulkan_version_minor = VK_API_VERSION_MINOR(dev_prop.apiVersion);
		game->application_state.machine_info.gpus[i].vulkan_version_patch = VK_API_VERSION_PATCH(dev_prop.apiVersion);

		if (VK_API_VERSION_MAJOR(dev_prop.apiVersion) < 1 || VK_API_VERSION_MINOR(dev_prop.apiVersion) < 2) continue;

		uint32_t format_count = 0;
		VKCall(vkGetPhysicalDeviceSurfaceFormatsKHR(game->renderer_state.backend.gpus[i], rws.surface, &format_count, 0));
		VkSurfaceFormatKHR* surface_formats = alloca(format_count * sizeof(VkSurfaceFormatKHR));
		VKCall(vkGetPhysicalDeviceSurfaceFormatsKHR(game->renderer_state.backend.gpus[i], rws.surface, &format_count, surface_formats));

		uint32_t found_format = 0;
		for (uint32_t k = 0; k < format_count; k++) if (surface_formats[k].format == VK_FORMAT_B8G8R8A8_UNORM) {
			game->renderer_state.backend.gpu_surface_formats[i] = surface_formats[k];
			found_format = 1;
			break;
		}
		if (found_format == 0) continue;

		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(game->renderer_state.backend.gpus[i], &queue_family_count, 0);
		VkQueueFamilyProperties* queue_props = alloca(queue_family_count * sizeof(VkQueueFamilyProperties));
		vkGetPhysicalDeviceQueueFamilyProperties(game->renderer_state.backend.gpus[i], &queue_family_count, queue_props);

		for (uint32_t j = 0; j < queue_family_count; j++) {

			if ((queue_props[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) && (queue_props[j].queueFlags & VK_QUEUE_TRANSFER_BIT) && (queue_props[j].queueFlags & VK_QUEUE_COMPUTE_BIT)) {

				VkBool32 surface_support = VK_FALSE;
				VKCall(vkGetPhysicalDeviceSurfaceSupportKHR(game->renderer_state.backend.gpus[i], j, rws.surface, &surface_support));

				if (surface_support) {
					game->renderer_state.backend.gpu_queue_indices[i] = j;
					game->application_state.machine_info.gpus[i].usable = 1;

				}

			}

		}

	}

	game->application_state.machine_info.gpu_count = gpu_count;

	return 0;

}

uint32_t renderer_backend_instance_create(struct game_client* game) {

	VkApplicationInfo app_info = { 0 };
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "blocks2";
	app_info.pEngineName = "blocks2_angine";

	char* instance_extensions[] = {
		PLATFORM_VK_SURFACE_EXTENSION,
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};

	char* layers[] = {
		"VK_LAYER_KHRONOS_validation",
	};

	VkInstanceCreateInfo instance_info = { 0 };
	instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_info.pApplicationInfo = &app_info;
	instance_info.ppEnabledExtensionNames = instance_extensions;
	instance_info.enabledExtensionCount = 4;
	instance_info.ppEnabledLayerNames = layers;
	instance_info.enabledLayerCount = 1;

	VKCall(vkCreateInstance(&instance_info, 0, &game->renderer_state.backend.instance));

	PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(game->renderer_state.backend.instance, "vkCreateDebugUtilsMessengerEXT");

	game->renderer_state.backend.debug_messenger = 0;
	if (vkCreateDebugUtilsMessengerEXT) {

		VkDebugUtilsMessengerCreateInfoEXT debug_info = { 0 };
		debug_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debug_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
		debug_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
		debug_info.pfnUserCallback = vulkan_debug_callbck;

		vkCreateDebugUtilsMessengerEXT(game->renderer_state.backend.instance, &debug_info, 0, &game->renderer_state.backend.debug_messenger);
	}

	game->renderer_state.backend.dummy_window = window_create(50, 50, 50, 50, "a", 0);
	rendering_window_new(game->renderer_state.backend.dummy_window, game->renderer_state.backend.instance);

	rendering_window_new(game->application_state.window, game->renderer_state.backend.instance);

	return 0;
}

uint32_t renderer_backend_instance_destroy(struct game_client* game) {

	rendering_window_destroy(game->renderer_state.backend.dummy_window);
	rendering_window_destroy(game->application_state.window);

	window_destroy(game->renderer_state.backend.dummy_window);

	PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT = vkGetInstanceProcAddr(game->renderer_state.backend.instance, "vkDestroyDebugUtilsMessengerEXT");

	if (vkDestroyDebugUtilsMessengerEXT != 0) {
		vkDestroyDebugUtilsMessengerEXT(game->renderer_state.backend.instance, game->renderer_state.backend.debug_messenger, 0);
	}

	vkDestroyInstance(game->renderer_state.backend.instance, 0);

	return 0;
}