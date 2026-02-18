#pragma once
#include <string>
#include "LeoTypes.h"

namespace leo
{
	std::string ReadFile(const std::string& filepath);

	std::string DirNameOf(const std::string& filepath);

	struct ImageData
	{
		i32 width  = 0;
		i32 height = 0;
		i32 bpp    = 0;
		u8* data   = nullptr;

		~ImageData();
	};

	ImageData ReadImageData(const std::string& filepath);

	void FreeImageData(ImageData data);
}