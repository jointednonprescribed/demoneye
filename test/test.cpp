
#include <demoneye.h>
#include <iostream>

#include "../sdk/lib/gl.cxx"
#include "../sdk/lib/gl.GLAD.cxx"
#include "../sdk/lib/gl.GLFWContext.cxx"
#include "../sdk/lib/gl.GLShaderContext.cxx"
#include "../sdk/lib/gl.GLShaderModule.cxx"
#include "../sdk/lib/gl.GLWindow.cxx"
#include "../sdk/lib/setup.types.iTerminatable.cxx"

int main()
{
	using namespace std;
	using namespace demoneye;

	GLFWContext glfw;

	if (!glfw.IsInitialized()) {
		cerr << "Failed to initialied GLFW." << endl;
		return -1;
	}

	GLWindow wnd(800, 600, "Demoneye 2025 Engine Test");
	if (!wnd) {
		cerr << "Failed to create the window." << endl;
		return -1;
	}
	wnd.MakeCurrentContext();

	if (!GLAD::LoadGLFWLoader()) {
		cerr << "Failed GLAD initialiation." << endl;
		return -1;
	}

	glClearColor(0.85f, 0.13f, 0.29f, 1.0f);

	GLShaderModule vertex("vertex", GL_VERTEX_SHADER), fragment("fragment", GL_FRAGMENT_SHADER);

	vertex.AddSource("/p/jointednonprescribed/demoneye/main/resources/default/shaders/vertex2d.glsh.c");
	vertex.AddSource("/p/jointednonprescribed/demoneye/main/resources/default/shaders/fragment2d.glsh.c");

	GLShaderContext shaders(2, (GLShaderMod[]){vertex, fragment});

	wnd.MakeViewport();

	wnd.MainLoop(predicate([](){glClear(GL_COLOR_BUFFER_BIT); return true;}));

	return 0;
}
