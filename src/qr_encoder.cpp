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

#include "qrmodule/base64.h"

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

        std::string str_q(res.begin(), res.end());
        std::cout << "SIZE_STR: " << str_q.size() << "\n";

        const std::string qr_base64 = utils::Base64::Encode(str_q.data(), str_q.size());
        std::cout << "SIZE: " << qr_base64.size() << "\n";

        std::ofstream fout("hash2_data.dat", std::ios::out | std::ios::binary);
        fout << qr_base64;
        fout.close();

        std::ifstream instream("hash2_data.dat", std::ios::in | std::ios::binary);
        std::string d((std::istreambuf_iterator<char>(instream)), std::istreambuf_iterator<char>());
        std::cout << "SIZE_LOAD: " << d.size() << "\n";
        instream.close();

        const std::string out_qr = utils::Base64::Decode(d);
        std::cout << "BIN: " << out_qr.size() << "\n";

        for (size_t i = 0; i < out_qr.size(); i += 4) {
            uint32_t ele = (uint8_t) out_qr[i] | (((uint8_t) out_qr[i + 1]) << 8) | (((uint8_t) out_qr[i + 2]) << 16) |
                           (((uint8_t) out_qr[i + 3]) << 24);

            float final = 0;
            std::memcpy(&final, &ele, sizeof(float));
            std::cout << final << ", ";
        }
        std::cout << "\n\n\n";


        std::wstring str;
        str.append(qr_base64.data(), qr_base64.data() + qr_base64.size());

        ZXing::QRCode::Writer wr;
        wr.setErrorCorrectionLevel(ZXing::QRCode::ErrorCorrectionLevel::High);
        wr.setMargin(4);
        wr.setVersion(40);

        return ZXing::ToMatrix<uint8_t>(wr.encode(str, 120, 120));
    }

    std::vector<uint8_t> QREncoder::encode(const float *data, size_t length) {
        auto qr_img = _encode(data, length);
        return std::vector<uint8_t>(qr_img.begin(), qr_img.end());
    }

    std::vector<uint8_t> QREncoder::encode_to_png(const float *data, size_t length) {
        auto qr = _encode(data, length);

        int final_len;
        uint8_t *png_mem = stbi_write_png_to_mem(qr.data(), 0, qr.width(), qr.height(), 1, &final_len);
        std::vector<uint8_t> final_data(png_mem, png_mem + final_len);

        delete[] png_mem;

        return final_data;
    }

    bool QREncoder::encode(const std::string &file_path, const float *data, size_t length) {
        auto qr_img = _encode(data, length);
        return stbi_write_png(file_path.c_str(), qr_img.width(), qr_img.height(), 1, qr_img.data(), 0) == 1;
    }


}
