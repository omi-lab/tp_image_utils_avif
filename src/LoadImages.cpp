#include "tp_image_utils_avif/LoadImages.h"

#include "tp_image_utils/ColorMap.h"

// You need LibAVIF the installation instructions can be found here:
// tp_image_utils_avif/README.md
#include <avif/avif.h>

#include <cstring>

namespace tp_image_utils_avif
{

//##################################################################################################
tp_image_utils::ColorMap loadImageFromData(const std::string& data)
{
  tp_image_utils::ColorMap image;

  avifRGBImage rgb;
  std::memset(&rgb, 0, sizeof(rgb));

  avifDecoder* decoder = avifDecoderCreate();
  TP_CLEANUP([&]{avifDecoderDestroy(decoder);});

  if(avifDecoderSetIOMemory(decoder,
                            reinterpret_cast<const uint8_t*>(data.c_str()),
                            data.size()) != AVIF_RESULT_OK)
    return tp_image_utils::ColorMap();

  if (avifDecoderParse(decoder) != AVIF_RESULT_OK)
    return tp_image_utils::ColorMap();

  if(avifDecoderNextImage(decoder) == AVIF_RESULT_OK)
  {
    image.setSize(size_t(decoder->image->width), size_t(decoder->image->height));

    rgb.width = decoder->image->width;
    rgb.height = decoder->image->height;
    rgb.depth = 8;
    rgb.format = AVIF_RGB_FORMAT_RGBA;
    rgb.chromaUpsampling = AVIF_CHROMA_UPSAMPLING_AUTOMATIC;
    rgb.ignoreAlpha = AVIF_TRUE;
    rgb.alphaPremultiplied = AVIF_FALSE;
    rgb.pixels = &image.data()->r;
    rgb.rowBytes = uint32_t(image.width()*4);

    if (avifImageYUVToRGB(decoder->image, &rgb) != AVIF_RESULT_OK)
      return tp_image_utils::ColorMap();
  }

  return image;
}

//##################################################################################################
bool isAVIF(const std::string& data)
{
  return (data.size()>12 && (data.substr(4, 8) == "ftypavif"));
}

//##################################################################################################
bool isAVIS(const std::string& data)
{
  return (data.size()>12 && (data.substr(4, 8) == "ftypavis"));
}

}
