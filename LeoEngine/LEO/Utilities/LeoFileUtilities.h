#pragma once
#include <string>
#include <memory>
#include "LeoTypes.h"

namespace leo
{
	std::string ReadFile(const std::string& filepath);

	std::string DirNameOf(const std::string& filepath);

	struct ImageDataDeleter
	{
		void operator()(u8* ptr) const;
	};

	class ImageData
	{
	public:
		ImageData() = default;

		ImageData(ImageData&&) noexcept = default;
		ImageData& operator=(ImageData&&) noexcept = default;

		ImageData(const ImageData&) = delete;
		ImageData& operator=(const ImageData&) = delete;
	public:
		i32 width = 0;
		i32 height = 0;
		i32 bpp = 0;
		std::unique_ptr<u8, ImageDataDeleter> data;
	};

	ImageData ReadImageData(const std::string& filepath);
}