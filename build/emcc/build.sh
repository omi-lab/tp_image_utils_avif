set -xe

export EMSDK_PATH=/home/tom/projects/external/emsdk/
export AOM_DOWNLOAD="https://aomedia.googlesource.com/aom/+archive/287164de79516c25c8c84fd544f67752c170082a.tar.gz" #v3.2.0

export CMAKE_TOOLCHAIN_FILE=${EMSDK_PATH}upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake

export LIBAVIF_SRC=`realpath libavif`

export CROSS_FILE=`realpath cross_file.txt`

rm -rf install
mkdir -p install
export INSTALL_DIR=`realpath install`

export LIBAVIF_AOM_SRC="${LIBAVIF_SRC}/ext/aom"
export LIBAVIF_AOM_BUILD="${LIBAVIF_AOM_SRC}/build.libavif"

export LIBAVIF_DAV1D_SRC="${LIBAVIF_SRC}/ext/dav1d"
export LIBAVIF_DAV1D_BUILD="${LIBAVIF_DAV1D_SRC}/build"

source ${EMSDK_PATH}emsdk_env.sh

rm -rf libavif
git clone git@github.com:AOMediaCodec/libavif.git

# rm -rf $LIBAVIF_AOM_SRC || true
# mkdir -p $LIBAVIF_AOM_BUILD && cd $LIBAVIF_AOM_BUILD
# curl -fsSL $AOM_DOWNLOAD | tar xz -C $LIBAVIF_AOM_SRC

# emcmake cmake $LIBAVIF_AOM_SRC \
#   -DCMAKE_BUILD_TYPE=Release \
#   -DCMAKE_CXX_FLAGS="-msimd128" \
#   -DENABLE_CCACHE=1 \
#   -DAOM_TARGET_CPU=generic \
#   -DAOM_EXTRA_C_FLAGS=-UNDEBUG \
#   -DAOM_EXTRA_CXX_FLAGS=-UNDEBUG \
#   -DENABLE_DOCS=0 \
#   -DENABLE_EXAMPLES=0 \
#   -DENABLE_TESTDATA=0 \
#   -DENABLE_TESTS=0 \
#   -DENABLE_TOOLS=0 \
#   -DCONFIG_ACCOUNTING=1 \
#   -DCONFIG_INSPECTION=1 \
#   -DCONFIG_MULTITHREAD=0 \
#   -DCONFIG_RUNTIME_CPU_DETECT=0 \
#   -DCONFIG_WEBM_IO=0 \
#   -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE

# emmake make -j`nproc` VERBOSE=1

# cd -

rm -rf $LIBAVIF_DAV1D_SRC || true
git clone -b 1.1.0 --depth 1 https://code.videolan.org/videolan/dav1d.git $LIBAVIF_DAV1D_SRC
mkdir -p $LIBAVIF_DAV1D_BUILD
cd $LIBAVIF_DAV1D_SRC

# emcmake cmake $LIBAVIF_DAV1D_SRC \
#  -DCMAKE_BUILD_TYPE=Release \
#  -DCMAKE_CXX_FLAGS="-msimd128" \
#  -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE

# emmake make -j`nproc` VERBOSE=1
meson . $LIBAVIF_DAV1D_BUILD \
  --prefix=${INSTALL_DIR} \
  --cross-file=${CROSS_FILE} \
  --default-library=static \
  --buildtype=release \
  -Dbitdepths="['8']" \
  -Denable_asm=false \
  -Denable_tools=false \
  -Denable_tests=false \
  -Dlogging=false 

ninja -C build install

cd -

rm -rf build
mkdir build
cd build/

emcmake cmake ../libavif \
  -G "Unix Makefiles" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS="-msimd128" \
  -DBUILD_SHARED_LIBS=0 \
  -DAVIF_CODEC_AOM=0 \
  -DAVIF_LOCAL_AOM=0 \
  -DAVIF_CODEC_DAV1D=1 \
  -DAVIF_LOCAL_DAV1D=1 \
  -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} 

emmake make -j`nproc` VERBOSE=1

