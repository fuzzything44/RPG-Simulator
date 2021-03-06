#include "OpenGLRendererPCH.h"

#include "OpenGLTextureLibrary.h"

#include "OpenGLRenderer.h"

#include <Logging.h>
#include <ENGException.h>
#include <Helper.h>
#include <vector>

#include "SOIL/SOIL.h"

OpenGLTextureLibrary::OpenGLTextureLibrary(OpenGLRenderer& renderer)
	: nextLocation(0, 0)
	, renderer(renderer)
{
}

OpenGLTextureLibrary::~OpenGLTextureLibrary() { glDeleteTextures(1, &texHandle); }

void OpenGLTextureLibrary::init(uint16 maxElems, uint16 indSize)
{

	individualSize = indSize;

	// if it isn't a square number...
	if (floor(sqrt(float(maxElems))) != sqrt(float(maxElems))) {
		int c = 1;
		for (; maxElems >= c * c; ++c)
			;

		maxElems = c * c;
	}
	maxElements = maxElems;

	renderer.runOnRenderThreadSync([this]
		{

			glGenTextures(1, &texHandle);
			glBindTexture(GL_TEXTURE_2D, texHandle);

			width = static_cast<uint16>(ceil(sqrt(maxElements)));

			texHandle = allocateCompressedTextureLibraryFromDDS(width, "textures\\0.dds");
		});
}

void OpenGLTextureLibrary::addImage(const std::string& name)
{
	renderer.runOnRenderThreadSync([this, &name]
		{
			appendDDS(texHandle,
				nextLocation.x * individualSize,
				nextLocation.y * individualSize,
				("textures\\" + name + ".dds").c_str());
		});

	QuadUVCoords data;
	data.upperLeft = vec2(nextLocation) / vec2(width);
	data.upperRight = vec2(nextLocation.x + 1, nextLocation.y) / vec2(width);
	data.lowerLeft = vec2(nextLocation.x, nextLocation.y + 1) / vec2(width);
	data.lowerRight = vec2(nextLocation.x + 1, nextLocation.y + 1) / vec2(width);

	UVs[name] = data;

	++nextLocation.x;

	if (nextLocation.x >= width) {
		++nextLocation.y;
		nextLocation.x = 0;
	}
}

boost::optional<QuadUVCoords> OpenGLTextureLibrary::getUVCoords(const std::string& name)
{
	auto iter = UVs.find(name);

	// if it exists
	if (iter != UVs.end()) {
		return iter->second;
	}

	MFLOG(Warning) << "Cannot find UVCoord named " << name;

	return boost::optional<QuadUVCoords>(); // return the "null" version
}

uint32 OpenGLTextureLibrary::getID() { return texHandle; }

void OpenGLTextureLibrary::setFilterMode(FilterMode newMode)
{
	renderer.runOnRenderThreadSync([this, newMode]
		{
			glBindTexture(GL_TEXTURE_2D, texHandle);

			switch (newMode)
			{
			case FilterMode::LINEAR:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			case FilterMode::NEAREST:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			case FilterMode::MIPMAP_LINEAR:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			case FilterMode::MIPMAP_NEAREST:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			default: break;
			}
		});
}

Texture::FilterMode OpenGLTextureLibrary::getFilterMode() const
{
	return renderer.runOnRenderThreadSync([this]
		{
			glBindTexture(GL_TEXTURE_2D, texHandle);

			int mode;
			glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &mode);

			switch (mode)
			{
			case GL_LINEAR: return Texture::FilterMode::LINEAR; break;
			case GL_NEAREST: return Texture::FilterMode::NEAREST; break;
			case GL_LINEAR_MIPMAP_LINEAR: return Texture::FilterMode::MIPMAP_LINEAR; break;
			case GL_NEAREST_MIPMAP_NEAREST: return Texture::FilterMode::MIPMAP_NEAREST; break;
			default: return Texture::FilterMode::LINEAR; // IDK break;
			}
		});
}

void OpenGLTextureLibrary::setWrapMode(WrapMode newMode)
{
	renderer.runOnRenderThreadAsync([this, newMode]
		{
			glBindTexture(GL_TEXTURE_2D, texHandle);
			switch (newMode)
			{
			case WrapMode::CLAMP_TO_EDGE:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				break;
			case WrapMode::MIRRORED_REPEAT:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
				break;
			case WrapMode::REPEAT:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				break;
			default: break;
			}
		});
}

