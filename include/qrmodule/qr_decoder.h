//
// Created by Patrik Patera on 18/12/2020.
//

#ifndef BIOMETRIC_PALAXO_QR_DECODER_H
#define BIOMETRIC_PALAXO_QR_DECODER_H

#include <vector>
#include <cstdint>
#include <string>

namespace qr {

    class QRDecoder {
    public:
        static std::vector<float> decode(const std::string &file_path);
    };

}
#endif //BIOMETRIC_PALAXO_QR_DECODER_H
