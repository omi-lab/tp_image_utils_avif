#ifndef tp_image_utils_avif_LoadImages_h
#define tp_image_utils_avif_LoadImages_h

#include "tp_image_utils_avif/Globals.h" // IWYU pragma: keep

#include "tp_image_utils/ColorMap.h"

namespace tp_image_utils_avif
{

//##################################################################################################
tp_image_utils::ColorMap loadImageFromData(const std::string& data);

//##################################################################################################
//! Returns true if this has an AVIF image header.
bool isAVIF(const std::string& data);

//##################################################################################################
//! Returns true if this has an AVIS image sequence header
bool isAVIS(const std::string& data);

}

#endif
