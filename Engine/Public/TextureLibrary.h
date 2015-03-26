#pragma once
#include "Engine.h"
#include "Logging.h"
#include <GLTools/glew.h>
#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>

// Hex defines for the compression methods.
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

// size of each texture
static const GLuint IMAGE_WIDTH = 256;

// amount of textures in one direction. Size of texture is (IMAGE_WIDTH * TEXTURE_WIDTH)^2
static const GLuint TEXTURE_WIDTH = 16;

struct UVData
{
	UVData(vec2 bottomLeftIn = vec2(0.f, 0.f), vec2 topLeftIn = vec2(0.f, 0.f),
		vec2 bottomRightIn = vec2(0.f, 0.f), vec2 topRightIn = vec2(0.f, 0.f))
		: bottomLeft(bottomLeftIn),
		topLeft(topLeftIn),
		bottomRight(bottomRightIn),
		topRight(topRightIn) { }


	vec2 bottomLeft;
	vec2 topLeft;
	vec2 bottomRight;
	vec2 topRight;
};



class TextureLibrary
{
public:

	static ENGINE_API UVData getUVData(std::string key);

	static ENGINE_API void addTexture(std::string key, const char* filename);


	static ENGINE_API GLuint getTextureHandle();

private:
	

	static std::unordered_map<std::string, UVData> UVDataMap;

	static void appendDDS(GLuint texToAppend, GLuint Xoffset, GLuint Yoffset, const GLchar* filepath);


	static GLuint allocateCompressedTextureLibraryFromDDS(GLuint num, const char* filepath);

	// the location of the next image.
	static uvec2 nextLocation;

	// stores the handles to the textures
	static GLuint texture;
};