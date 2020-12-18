//
// Created by Patrik Patera on 18/12/2020.
//

#include "qrmodule/qr_encoder.h"

#include <qrcode/QRWriter.h>
#include <qrcode/QRErrorCorrectionLevel.h>
#include <BitMatrix.h>

#include <vector>
#include <cstring>
#include <CharacterSet.h>
#include <fstream>
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb/stb_image_write.h"

namespace qr {

    ZXing::Matrix<uint8_t> QREncoder::_encode(const float *data, size_t length) {
        std::vector<uint8_t> res;
        for (size_t i = 0; i < length; ++i) {
            float num = data[i];

            uint32_t num_i = 0;
            std::memcpy(&num_i, &num, sizeof(num_i));

            res.push_back((num_i) & 0xFF);
            res.push_back((num_i >> 8) & 0xFF);
            res.push_back((num_i >> 16) & 0xFF);
            res.push_back((num_i >> 24) & 0xFF);
        }

        std::ofstream fout("hash1_data.dat", std::ios::out | std::ios::binary);
        fout.write((char *)res.data(), res.size() * sizeof(res.front()));
        fout.close();

        std::ifstream instream("hash1_data.dat", std::ios::in | std::ios::binary);
        std::vector<uint8_t> d((std::istreambuf_iterator<char>(instream)), std::istreambuf_iterator<char>());
        std::cout << "BIN: \n";
        for (auto a : d) {
            std::cout << (int)a << "; ";
        }
        std::cout << "\n\n\n";


        std::wstring str;
        str.append(res.data(), res.data() + res.size());

        ZXing::QRCode::Writer wr;
        wr.setErrorCorrectionLevel(ZXing::QRCode::ErrorCorrectionLevel::High);
        wr.setMargin(4);
        wr.setVersion(34);

        return ZXing::ToMatrix<uint8_t>(wr.encode(str, 120, 120));
    }

    std::vector<uint8_t> QREncoder::encode(const float *data, size_t length) {
        auto qr_img = _encode(data, length);
        return std::vector<uint8_t>(qr_img.begin(), qr_img.end());
    }

    bool QREncoder::encode(const std::string &file_path, const float *data, size_t length) {
        auto qr_img = _encode(data, length);
        return stbi_write_png(file_path.c_str(), qr_img.width(), qr_img.height(), 1, qr_img.data(), 0) == 1;
    }

}