//
// Created by Patrik Patera on 18/12/2020.
//

#include <memory>

#include "qrmodule/qr_decoder.h"
#include "qrmodule/image_view.h"

#include <HybridBinarizer.h>
#include <LuminanceSource.h>
#include <GenericLuminanceSource.h>
#include <DecodeHints.h>
#include <qrcode/QRDecoder.h>
#include <qrcode/QRReader.h>
#include <iostream>

#include <qrcode/QRDetector.h>
#include <DetectorResult.h>
#include <BitMatrixIO.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb/stb_image.h"


namespace qr {

    std::vector<float> QRDecoder::decode(const std::string &file_path) {
        int width, height, channels;
        std::unique_ptr<stbi_uc, void (*)(void *)> buffer(
                stbi_load(file_path.c_str(), &width, &height, &channels, 4), stbi_image_free);
        if (buffer == nullptr) return {};

        ZXing::DecodeHints hints;
        hints.setIsPure(false);
        hints.setTryHarder(true);
        hints.setTryRotate(true);
        hints.setFormats(ZXing::BarcodeFormat::QRCode);
        hints.setCharacterSet("ASCII");

        ImageView imgview({buffer.get(), width, height, ImageFormat::RGBX});
        ZXing::GenericLuminanceSource source(0, 0, width, height, buffer.get(), imgview.row_stride(),
                                             imgview.pix_stride(), imgview.red_index(), imgview.green_index(),
                                             imgview.blue_index(), nullptr);
        auto lum_src = std::shared_ptr<ZXing::LuminanceSource>(&source, [](void *) {});

        std::unique_ptr<ZXing::BinaryBitmap> imgbin = std::make_unique<ZXing::HybridBinarizer>(lum_src);

        ZXing::QRCode::Reader reader(hints);
        auto res = reader.decode(*imgbin);
        size_t max = res.text().size();

        /*  ONLY DETECTION
        auto detectorResult = ZXing::QRCode::Detect(*imgbin->getBlackMatrix(), true, false);
        ZXing::SaveAsPBM(detectorResult.bits(), "tttmenu.bpm");
        */

        std::vector<float> data;
        for (size_t i = 0; i < max; i += 4) {
            uint32_t ele = res.text()[i] | (res.text()[i + 1] << 8) | (res.text()[i + 2] << 16) |
                           (res.text()[i + 3] << 24);

            float final = 0;
            std::memcpy(&final, &ele, sizeof(float));
            std::cout << final << ", ";
            data.push_back(final);
        }

        return data;
    }

}