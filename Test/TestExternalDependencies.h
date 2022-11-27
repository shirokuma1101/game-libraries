#pragma once

#include "ExternalDependencies/Asset/IAsset/IAssetData.h"
#include "ExternalDependencies/Asset/IAsset/IAssetManager.h"
GAME_LIBRARIES_EXTERNALDEPENDENCIES_ASSET_IASSET_IASSETDATA_H_
GAME_LIBRARIES_EXTERNALDEPENDENCIES_ASSET_IASSET_IASSETMANAGER_H_
#include "ExternalDependencies/Asset/Json/JsonData.h"
#include "ExternalDependencies/Asset/Json/JsonManager.h"
GAME_LIBRARIES_EXTERNALDEPENDENCIES_ASSET_JSON_JSONDATA_H_
GAME_LIBRARIES_EXTERNALDEPENDENCIES_ASSET_JSON_JSONMANAGER_H_
#include "ExternalDependencies/Audio/AudioManager.h"
#include "ExternalDependencies/Audio/AudioHelper.h"
GAME_LIBRARIES_EXTERNALDEPENDENCIES_AUDIO_AUDIOMANAGER_H_
GAME_LIBRARIES_EXTERNALDEPENDENCIES_AUDIO_AUDIOHELPER_H_
#include "ExternalDependencies/Effekseer/EffekseerHelper.h"
#include "ExternalDependencies/Effekseer/EffekseerManager.h"
GAME_LIBRARIES_EXTERNALDEPENDENCIES_EFFEKSEER_EFFEKSEERHELPER_H_
GAME_LIBRARIES_EXTERNALDEPENDENCIES_EFFEKSEER_EFFEKSEERMANAGER_H_
#include "ExternalDependencies/Input/InputHelper.h"
#include "ExternalDependencies/Input/InputManager.h"
#include "ExternalDependencies/Input/TextBox.h"
GAME_LIBRARIES_EXTERNALDEPENDENCIES_INPUT_INPUTHELPER_H_
GAME_LIBRARIES_EXTERNALDEPENDENCIES_INPUT_INPUTMANAGER_H_
GAME_LIBRARIES_EXTERNALDEPENDENCIES_INPUT_TEXTBOX_H_
#include "ExternalDependencies/Math/Camera.h"
#include "ExternalDependencies/Math/Collider.h"
#include "ExternalDependencies/Math/Collision.h"
#include "ExternalDependencies/Math/Transform.h"
GAME_LIBRARIES_EXTERNALDEPENDENCIES_MATH_CAMERA_H_
GAME_LIBRARIES_EXTERNALDEPENDENCIES_MATH_COLLIDER_H_
GAME_LIBRARIES_EXTERNALDEPENDENCIES_MATH_COLLISION_H_
GAME_LIBRARIES_EXTERNALDEPENDENCIES_MATH_TRANSFORM_H_
#include "ExternalDependencies/DirectX11/DirectX11Helper.h"
#include "ExternalDependencies/DirectX11/DirectX11.h"
#include "ExternalDependencies/DirectX11/DirectX11Buffer.h"
#include "ExternalDependencies/DirectX11/DirectX11Texture.h"
GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11_H_
GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11BUFFER_H_
GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11TEXTURE_H_
#include "ExternalDependencies/PhysX/PhysXHelper.h"
#include "ExternalDependencies/PhysX/PhysXManager.h"
GAME_LIBRARIES_EXTERNALDEPENDENCIES_PHYSX_PHYSXHELPER_H_
GAME_LIBRARIES_EXTERNALDEPENDENCIES_PHYSX_PHYSXMANAGER_H_
//#include "ExternalDependencies/Socket/SocketHelper.h"
//GAME_LIBRARIES_EXTERNALDEPENDENCIES_SOCKET_SOCKETHELPER_H_
#include "ExternalDependencies/Utility/DateTime.h"
#include "ExternalDependencies/Utility/String.h"
GAME_LIBRARIES_EXTERNALDEPENDENCIES_UTILITY_DATETIME_H_
GAME_LIBRARIES_EXTERNALDEPENDENCIES_UTILITY_STRING_H_

class TEST_EXTERNALDEPENDENCIES
{
public:

