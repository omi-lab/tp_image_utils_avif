#include "tp_image_utils_avif/AVIFSequence.h"

#include "tp_image_utils/ColorMap.h"

// You need LibAVIF the installation instructions can be found here:
// tp_image_utils_avif/README.md
#include <avif/avif.h>

#include <cstring>

namespace tp_image_utils_avif
{

//##################################################################################################
struct AVIFSequence::Private
{
  std::string data;
  avifDecoder* decoder{nullptr};
  bool ready{false};

  //################################################################################################
  Private(const std::string& data_):
    data(data_)
  {
    decoder = avifDecoderCreate();
    if(avifDecoderSetIOMemory(decoder,
                              reinterpret_cast<const uint8_t*>(data.c_str()),
                              data.size()) != AVIF_RESULT_OK)
      return;

    if(avifDecoderParse(decoder) != AVIF_RESULT_OK)
      return;

    ready = true;
  }

  //################################################################################################
  ~Private()
  {
    if(decoder)
      avifDecoderDestroy(decoder);
  }
};

//##################################################################################################
AVIFSequence::AVIFSequence(const std::string& data):
  d(new Private(data))
{

}

//##################################################################################################
AVIFSequence::~AVIFSequence()
{
  delete d;
}

//##################################################################################################
tp_image_utils::ColorMap AVIFSequence::loadImage(size_t i)
{
  if(!d->ready)
    return tp_image_utils::ColorMap();

  tp_image_utils::ColorMap image;

  avifRGBImage rgb;
  std::memset(&rgb, 0, sizeof(rgb));

  if(auto r=avifDecoderNthImage(d->decoder, uint32_t(i)); r == AVIF_RESULT_OK)
  {
    image.setSize(size_t(d->decoder->image->width), size_t(d->decoder->image->height));

    rgb.width = d->decoder->image->width;
    rgb.height = d->decoder->image->height;
    rgb.depth = 8;
    rgb.format = AVIF_RGB_FORMAT_RGBA;
    rgb.chromaUpsampling = AVIF_CHROMA_UPSAMPLING_AUTOMATIC;
    rgb.ignoreAlpha = AVIF_TRUE;
    rgb.alphaPremultiplied = AVIF_FALSE;
    rgb.pixels = &image.data()->r;
    rgb.rowBytes = uint32_t(image.width()*4);

    if(avifImageYUVToRGB(d->decoder->image, &rgb) != AVIF_RESULT_OK)
      return tp_image_utils::ColorMap();
  }

  return image;
}

}
