#include <stb/stb_image.h>
#include <fstream>
#include <sstream>
#include <LEO/Log/Log.h>
#include "LeoFileUtilities.h"


namespace leo
{
    ImageData::~ImageData()
    {
        FreeImageData(*this);
    }

    std::string ReadFile(const std::string& filepath)
    {
        std::ifstream input_file(filepath);

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
        image_data.data = stbi_load((filepath).c_str(), &image_data.width, &image_data.height, &image_data.bpp, 4);

        if (image_data.data == nullptr)
        {
            LEOLOGERROR("Failed to read image data From: {}", filepath);
            stbi_uc* data_error = new stbi_uc[16]{
                    255,   0, 255, 255, /**/   0,   0,   0, 255,
                      0,   0,   0, 255, /**/ 255,   0, 255, 255
            };
            image_data.width = 2;
            image_data.height = 2;
            image_data.bpp = 4;
            image_data.data = data_error;
        }

        return image_data;
    }

    void FreeImageData(ImageData image_data)
    {
        if (image_data.data)
        {
            stbi_image_free(image_data.data);
        }
    }
}