    static void TEST_JSONDATA() {
        
    }

    static void TEST_JSONMANAGER() {

    }

    static void TEST_AUDIOHELPER() {

    }

    static void TEST_AUDIOMANAGER() {
        //AudioManager audio_manager;
        //audio_manager.Init();
        //audio_manager.SetAudio("test", "D:/OneDrive/SystemFolderForSync/Music/VOCALOID/VOCALOID/マジカルミライ/wav/マジカルミライ2013_1.wav");
        //audio_manager.Play("test");
        //audio_manager.GetSoundInstance("test")->GetSoundEffectInstance()->SetPitch(0.5f);
        //while (true) {
        //    audio_manager.Update();
        //}
    }

    static void TEST_DIRECTX11() {
        
    }

    static void TEST_DIRECTX11BUFFER() {

    }

    static void TEST_DIRECTX11HELPER() {

    }

    static void TEST_DIRECTX11TEXTURES() {

    }
    
    static void TEST_EFFEKSEERHELPER() {

    }

    static void TEST_EFFEKSEERMANAGER() {

    }

    static void TEST_IMGUIHELPER() {

    }

    static void TEST_INPUTHELPER() {

    }

    static void TEST_INPUTMANAGER() {

    }

    static void TEST_TEXTBOX() {
        //TextBox tb(9);
        //std::string before_str;
        //std::string after_str;
        //while (true) {
        //    std::system("cls");
        //    before_str = tb.GetText();
        //    tb.Update();
        //    std::cout << tb.GetCursor() << ":" << tb.GetText() << std::endl;
        //}
    }

    static void TEST_CAMERA() {

    }

    static void TEST_COLLIDER() {
        //enum class ColTy {
        //    Bump = 0 << 1,
        //    Attack = 0 << 2
        //};
        //Collider<uint64_t> a;
        //Collider<ColTy> a;
        //DirectX::BoundingSphere sphere;
        //std::list<collision::Result> list;
        //a.Intersects(0, DirectX::SimpleMath::Matrix::Identity, sphere, &list);
        //a.Intersects(ColTy::Bump, DirectX::SimpleMath::Matrix::Identity, sphere, &list);
        //collision::GetNearest(list);
    }

    static void TEST_COLLISION() {

    }

    static void TEST_TRANSFORM() {

    }

    static void TEST_PHYSXHELPER() {

    }

    static void TEST_PHYSXMANAGER() {

    }

    static void TEST_SOCKETHELPER() {
        //SOCKET sock = socket_helper::Create();
        //ADDRINFO addr_info;
        //socket_helper::SetNonBlocking(&sock);
        //socket_helper::SetBlocking(&sock);
        //socket_helper::GetAddrInfo("192.168.1.1", 80, &addr_info);
        //std::cout << socket_helper::GetIPAddr(addr_info) << std::endl;
        //socket_helper::GetAddrInfo("google.com", 80, &addr_info);
        //std::cout << socket_helper::GetIPAddr(addr_info) << std::endl;
        //socket_helper::GetAddrInfo("192.168.1.17", 8080, &addr_info);
        //std::cout << socket_helper::GetIPAddr(addr_info) << std::endl;
        //socket_helper::Bind(&sock, addr_info);
        //socket_helper::Listen(&sock, 10);
        //ShellExecute(0, 0, L"http://192.168.1.17:8080", 0, 0, SW_SHOW);
        //while (true) {
        //    ADDRINFO client_addr_info;
        //    SOCKET client_sock = socket_helper::Accept(&sock, &client_addr_info);
        //    std::cout << socket_helper::GetIPAddr(client_addr_info) << std::endl;
        //    std::cout << socket_helper::Recv(client_sock) << std::endl;
        //    std::string http_response =
        //        "HTTP/1.1 200 OK\r\n"
        //        "Content-Length: 20\r\n"
        //        "Content-Type: text/html\r\n"
        //        "\r\n"
        //        "HELLO\r\n";
        //    socket_helper::Send(client_sock, http_response);
        //    socket_helper::Close(&client_sock, false);
        //}
        //socket_helper::Close(&sock);
    }

    static void TEST_DELTATIME() {

    }

    static void TEST_STRING() {

    }
    
};
