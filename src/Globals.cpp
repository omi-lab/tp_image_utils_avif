#include "tp_image_utils_avif/LoadImages.h"
#include "tp_image_utils_avif/AVIFSequence.h"

#include "tp_image_utils/LoadImages.h"

namespace tp_image_utils_avif
{

namespace
{
tp_image_utils::ColorMap (*loadImageFromData_)(const std::string& data) = nullptr;

//##################################################################################################
tp_image_utils::ColorMap testLoadImageFromData(const std::string& data)
{
  if(isAVIF(data) || isAVIS(data))
    return loadImageFromData(data);

  if(loadImageFromData_)
    return loadImageFromData_(data);

  return tp_image_utils::ColorMap();
}

}

//##################################################################################################
void init()
{
  tp_image_utils::AbstractImageSequence::addFactory([](const std::string& data) -> tp_image_utils::AbstractImageSequence*
  {
    if(isAVIS(data))
      return new AVIFSequence(data);
    return nullptr;
  });

  loadImageFromData_ = tp_image_utils::loadImageFromData_;
  tp_image_utils::loadImageFromData_ = testLoadImageFromData;
}

}

