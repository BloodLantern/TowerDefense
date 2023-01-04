#include <iostream>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Globals.h"
#include "PlayField.h"
#include "LevelEditor.h"
#include "Gui.h"

#include <chrono>

#define WINDOW_TITLE "Fourmi Defense"

int main(int argc, char** argv)
{
	std::srand((uint32_t)std::chrono::system_clock::now().time_since_epoch().count());

	// Setup GLFW
	glfwSetErrorCallback(
		[](int error, const char* description)
		{
			std::cerr << "GLFW error " << error << ": " << description << std::endl;
		}
	);

	if (!glfwInit())
		return 1;

	// GL 3.0 + GLSL 130
	const char* glslVersion = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow* window;
	if (Globals::fullscreen)
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

		window = glfwCreateWindow(mode->width, mode->height, WINDOW_TITLE, monitor, NULL);
	}
	else
	{
		window = glfwCreateWindow(Globals::gWindowWidth, Globals::gWindowWidth,
			WINDOW_TITLE, NULL, NULL);
	}
	if (window == NULL)
		return 1;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable viewports

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glslVersion);

	PlayField pf;
	LevelEditor::bindPlayField(&pf);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glfwGetWindowPos(window, &Globals::gWindowX, &Globals::gWindowY);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		Globals::updateGlobals();
		pf.draw();
		Gui::update();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);

		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);

		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
