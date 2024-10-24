#include "testing.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include "general/platformlib/networking.h"
#include "general/platformlib/platform.h"
#include "general/platformlib/opengl_rendering.h"



int main(int argc, char* argv[]) {

	platform_init();
	networking_init();
	opengl_init(4, 3);

	glewInit();

	testing_main();

	opengl_exit();
	networking_exit();
	platform_exit();

	return 0;
}