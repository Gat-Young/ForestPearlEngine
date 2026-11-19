// Standalone GPU integration test: link with the Water root .cpp files and the engine.
// Run from the usual Debug working directory; writes verification PNGs to that directory.
#include "ForestPearlEngine/ForestPearlEngine.h"
#include "ForestPearlEngine/FPGameInstance.h"
#include "ForestPearlEngine/FPAssetManager.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/FPSkyLightRenderList.h"
#include "ForestPearlEngine/Renderers/Renderer.h"
#include "ForestPearlEngine/Renderers/RenderingDevice.h"
#include "ForestPearlEngine/Libraries/DirectXTK/Inc/ScreenGrab.h"
#include "../AWater.h"
#include <d3d11sdklayers.h>
#include <wincodec.h>
#include <wrl/client.h>
#include <vector>
#include <cassert>
#include <iostream>
#include <filesystem>
#ifdef NDEBUG
#error Build WaterGpuTests with assertions enabled and the engine debug layer.
#endif
using Microsoft::WRL::ComPtr;
static void Render(Renderer& r) {
 r.CreateCamList(); r.CreatePostProcessList(); r.CreateLightRenderList(); r.CreateSkyLightRenderList();
 r.CreateMeshRenderList(); r.CreateSceneCaptureList(); r.ClearDepthStencil();
 r.PreDepthStencilRenderPass(); r.SceneCaptureRenderPass(); r.MeshRenderPass();
}
static std::vector<unsigned char> Read(const char* name, const wchar_t* filename=nullptr) {
 auto& device=RenderingDevice::GetRenderingDevice();
 auto* assets=static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
 auto* source=static_cast<ID3D11Texture2D*>(assets->GetRenderTargetData(name).ResolveTexture);
 D3D11_TEXTURE2D_DESC desc{}; source->GetDesc(&desc);
 assert(desc.Format==DXGI_FORMAT_R8G8B8A8_UNORM);
 if(filename) assert(SUCCEEDED(DirectX::SaveWICTextureToFile(device.GetDXDeviceContext(),source,GUID_ContainerFormatPng,filename)));
 desc.BindFlags=0;desc.MiscFlags=0;desc.Usage=D3D11_USAGE_STAGING;desc.CPUAccessFlags=D3D11_CPU_ACCESS_READ;
 ComPtr<ID3D11Texture2D> staging;
 assert(SUCCEEDED(device.GetDXDevice()->CreateTexture2D(&desc,nullptr,staging.GetAddressOf())));
 device.GetDXDeviceContext()->CopyResource(staging.Get(),source);
 D3D11_MAPPED_SUBRESOURCE mapped{};
 assert(SUCCEEDED(device.GetDXDeviceContext()->Map(staging.Get(),0,D3D11_MAP_READ,0,&mapped)));
 std::vector<unsigned char> pixels(desc.Width*desc.Height*4);
 for(unsigned y=0;y<desc.Height;y++) memcpy(pixels.data()+y*desc.Width*4,static_cast<unsigned char*>(mapped.pData)+y*mapped.RowPitch,desc.Width*4);
 device.GetDXDeviceContext()->Unmap(staging.Get(),0); return pixels;
}
static size_t Changed(const std::vector<unsigned char>& a,const std::vector<unsigned char>& b) {
 size_t count=0;for(size_t i=0;i<a.size();i+=4)if(a[i]!=b[i]||a[i+1]!=b[i+1]||a[i+2]!=b[i+2])++count;return count;
}
int main() {
 std::cout.setf(std::ios::unitbuf);
 auto& engine=ForestPearlEngine::GetGameEngine();assert(engine.PreInitialize());assert(engine.Initialize());
 ULONG_PTR comToken=0;assert(SUCCEEDED(CoGetContextToken(&comToken)));
 auto& game=FPGameInstance::Get(); auto* timer=static_cast<FPGameTimer*>(game.GetGameTimer());timer->Reset();game.Tick();
 auto* assets=static_cast<FPAssetManager*>(game.GetAssetManager());
 auto* skyLights=static_cast<FPSkyLightRenderList*>(game.GetSkyLightRenderList());
 assert(skyLights->GetRenderList().size()==1);
 assert(*skyLights->GetRenderList()[0].CubeMap=="rustig_koppie_puresky_2k");
 assert(assets->HasTextureData("rustig_koppie_puresky_2k"));
 assert(assets->HasTextureInfoData("WaterBump"));
 const auto& bumpInfo=assets->GetTextureInfoData("WaterBump");
 assert(bumpInfo.Path=="Water/WaterBumps1.png" && bumpInfo.Type=="WIC" && !bumpInfo.CubeMap);
 auto* bumpView=static_cast<ID3D11ShaderResourceView*>(assets->GetTextureData("WaterBump").ShaderResourceView);
 assert(bumpView);
 D3D11_SHADER_RESOURCE_VIEW_DESC bumpViewDesc{};bumpView->GetDesc(&bumpViewDesc);
 // WIC can preserve either RGBA or BGRA channel storage; both must remain linear.
 assert(bumpViewDesc.Format==DXGI_FORMAT_R8G8B8A8_UNORM || bumpViewDesc.Format==DXGI_FORMAT_B8G8R8A8_UNORM);
 ComPtr<ID3D11Resource> bumpResource;bumpView->GetResource(bumpResource.GetAddressOf());
 ComPtr<ID3D11Texture2D> bumpTexture;assert(SUCCEEDED(bumpResource.As(&bumpTexture)));
 D3D11_TEXTURE2D_DESC bumpDesc{};bumpTexture->GetDesc(&bumpDesc);
 assert(bumpDesc.Width==256 && bumpDesc.Height==256);
 std::cout<<"PASS: JSON-loaded bump PNG is 256x256, linear 8-bit channels (DXGI format="<<bumpViewDesc.Format<<")\n";
 AWater* water=nullptr;for(auto* a:game.GetWorld()->GetGameActorList())if(auto* w=dynamic_cast<AWater*>(a))water=w;
 assert(water);Renderer renderer(RenderingDevice::GetRenderingDevice());
 ComPtr<ID3D11InfoQueue> queue;RenderingDevice::GetRenderingDevice().GetDXDevice()->QueryInterface(IID_PPV_ARGS(queue.GetAddressOf()));
 assert(queue);queue->ClearStoredMessages();
 water->Settings.DistortionStrength=0;water->Settings.FlowSpeed=0;water->Tick();Render(renderer);
 auto flat=Read("MeshRenderPass-RT-BuiltIn",L"flat.png");auto reflection=Read("WaterReflection",L"reflection.png");
 size_t modelPixels=0;for(size_t i=3;i<reflection.size();i+=4)if(reflection[i])++modelPixels;assert(modelPixels>1000);
 water->Settings.DistortionStrength=.8f;water->Tick();Render(renderer);auto bump=Read("MeshRenderPass-RT-BuiltIn",L"water.png");
 assert(Changed(flat,bump)>500);
 water->Settings.FlowSpeed=.2f;Sleep(300);timer->Tick();water->Tick();Render(renderer);auto flowing=Read("MeshRenderPass-RT-BuiltIn",L"flowing.png");
 assert(Changed(bump,flowing)>500);
 water->Settings.FlowSpeed=0;Sleep(100);timer->Tick();water->Tick();Render(renderer);auto paused=Read("MeshRenderPass-RT-BuiltIn");assert(Changed(flowing,paused)==0);
 water->Settings.ReflectionAmount=0;water->Tick();Render(renderer);auto solid=Read("MeshRenderPass-RT-BuiltIn",L"solid.png");assert(Changed(paused,solid)>1000);
 // Isolate the environment: model-reflection contribution is zero and model transforms stay fixed.
 water->Settings.DistortionStrength=0;water->Tick();Render(renderer);auto envFlat=Read("MeshRenderPass-RT-BuiltIn",L"environment-flat.png");
 water->Settings.DistortionStrength=.8f;water->Tick();Render(renderer);auto envBump=Read("MeshRenderPass-RT-BuiltIn",L"environment-water.png");
 size_t envDistortion=Changed(envFlat,envBump);assert(envDistortion>10000);
 water->Settings.FlowSpeed=.2f;Sleep(300);timer->Tick();water->Tick();Render(renderer);auto envFlow=Read("MeshRenderPass-RT-BuiltIn");
 size_t envFlowChanges=Changed(envBump,envFlow);assert(envFlowChanges>10000);
 water->Settings.FlowSpeed=0;water->Tick();Render(renderer);auto envPause=Read("MeshRenderPass-RT-BuiltIn");assert(Changed(envFlow,envPause)==0);
 water->Settings.EnvironmentIntensity=0;water->Tick();Render(renderer);auto envOff=Read("MeshRenderPass-RT-BuiltIn",L"environment-off.png");
 size_t envContribution=Changed(envPause,envOff);assert(envContribution>10000);
 // Restore the initial appearance for the preview artifact.
 water->Settings.EnvironmentIntensity=.75f;water->Settings.ReflectionAmount=.8f;water->Tick();Render(renderer);
 Read("MeshRenderPass-RT-BuiltIn",L"water.png");
 std::cout<<"PASS: environment-only distortion="<<envDistortion<<", flow="<<envFlowChanges<<", intensity contribution="<<envContribution<<" pixels\n";
 size_t errors=0;if(queue)for(UINT64 i=0;i<queue->GetNumStoredMessages();i++){
 SIZE_T size=0;queue->GetMessage(i,nullptr,&size);std::vector<char> bytes(size);auto* message=reinterpret_cast<D3D11_MESSAGE*>(bytes.data());queue->GetMessage(i,message,&size);
 if(message->Severity<=D3D11_MESSAGE_SEVERITY_ERROR){++errors;std::cerr<<message->pDescription<<'\n';}}
 assert(errors==0);
 std::cout<<"PASS: model pixels in reflection="<<modelPixels<<", distortion changes="<<Changed(flat,bump)<<", flow changes="<<Changed(bump,flowing)<<", paused changes="<<Changed(flowing,paused)<<", zero-reflection changes="<<Changed(paused,solid)<<", D3D errors="<<errors<<'\n';
 return 0;
}
