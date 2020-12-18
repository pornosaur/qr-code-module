//
// Created by Patrik Patera on 18/12/2020.
//

#ifndef BIOMETRIC_PALAXO_QR_ENCODER_H
#define BIOMETRIC_PALAXO_QR_ENCODER_H

#include <vector>
#include <cstdint>
#include <string>

#include <BitMatrix.h>

namespace qr {

    class QREncoder {
    private:
        static ZXing::Matrix<uint8_t> _encode(const float *data, size_t length);

    public:

        static std::vector<uint8_t> encode(const float *data, size_t length);

        static bool encode(const std::string &file_path, const float *data, size_t length);
    };

}

#endif //BIOMETRIC_PALAXO_QR_ENCODER_H
