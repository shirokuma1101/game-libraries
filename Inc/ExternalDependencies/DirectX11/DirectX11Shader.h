#pragma once

#ifndef GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11SHADER_H_
#define GAME_LIBRARIES_EXTERNALDEPENDENCIES_DIRECTX11_DIRECTX11SHADER_H_

#include <string_view>

#include "Utility/Assert.h"
#include "Utility/Memory.h"
#include "DirectX11Helper.h"

template<class T>
class DirectX11Shader
{
public:

    DirectX11Shader(ID3D11Device* dev, ID3D11DeviceContext* ctx)
        : m_pDev(dev)
        , m_pCtx(ctx)
    {}
    virtual ~DirectX11Shader() {
        Release();
    }

    T* Get() noexcept {
        return m_pShader;
    }
    T* Get() const noexcept {
        return m_pShader;
    }

    virtual bool Compile(std::string_view file_path, std::string_view entry_point, bool enable_debug) = 0;

    virtual bool Create(const void* shader_bytecode, SIZE_T bytecode_length) = 0;
    virtual bool Create(ID3DBlob* blob) final {
        return Create(blob->GetBufferPointer(), blob->GetBufferSize());
    }

    virtual void Set() = 0;

protected:

    void Release() {
        memory::SafeRelease(&m_pShader);
    }

    ID3D11Device*        m_pDev    = nullptr;
    ID3D11DeviceContext* m_pCtx    = nullptr;
    ID3DBlob*            m_pBlob   = nullptr;
    T*                   m_pShader = nullptr;

};

class DirectX11ComputeShader : public DirectX11Shader<ID3D11ComputeShader>
{
public:

    DirectX11ComputeShader(ID3D11Device* dev, ID3D11DeviceContext* ctx)
        : DirectX11Shader(dev, ctx)
    {}

    bool Compile(std::string_view file_path, std::string_view entry_point, bool enable_debug) override {
        if (m_pBlob = directx11_helper::CompileShaderFromFile(file_path, entry_point, directx11_helper::ShaderTarget::CS, enable_debug), m_pBlob) {
            return true;
        }
        return false;
    }

    bool Create(const void* shader_bytecode, SIZE_T bytecode_length) override {
        if (FAILED(m_pDev->CreateComputeShader(shader_bytecode, bytecode_length, nullptr, &m_pShader))) {
            assert::ShowError(ASSERT_FILE_LINE, "Create compute shader failed");
            return false;
        }
        return true;
    }

    void Set() override {
        m_pCtx->CSSetShader(m_pShader, nullptr, 0);
    }

};

class DirectX11DomainShader : public DirectX11Shader<ID3D11DomainShader>
{
public:

    DirectX11DomainShader(ID3D11Device* dev, ID3D11DeviceContext* ctx)
        : DirectX11Shader(dev, ctx)
    {}

    bool Compile(std::string_view file_path, std::string_view entry_point, bool enable_debug) override {
        if (m_pBlob = directx11_helper::CompileShaderFromFile(file_path, entry_point, directx11_helper::ShaderTarget::DS, enable_debug), m_pBlob) {
            return true;
        }
        return false;
    }

    bool Create(const void* shader_bytecode, SIZE_T bytecode_length) override {
        if (FAILED(m_pDev->CreateDomainShader(shader_bytecode, bytecode_length, nullptr, &m_pShader))) {
            assert::ShowError(ASSERT_FILE_LINE, "Create domain shader failed");
            return false;
        }
        return true;
    }

    void Set() override {
        m_pCtx->DSSetShader(m_pShader, nullptr, 0);
    }

};

class DirectX11GeometryShader : public DirectX11Shader<ID3D11GeometryShader>
{
public:

    DirectX11GeometryShader(ID3D11Device* dev, ID3D11DeviceContext* ctx)
        : DirectX11Shader(dev, ctx)
    {}

    bool Compile(std::string_view file_path, std::string_view entry_point, bool enable_debug) override {
        if (m_pBlob = directx11_helper::CompileShaderFromFile(file_path, entry_point, directx11_helper::ShaderTarget::GS, enable_debug), m_pBlob) {
            return true;
        }
        return false;
    }

    bool Create(const void* shader_bytecode, SIZE_T bytecode_length) override {
        if (FAILED(m_pDev->CreateGeometryShader(shader_bytecode, bytecode_length, nullptr, &m_pShader))) {
            assert::ShowError(ASSERT_FILE_LINE, "Create geometry shader failed");
            return false;
        }
        return true;
    }

    void Set() override {
        m_pCtx->GSSetShader(m_pShader, nullptr, 0);
    }

};

class DirectX11HullShader : public DirectX11Shader<ID3D11HullShader>
{
public:

    DirectX11HullShader(ID3D11Device* dev, ID3D11DeviceContext* ctx)
        : DirectX11Shader(dev, ctx)
    {}

    bool Compile(std::string_view file_path, std::string_view entry_point, bool enable_debug) override {
        if (m_pBlob = directx11_helper::CompileShaderFromFile(file_path, entry_point, directx11_helper::ShaderTarget::HS, enable_debug), m_pBlob) {
            return true;
        }
        return false;
    }

    bool Create(const void* shader_bytecode, SIZE_T bytecode_length) override {
        if (FAILED(m_pDev->CreateHullShader(shader_bytecode, bytecode_length, nullptr, &m_pShader))) {
            assert::ShowError(ASSERT_FILE_LINE, "Create hull shader failed");
            return false;
        }
        return true;
    }

    void Set() override {
        m_pCtx->HSSetShader(m_pShader, nullptr, 0);
    }

};

class DirectX11PixelShader : public DirectX11Shader<ID3D11PixelShader>
{
public:

    DirectX11PixelShader(ID3D11Device* dev, ID3D11DeviceContext* ctx)
        : DirectX11Shader(dev, ctx)
    {}

    bool Compile(std::string_view file_path, std::string_view entry_point, bool enable_debug) override {
        if (m_pBlob = directx11_helper::CompileShaderFromFile(file_path, entry_point, directx11_helper::ShaderTarget::PS, enable_debug), m_pBlob) {
            return true;
        }
        return false;
    }

    bool Create(const void* shader_bytecode, SIZE_T bytecode_length) override {
        if (FAILED(m_pDev->CreatePixelShader(shader_bytecode, bytecode_length, nullptr, &m_pShader))) {
            assert::ShowError(ASSERT_FILE_LINE, "Create pixel shader failed");
            return false;
        }
        return true;
    }

    void Set() override {
        m_pCtx->PSSetShader(m_pShader, nullptr, 0);
    }

};

class DirectX11VertexShader : public DirectX11Shader<ID3D11VertexShader>
{
public:

    DirectX11VertexShader(ID3D11Device* dev, ID3D11DeviceContext* ctx)
        : DirectX11Shader(dev, ctx)
    {}

    bool Compile(std::string_view file_path, std::string_view entry_point, bool enable_debug) override {
        if (m_pBlob = directx11_helper::CompileShaderFromFile(file_path, entry_point, directx11_helper::ShaderTarget::VS, enable_debug), m_pBlob) {
            return true;
        }
        return false;
    }

    bool Create(const void* shader_bytecode, SIZE_T bytecode_length) override {
        if (FAILED(m_pDev->CreateVertexShader(shader_bytecode, bytecode_length, nullptr, &m_pShader))) {
            assert::ShowError(ASSERT_FILE_LINE, "Create vertex shader failed");
            return false;
        }
        return true;
    }

    void Set() override {
        m_pCtx->VSSetShader(m_pShader, nullptr, 0);
    }

};

#endif
