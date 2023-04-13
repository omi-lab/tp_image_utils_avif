#ifndef tp_image_utils_avif_AVIFSequence_h
#define tp_image_utils_avif_AVIFSequence_h

#include "tp_image_utils_avif/Globals.h" // IWYU pragma: keep

#include "tp_image_utils/AbstractImageSequence.h"

namespace tp_image_utils_avif
{

//##################################################################################################
class AVIFSequence : public tp_image_utils::AbstractImageSequence
{
public:
  //################################################################################################
  AVIFSequence(const std::string& data);

  //################################################################################################
  ~AVIFSequence() override;

  //################################################################################################
  tp_image_utils::ColorMap loadImage(size_t i) override;

private:
  struct Private;
  friend struct Private;
  Private* d;
};

}

#endif
