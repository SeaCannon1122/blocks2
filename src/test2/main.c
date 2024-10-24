#include "testing.h"
#include <stdio.h>

#include "general/platformlib/networking.h"
#include "general/platformlib/platform.h"

#include <vulkan/vulkan.h>

VkExtent2D screen_size;

VkInstance instance;
VkDebugUtilsMessengerEXT debug_messenger;
VkSurfaceKHR surface;
VkPhysicalDevice gpu;
VkDevice device;
VkQueue graphics_queue;
VkSwapchainKHR swapchain;
VkImage sc_images[5];
uint32_t sc_image_count;
VkCommandPool command_pool;
VkSemaphore submit_semaphore;
VkSemaphore aquire_semaphore;

VkFramebuffer framebuffers[5];

VkImageView sc_image_views[5];

VkRenderPass render_pass;

#define VKCall(call) \
do { \
    VkResult result = (call); \
    if (result != VK_SUCCESS) { \
        printf("Vulkan error in \n    %s \n at %s:%d: %d\n", #call, __FILE__, __LINE__, result); \
		DEBUG_BREAK();\
    } \
} while(0)



static VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_debug_callbck(
	VkDebugUtilsMessageSeverityFlagBitsEXT msg_severity,
	VkDebugUtilsMessageTypeFlagsEXT msg_flags,
	const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
	void* p_user_data
) {
	printf("--------------------------------------------------------------------------\n\nValidation Error: %s\n\n\n\n", p_callback_data->pMessage);
	return false;
}

int main(int argc, char* argv[]) {

	platform_init();
	networking_init();

	show_console_window();

	int window = window_create(10, 10, 1500, 1000, "window");

	screen_size.width = window_get_width(window);
	screen_size.height = window_get_height(window);

	VkApplicationInfo app_info = { 0 };
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "testapp";
	app_info.pEngineName = "testengine";

	char* instance_extensions[] = {
		PLATFORM_VK_SURFACE_EXTENSION,
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};

	char* layers[] = {
		"VK_LAYER_KHRONOS_validation",
	};

	VkInstanceCreateInfo instance_info = { 0 };
	instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_info.pApplicationInfo = &app_info;
	instance_info.ppEnabledExtensionNames = instance_extensions;
	instance_info.enabledExtensionCount = 3;
	instance_info.ppEnabledLayerNames = layers;
	instance_info.enabledLayerCount = 1;

	VKCall(vkCreateInstance(&instance_info, 0, &instance));

	PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if (vkCreateDebugUtilsMessengerEXT) {

		VkDebugUtilsMessengerCreateInfoEXT debug_info = { 0 };
		debug_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debug_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
		debug_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
		debug_info.pfnUserCallback = vulkan_debug_callbck;

		vkCreateDebugUtilsMessengerEXT(instance, &debug_info, 0, &debug_messenger);
	}


	VKCall(create_vulkan_surface(instance, window, &surface));

	uint32_t gpu_count = 0;
	VkPhysicalDevice gpus[10];

	uint32_t graphics_idx = -1;

	VKCall(vkEnumeratePhysicalDevices(instance, &gpu_count, 0));
	VKCall(vkEnumeratePhysicalDevices(instance, &gpu_count, gpus));

	for (uint32_t i = 0; i < gpu_count; i++) {

		uint32_t queue_family_count = 0;

		VkQueueFamilyProperties queue_props[10];

		vkGetPhysicalDeviceQueueFamilyProperties(gpus[i], &queue_family_count, 0);
		vkGetPhysicalDeviceQueueFamilyProperties(gpus[i], &queue_family_count, queue_props);

		uint32_t j = 0;
		for (; j < queue_family_count; j++) {

			if (queue_props[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				
				VkBool32 surface_support = VK_FALSE;
				VKCall(vkGetPhysicalDeviceSurfaceSupportKHR(gpus[i], j, surface, &surface_support));
				
				if (surface_support) {

					graphics_idx = j;
					gpu = gpus[i];
					break;
				}
					
			}

		}

		if (j != queue_family_count) break;

	}

	if (graphics_idx == -1) {
		printf("couldnt fint suitable device");
		goto close;
	}


	float queue_priority = 1.0f;

	VkDeviceQueueCreateInfo queue_info = { 0 };
	queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_info.queueFamilyIndex = graphics_idx;
	queue_info.queueCount = 1;
	queue_info.pQueuePriorities = &queue_priority;

	char* device_extentions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	VkDeviceCreateInfo device_info = { 0 };
	device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_info.pQueueCreateInfos = &queue_info;
	device_info.queueCreateInfoCount = 1;
	device_info.ppEnabledExtensionNames = device_extentions;
	device_info.enabledExtensionCount = 1;

	VKCall(vkCreateDevice(gpu, &device_info, 0, &device));

	vkGetDeviceQueue(device, graphics_idx, 0, &graphics_queue);

	VkSurfaceFormatKHR surface_format;


	uint32_t format_count = 0;
	VkSurfaceFormatKHR surface_formats[10];
	VKCall(vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count, 0));
	VKCall(vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, surface, &format_count, surface_formats));

	for (int i = 0; i < format_count; i++) {

		if (surface_formats[i].format == VK_FORMAT_B8G8R8A8_SRGB) {
			surface_format = surface_formats[i];
			break;
		}
	}
	

	VkSurfaceCapabilitiesKHR surface_capabilities = { 0 };
	VKCall(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, surface, &surface_capabilities));

	sc_image_count = surface_capabilities.minImageCount + 1;
	if (surface_capabilities.minImageCount == surface_capabilities.maxImageCount) sc_image_count = surface_capabilities.minImageCount;

	VkSwapchainCreateInfoKHR sc_info = { 0 };
	sc_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	sc_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	sc_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; 
	sc_info.surface = surface;
	sc_info.imageFormat = surface_format.format;
	sc_info.preTransform = surface_capabilities.currentTransform;
	sc_info.imageExtent = surface_capabilities.currentExtent;
	sc_info.minImageCount = sc_image_count;
	sc_info.imageArrayLayers = 1;

	VKCall(vkCreateSwapchainKHR(device, &sc_info, 0, &swapchain));

	VKCall(vkGetSwapchainImagesKHR(device, swapchain, &sc_image_count, 0));
	VKCall(vkGetSwapchainImagesKHR(device, swapchain, &sc_image_count, sc_images));

	VkImageViewCreateInfo image_view_info = { 0 };
	image_view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	image_view_info.format = surface_format.format;
	image_view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	image_view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	image_view_info.subresourceRange.layerCount = 1;
	image_view_info.subresourceRange.levelCount = 1;

	for (uint32_t i = 0; i < sc_image_count; i++) {

		image_view_info.image = sc_images[i];
		VKCall(vkCreateImageView(device, &image_view_info, 0, &sc_image_views[i]));
	}



	VkAttachmentDescription attachment = { 0 };
	attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	attachment.format = surface_format.format;

	VkAttachmentReference color_attachment_reference = { 0 };
	color_attachment_reference.attachment = 0;
	color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass_description = { 0 };
	subpass_description.colorAttachmentCount = 1;
	subpass_description.pColorAttachments = &color_attachment_reference;

	VkAttachmentDescription attachments[] = {
		attachment
	};

	VkRenderPassCreateInfo render_pass_info = { 0 };
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_info.pAttachments = attachments;
	render_pass_info.attachmentCount = 1;
	render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = &subpass_description;

	VKCall(vkCreateRenderPass(device, &render_pass_info, 0, &render_pass));

	VkFramebufferCreateInfo framebuffer_info = { 0 };
	framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebuffer_info.width = screen_size.width;
	framebuffer_info.height = screen_size.height;
	framebuffer_info.renderPass = render_pass;
	framebuffer_info.layers = 1;
	framebuffer_info.attachmentCount = 1;

	for (uint32_t i = 0; i < sc_image_count; i++) {
		framebuffer_info.pAttachments = &sc_image_views[i];
		VKCall(vkCreateFramebuffer(device, &framebuffer_info, 0, &framebuffers[i]));
	}


	VkCommandPoolCreateInfo pool_info = { 0 };
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.queueFamilyIndex = graphics_idx;

	VKCall(vkCreateCommandPool(device, &pool_info, 0, &command_pool));

	VkSemaphoreCreateInfo sema_info = { 0 };
	sema_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VKCall(vkCreateSemaphore(device, &sema_info, 0, &aquire_semaphore));
	VKCall(vkCreateSemaphore(device, &sema_info, 0, &submit_semaphore));


	while (!get_key_state(KEY_ESCAPE) && window_is_active(window)) {

		uint32_t img_index;
		VKCall(vkAcquireNextImageKHR(device, swapchain, 0, aquire_semaphore, 0, &img_index));

		VkCommandBuffer cmd;
		VkCommandBufferAllocateInfo alloc_info = { 0 };
		alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		alloc_info.commandBufferCount = 1;
		alloc_info.commandPool = command_pool;
		VKCall(vkAllocateCommandBuffers(device, &alloc_info, &cmd));

		VkCommandBufferBeginInfo begin_info = { 0 };
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		VKCall(vkBeginCommandBuffer(cmd, &begin_info));

		VkClearValue clear_value = { 0 };
		clear_value.color = (VkClearColorValue){ 1, 1, 0, 1 };

		VkRenderPassBeginInfo renderpass_begin_info = { 0 };
		renderpass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderpass_begin_info.renderPass = render_pass;
		renderpass_begin_info.renderArea.extent = screen_size;
		renderpass_begin_info.framebuffer = framebuffers[img_index];
		renderpass_begin_info.pClearValues = &clear_value;
		renderpass_begin_info.clearValueCount = 1;

		vkCmdBeginRenderPass(cmd, &renderpass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdEndRenderPass(cmd);

		VKCall(vkEndCommandBuffer(cmd));

		VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		VkSubmitInfo submit_info = { 0 };
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.pWaitDstStageMask = &wait_stage;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &cmd;
		submit_info.pSignalSemaphores = &submit_semaphore;
		submit_info.pWaitSemaphores = &aquire_semaphore;
		submit_info.signalSemaphoreCount = 1;
		submit_info.waitSemaphoreCount = 1;


		VKCall(vkQueueSubmit(graphics_queue, 1, &submit_info, 0));

		VkPresentInfoKHR present_info = { 0 };
		present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.pSwapchains = &swapchain;
		present_info.swapchainCount = 1;
		present_info.pImageIndices = &img_index;
		present_info.pWaitSemaphores = &submit_semaphore;
		present_info.waitSemaphoreCount = 1;

		VKCall(vkQueuePresentKHR(graphics_queue, &present_info));

		VKCall(vkDeviceWaitIdle(device));

		vkFreeCommandBuffers(device, command_pool, 1, &cmd);



		sleep_for_ms(10);
	}
	

close:
	window_destroy(window);

	networking_exit();
	platform_exit();

	return 0;
}