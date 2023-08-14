#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_GLTF_MATERIAL_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_GLTF_MATERIAL_H_

#include <memory>
#include <string>
#include <string_view>

#include "ExternalDependencies/DirectX11/DirectX11Texture.h"

struct Material {

    bool Load(std::string_view file_path) {
        return true;
    }

    std::string name;
    std::shared_ptr<DirectX11Texture> spBaseColorTexture;
    std::shared_ptr<DirectX11Texture> spMetallicRoughnessTexture;
    std::shared_ptr<DirectX11Texture> spOcclusionTexture;
    std::shared_ptr<DirectX11Texture> spEmissiveTexture;
    std::shared_ptr<DirectX11Texture> spNormalTexture;
};

#endif
