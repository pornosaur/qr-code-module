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

        static std::vector<float> decode_str(const std::string &qr_code);
    };

}
#endif //BIOMETRIC_PALAXO_QR_DECODER_H
