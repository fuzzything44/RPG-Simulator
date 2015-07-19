#include "OpenGLMaterialSource.h"

#include "Helper.h"

OpenGLMaterialSource::OpenGLMaterialSource(const path_t& name)
{
	if (!name.empty())
		init(name);
}

OpenGLMaterialSource::~OpenGLMaterialSource()
{

	glDeleteProgram(program);
}

void OpenGLMaterialSource::init(const path_t& name)
{
	path_t vertexPath = L"shaders\\" + name.wstring() + L"vert.glsl";
	path_t fragPath = L"shaders\\" + name.wstring() + L"frag.glsl";

	// Create the shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string&& VertexShaderCode = loadFileToStr(vertexPath);

	// Read the Fragment Shader code from the file
	std::string&& FragmentShaderCode = loadFileToStr(fragPath);


	int32 Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	logger<Trace>() << "Compiling vertex Shader " << vertexPath;

	const char* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(vertexShader, 1, &VertexSourcePointer, nullptr);
	glCompileShader(vertexShader);

	// Check Vertex Shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 1){
		auto VertexShaderErrorMessage = std::vector<char>(InfoLogLength + 1);
		glGetShaderInfoLog(vertexShader, InfoLogLength, nullptr, VertexShaderErrorMessage.data());
		logger<Error>() << VertexShaderErrorMessage.data();


	}
	else
	{
		logger<Trace>() << "Shader " << vertexPath << " Successfully Compiled\n";
	}


	// Compile Fragment Shader
	logger<Trace>() << "\nCompiling fragment Shader " << fragPath;
	const char* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(fragmentShader, 1, &FragmentSourcePointer, nullptr);
	glCompileShader(fragmentShader);

	// Check Fragment Shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 1){
		auto FragmentShaderErrorMessage = std::vector<char>(InfoLogLength + 1);
		glGetShaderInfoLog(fragmentShader, InfoLogLength, nullptr, FragmentShaderErrorMessage.data());
		logger<Error>() << FragmentShaderErrorMessage.data();
	}
	else
	{
		logger<Trace>() << "Shader " << fragPath << " Successfully Compiled\n";
	}


	// Link the program
	logger<Trace>() << "\tLinking program " << name;
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	// Check the program
	glGetProgramiv(program, GL_LINK_STATUS, &Result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 1){
		auto ProgramErrorMessage = std::vector<char>(InfoLogLength + 1);
		glGetProgramInfoLog(program, InfoLogLength, nullptr, ProgramErrorMessage.data());
		logger<Error>() << ProgramErrorMessage.data();
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

std::string OpenGLMaterialSource::getName()
{
	return name;
}