Texture::WrapMode OpenGLTextureLibrary::getWrapMode() const
{
	return renderer.runOnRenderThreadSync([this]
		{
			glBindTexture(GL_TEXTURE_2D, texHandle);

			GLint wrap;

			glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &wrap); // retreive the
																		  // data

			switch (wrap)
			{
			case GL_CLAMP_TO_EDGE: return WrapMode::CLAMP_TO_EDGE;
			case GL_MIRRORED_REPEAT: return WrapMode::MIRRORED_REPEAT;
			case GL_REPEAT: return WrapMode::REPEAT;
			default: return WrapMode::REPEAT; // just a deault fallback
			}
		});
}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

void OpenGLTextureLibrary::appendDDS(uint32 texToAppend, uint32 Xoffset, uint32 Yoffset, const char* filepath)
{

	unsigned char header[124];

	FILE* fp;

	/* try to open the file */
	fopen_s(&fp, filepath, "rb");
	if (fp == nullptr) MFLOG(Fatal) << "cannot open file: " << filepath;

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		MFLOG(Fatal) << "file not a dds: " << filepath;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *reinterpret_cast<unsigned int*>(&(header[8]));
	unsigned int width = *reinterpret_cast<unsigned int*>(&(header[12]));
	unsigned int linearSize = *reinterpret_cast<unsigned int*>(&(header[16]));
	unsigned int mipMapCount = *reinterpret_cast<unsigned int*>(&(header[24]));
	unsigned int fourCC = *reinterpret_cast<unsigned int*>(&(header[80]));

	unsigned char* buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = static_cast<unsigned char*>(malloc(bufsize * sizeof(unsigned char)));
	assert(buffer);
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	// unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1: format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; break;
	case FOURCC_DXT3: format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; break;
	case FOURCC_DXT5: format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;
	default: free(buffer); MFLOG(Fatal) << "unrecgnized compressed DDS format: " << filepath;
	}

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	glBindTexture(GL_TEXTURE_2D, texToAppend);

	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {

		unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

		// these need to be a multiple of 4 according to the S3TC spec
		if (Xoffset % 4 == 0 && Yoffset % 4 == 0 && width % 4 == 0 && height % 4 == 0) {
			glCompressedTexSubImage2D(
				GL_TEXTURE_2D, level, Xoffset, Yoffset, width, height, format, size, buffer + offset);
		}

		offset += size;
		width /= 2;
		height /= 2;
		Xoffset /= 2;
		Yoffset /= 2;
	}
	free(buffer);
}

uint32 OpenGLTextureLibrary::allocateCompressedTextureLibraryFromDDS(uint32 num, const char* filepath)
{
	unsigned char header[124];

	FILE* fp;

	/* try to open the file */
	fopen_s(&fp, filepath, "rb");
	if (fp == nullptr) return 0;

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *reinterpret_cast<unsigned int*>(&(header[8]));
	unsigned int width = *reinterpret_cast<unsigned int*>(&(header[12]));
	// unsigned int linearSize = *reinterpret_cast<unsigned int*>(&(header[16]));
	unsigned int mipMapCount = *reinterpret_cast<unsigned int*>(&(header[24]));
	unsigned int fourCC = *reinterpret_cast<unsigned int*>(&(header[80]));

	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1: format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; break;
	case FOURCC_DXT3: format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; break;
	case FOURCC_DXT5: format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;
	default: return 0;
	}

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	// unsigned int offset = 0;

	uint32 texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	int extraMips = static_cast<int>(floorf(log2f(static_cast<float>(num))));
	mipMapCount += extraMips;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipMapCount - 1);

	width *= num;
	height *= num;
	uint32 size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

	std::vector<GLubyte> zeros(size);

	for (uint32 i = 0; i < mipMapCount; i++) {

		size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

		glCompressedTexImage2D(GL_TEXTURE_2D, i, format, width, height, 0, size, &zeros[0]);

		width /= 2;
		height /= 2;
	}
	return texture;
}
