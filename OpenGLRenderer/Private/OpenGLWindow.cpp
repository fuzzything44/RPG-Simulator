#include "OpenGLRendererConfig.h"

#include "OpenGLWindow.h"
#include "Logging.h"
#include <Runtime.h>

std::map<GLFWwindow*, OpenGLWindow*> OpenGLWindow::windows = std::map<GLFWwindow*, OpenGLWindow*>();

OpenGLWindow::OpenGLWindow(const WindowProps& props) 
	: Window(),
	props(props)
{
	hasFocus = true;

	// init GLFW (our window handler)
	int err = glfwInit();
	if (err != 1)
	{
		ENG_LOG("Failed to init GLFW" << std::endl);

		return;
	}

	GLFWmonitor* mon = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(mon);

	// set AA
	glfwWindowHint(GLFW_SAMPLES, 8);

	glfwWindowHint(GL_DOUBLEBUFFER, false);



	// set GL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	

	// set profile to core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// set the window to non-resizable
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// create the winodw
	switch (props.windowMode)
	{
	case WindowMode::FULLSCREEN:
		window = glfwCreateWindow(props.size.x, props.size.y, props.title.c_str(), mon, nullptr);
		break;
	case WindowMode::FULLSCREEN_WINDOWED:
		window = glfwCreateWindow(mode->width, mode->height, props.title.c_str(), nullptr, nullptr);
		break;

	case WindowMode::WINDOWED:
		window = glfwCreateWindow(props.size.x, props.size.y, props.title.c_str(), nullptr, nullptr);
		break;

	default:

		break;
	}



	// exit if the window wasn't initialized correctly
	if (!window)
	{
		ENG_LOG("\nWindow failed to create. Exiting");
		// terminate the glfw session
		glfwTerminate();
		return;
	}


	windows[window] = this;

	// make context current in this thread
	glfwMakeContextCurrent(window);

	// use newer GL
	glewExperimental = GL_TRUE;

	// make sure the cursor is shown. Most likely want to change this in the future
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);



	// init GL (glew is an extension that does this for us)
	err = glewInit();
	if (err != GLEW_OK)
	{
		ENG_LOG("Failed to init GLEW. err code: ");
		// terminate the glfw session
		glfwTerminate();
		return;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


	glfwSetScrollCallback(window, &OpenGLWindow::scrollCallback);


	glfwSetWindowFocusCallback(window, &OpenGLWindow::focusCallback);
}

OpenGLWindow::~OpenGLWindow()
{
}

const WindowProps& OpenGLWindow::getWindowProps() const
{
	return props;
}

void OpenGLWindow::setWindowProps(const WindowProps& props)
{
	this->props = props;

	updateProps();
}

int OpenGLWindow::getIsKeyPressed(Keyboard key)
{
	return glfwGetKey(window, static_cast<int>(key));
}

vec2 OpenGLWindow::getCursorLocPixels()
{
	dvec2 locationdouble;
	glfwGetCursorPos(window, &locationdouble.x, &locationdouble.y);

	return static_cast<vec2>(locationdouble);
}

void OpenGLWindow::swapBuffers()
{
	glfwSwapBuffers(window);
}

void OpenGLWindow::pollEvents()
{
	glfwPollEvents();
}

bool OpenGLWindow::shouldClose()
{
	return static_cast<bool>(glfwWindowShouldClose(window));
}

void OpenGLWindow::updateProps()
{
	glfwSetWindowSize(window, props.size.x, props.size.y);
	glfwSetWindowTitle(window, props.title.c_str());

	switch (props.renderMode)
	{
	case RenderMode::NORMAL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case RenderMode::WIREFRAME:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	default:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}

	// show the window if it should be visible
	props.visible ? glfwShowWindow(window) : glfwHideWindow(window);
	

}

void OpenGLWindow::scrollCallback(GLFWwindow* window, double x, double y)
{
	auto iter = windows.find(window);
	if (iter != windows.end())
	{
		// iter->second->scroll(x, y);
	}
}

void OpenGLWindow::focusCallback(GLFWwindow* window, int x)
{
	auto iter = windows.find(window);
	if (iter != windows.end())
	{
		// iter->second->focus(x);
	}
}