#include <stb/stb_image.h>
#include <fstream>
#include <sstream>
#include <LEO/Log/Log.h>
#include "LeoFileUtilities.h"


namespace leo
{
    std::string ReadFile(const std::string& filepath)
    {
        std::ifstream input_file(filepath, std::ios::binary);

        if (!input_file.is_open())
        {
            //LEOLOGERROR("Failed to open file at {}", filepath.c_str());
            return std::string();
        }

        return std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    }

    std::string DirNameOf(const std::string& filepath)
    {
        size_t pos = filepath.find_last_of("\\/");
        return (std::string::npos == pos) ? "" : filepath.substr(0, pos);
    }

    ImageData ReadImageData(const std::string& filepath)
    {
        ImageData image_data;

        stbi_set_flip_vertically_on_load(1);

        int width, height, bpp;
        u8* raw = stbi_load(filepath.c_str(), &width, &height, &bpp, 4);

        if (raw == nullptr)
        {
            LEOLOGERROR("Failed to read image data From: {}", filepath);

            // Fallback 2x2 magenta checker
            const u8 fallback[16] = {
                255,   0, 255, 255,   0,   0,   0, 255,
                  0,   0,   0, 255, 255,   0, 255, 255
            };

            u8* fallbackHeap = static_cast<u8*>(std::malloc(16));
            if (!fallbackHeap)
            {
                LEOLOGERROR("Fallback allocation failed for image: {}", filepath);
                return image_data; // empty image
            }

            std::memcpy(fallbackHeap, fallback, 16);

            image_data.width = 2;
            image_data.height = 2;
            image_data.bpp = 4;
            image_data.data.reset(fallbackHeap);

            return image_data;
        }

        image_data.width = width;
        image_data.height = height;
        image_data.bpp = 4;

        image_data.data.reset(raw);

        return image_data;
    }

    void ImageDataDeleter::operator()(u8* ptr) const
    {
        if (ptr != nullptr) {
            stbi_image_free(ptr);
        }
    }
}