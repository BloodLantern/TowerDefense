#include <iostream>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "globals.hpp"
#include "game.hpp"
#include "level_editor.hpp"
#include "gui.hpp"

#include "discordRPC/discord_rpc.h"
#include "stb_image.h"

#include <chrono>

#define WINDOW_TITLE "Fourmi Defense"
#define WINDOW_WIDTH 1350
#define WINDOW_HEIGHT 700

#define DISCORD_APP_ID "1062095306986115073"

#define FONT_NAME "assets\\fonts\\unispace.ttf"

void SetupDiscordRPC(DiscordRichPresence* rpc)
{
	memset(rpc, 0, sizeof(*rpc));
	rpc->state = "Playing Solo";
	rpc->details = "Wave 1/20";
	rpc->startTimestamp = std::time(nullptr);
	rpc->largeImageKey = "logo";
	rpc->largeImageText = "Fourmi defense";
	rpc->partyId = "ae488379-351d-4a4f-ad32-2b9b01c91657";
	rpc->joinSecret = "MTI4NzM0OjFpMmhuZToxMjMxMjM= ";
	Discord_UpdatePresence(rpc);
}

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
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);

	GLFWwindow* window;
	if (Globals::gFullscreen)
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
		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	}
	if (window == NULL)
		return 1;

	// Set icon
	glfwMakeContextCurrent(window);
	GLFWimage windowIcon;
	windowIcon.pixels = stbi_load("assets/logo.png", &windowIcon.width, &windowIcon.height, 0, 4);

	glfwSetWindowIcon(window, 1, &windowIcon);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable viewports

	io.Fonts->AddFontDefault();
	Globals::gFontBig = io.Fonts->AddFontFromFileTTF(FONT_NAME, TOWER_PANEL_TEXT_SIZE_BIG);
	Globals::gFontSemiBig = io.Fonts->AddFontFromFileTTF(FONT_NAME, 30);
	Globals::gFontMedium = io.Fonts->AddFontFromFileTTF(FONT_NAME, TOWER_PANEL_TEXT_SIZE_MEDIUM);

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glslVersion);

	Game game;
	Globals::InitGlobals(&game);
	game.Init();

	DiscordEventHandlers discordEvents = { 0 };
	Discord_Initialize(DISCORD_APP_ID, &discordEvents, true, nullptr);
	SetupDiscordRPC(&Globals::gDiscordRpc);

	// Show window
	glfwShowWindow(window);

	// Main loop
	while (!glfwWindowShouldClose(window) && !Globals::gClosingGame)
	{
		glfwPollEvents();

		glfwGetWindowPos(window, &Globals::gWindowX, &Globals::gWindowY);
		glfwGetWindowSize(window, &Globals::gWindowWidth, &Globals::gWindowHeight);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		bool _fullscreenPrev = Globals::gFullscreen;

		Globals::UpdateGlobals();
		game.Update();
		Gui::Update();

		if (Globals::gFullscreen != _fullscreenPrev)
		{
			GLFWmonitor* monitor;
			int width;
			int height;
			if (Globals::gFullscreen)
			{
				monitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);
				width = mode->width;
				height = mode->height;
			}
			else
			{
				monitor = nullptr;
				width = WINDOW_WIDTH;
				height = WINDOW_HEIGHT;
			}

			glfwSetWindowMonitor(window, Globals::gFullscreen ? monitor : nullptr, 100, 100, width, height, GLFW_DONT_CARE);
		}

		Discord_UpdatePresence(&Globals::gDiscordRpc);

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

	Globals::DestroyGlobals();

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
