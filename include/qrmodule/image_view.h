//
// Created by Patrik Patera on 18/12/2020.
//

#ifndef BIOMETRIC_PALAXO_IMAGE_VIEW_H
#define BIOMETRIC_PALAXO_IMAGE_VIEW_H

#include <cstdint>

#include <Result.h>
#include <DecodeHints.h>
#include <BinaryBitmap.h>
#include <BitMatrix.h>


namespace qr {
    enum class ImageFormat : uint32_t {
        None = 0,
        Lum = 0x01000000,
        RGB = 0x03000102,
        BGR = 0x03020100,
        RGBX = 0x04000102,
        XRGB = 0x04010203,
        BGRX = 0x04020100,
        XBGR = 0x04030201,
    };

    constexpr inline int PixStride(ImageFormat format) { return (static_cast<uint32_t>(format) >> 3 * 8) & 0xFF; }

    constexpr inline int RedIndex(ImageFormat format) { return (static_cast<uint32_t>(format) >> 2 * 8) & 0xFF; }

    constexpr inline int GreenIndex(ImageFormat format) { return (static_cast<uint32_t>(format) >> 1 * 8) & 0xFF; }

    constexpr inline int BlueIndex(ImageFormat format) { return (static_cast<uint32_t>(format) >> 0 * 8) & 0xFF; }

    class ImageView {
    protected:
        const uint8_t *_data = nullptr;
        ImageFormat _format;
        int _width = 0, _height = 0, _pixStride = 0, _rowStride = 0, _redIndex = 0, _greenIndex = 0, _blueIndex = 0;

    public:
        /**
         * ImageView contructor
         *
         * @param data  pointer to image buffer
         * @param width  image width in pixels
         * @param height  image height in pixels
         * @param format  image/pixel format
         * @param rowStride  optional row stride in bytes, default is width * pixStride
         * @param pixStride  optional pixel stride in bytes, default is calculated from format
         */
        ImageView(const uint8_t *data, int width, int height, ImageFormat format, int rowStride = 0, int pixStride = 0)
                : _data(data), _format(format), _width(width), _height(height),
                  _pixStride(pixStride ? pixStride : PixStride(format)),
                  _rowStride(rowStride ? rowStride : width * _pixStride),
                  _redIndex(RedIndex(format)), _greenIndex(GreenIndex(format)), _blueIndex(BlueIndex(format)) {}

        [[nodiscard]] int pix_stride() const { return _pixStride; }

        [[nodiscard]] int row_stride() const { return _rowStride; }

        [[nodiscard]] int red_index() const { return _redIndex; }

        [[nodiscard]] int blue_index() const { return _blueIndex; }

        [[nodiscard]] int green_index() const { return _greenIndex; }

        [[nodiscard]] const uint8_t *data(int x, int y) const { return _data + y * _rowStride + x * _pixStride; }
    };

}
#endif //BIOMETRIC_PALAXO_IMAGE_VIEW_H
