#pragma once
#include "OpenGLRendererConfig.h"

#include "OpenGLCharacterData.h"

#include <Font.h>

#include <unordered_map>
#include <string>

class OpenGLMaterialSource;
class OpenGLTexture;
class OpenGLRenderer;

class OpenGLFont : public Font
{
public:
	OpenGLFont(OpenGLRenderer& renderer, const std::string& name = std::string());
	OpenGLFont(const OpenGLFont& other) = delete;
	OpenGLFont(OpenGLFont&& other) = default;

	OpenGLCharacterData getCharacterData(wchar_t ch);
	std::shared_ptr<OpenGLMaterialSource> getMaterialSource();

private:
	std::string fontName;

	std::unordered_map<wchar_t, OpenGLCharacterData> charData;

	std::shared_ptr<OpenGLMaterialSource> matSource;
	std::shared_ptr<OpenGLTexture> tex;

	OpenGLRenderer& renderer;
};
