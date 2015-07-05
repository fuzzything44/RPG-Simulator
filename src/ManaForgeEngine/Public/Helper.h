#pragma once
#include <Engine.h>
#include <ENGException.h>

#include <SaveData.h>

// standard library includes
#include <ios>


/// <summary> Loads file to string.</summary>
///
/// <param name="filename"> Filename of the file.</param>
///
/// <returns> The file to string.</returns>
ENGINE_API std::string loadFileToStr(const char* filename);

/// <summary> Loads the shaders.</summary>
///
/// <param name="vertexFile">   The vertex file.</param>
/// <param name="fragmentFile"> The fragment file.</param>
///
/// <returns> The shaders.</returns>
ENGINE_API uint32 LoadShaders(const char* vertexFile, const char* fragmentFile);

/// <summary> Loads the DDS.</summary>
///
/// <param name="imagepath"> The imagepath.</param>
///
/// <returns> The DDS.</returns>
ENGINE_API uint32 loadDDS(const char * imagepath);

///// <summary> Gets the string assiociated with the error code.</summary>
/////
///// <param name="error"> The error.</param>
/////
///// <returns> A the string that is the error.</returns>
//ENGINE_API std::string clGetErrorString(cl_int error);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const vec2& vec);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const vec3& vec);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const vec4& vec);


/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const ivec2& vec);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const ivec3& vec);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const ivec4& vec);


/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const uvec2& vec);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const uvec3& vec);

/// <summary> custom vector printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const uvec4& vec);

/// <summary> custom matrix printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const mat3& mat);

/// <summary> custom matrix printing </summary>
ENGINE_API std::ostream& operator<<(std::ostream& os, const mat4& mat);


// ONLY define these macros if we are debugging -- they could be slow
#ifdef _DEBUG
	

	/// <summary> Checks a pointer, and throws an exception if it is null.</summary>
	#define check(ptr) if(!ptr){  ::logger<Fatal>() << "Null pointer exception caught: " << #ptr; }

	
#else
	#define errChkCL(err)
	#define check(ptr)
#endif

#define LOAD_PROPERTY_WITH_WARNING(propertyManager, key, value, defaultValue)						\
	if(::boost::optional<decltype(value)> tempOptional =											\
		propertyManager.queryValue<decltype(value)>(key))											\
	{																								\
		value = *tempOptional;																		\
	}																								\
	else																							\
	{																								\
		logger<Warning>() << "Value from key " << key												\
			<< " doesn't exist. Using default value of: " << defaultValue;							\
	}																								\
	/**/

#define LOAD_PROPERTY_WITH_ERROR(propertyManager, key, value)										\
	if(::boost::optional<decltype(value)> tempOptional =											\
		propertyManager.queryValue<decltype(value)>(key))											\
	{																								\
		value = *tempOptional;																		\
	}																								\
	else																							\
	{																								\
		logger<Fatal>() << "Value from key " << key << " doesn't exist.";							\
	}																								\
	/**/
