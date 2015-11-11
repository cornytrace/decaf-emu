#include "gx2.h"
#include "gx2_contextstate.h"
#include "gx2_display.h"
#include "gx2_displaylist.h"
#include "gx2_draw.h"
#include "gx2_event.h"
#include "gx2_mem.h"
#include "gx2_registers.h"
#include "gx2_sampler.h"
#include "gx2_shaders.h"
#include "gx2_state.h"
#include "gx2_surface.h"
#include "gx2_swap.h"
#include "gx2_temp.h"
#include "gx2_texture.h"
#include "gx2r_resource.h"

GX2::GX2()
{
}

void GX2::initialise()
{
   initialiseVsync();
}

void
GX2::RegisterFunctions()
{
   registerContextStateFunctions();
   registerDisplayFunctions();
   registerDisplayListFunctions();
   registerDrawFunctions();
   registerEventFunctions();
   registerResourceFunctions();
   registerMemFunctions();
   registerRegisterFunctions();
   registerSamplerFunctions();
   registerShaderFunctions();
   registerStateFunctions();
   registerSurfaceFunctions();
   registerSwapFunctions();
   registerTempFunctions();
   registerTextureFunctions();
}

void
GX2::registerContextStateFunctions()
{
   RegisterKernelFunction(GX2SetupContextState);
   RegisterKernelFunction(GX2SetupContextStateEx);
   RegisterKernelFunction(GX2GetContextStateDisplayList);
   RegisterKernelFunction(GX2SetContextState);
}

void
GX2::registerDisplayFunctions()
{
   RegisterKernelFunction(GX2SetTVEnable);
   RegisterKernelFunction(GX2SetDRCEnable);
   RegisterKernelFunction(GX2CalcTVSize);
   RegisterKernelFunction(GX2SetTVBuffer);
   RegisterKernelFunction(GX2SetTVScale);
   RegisterKernelFunction(GX2CalcDRCSize);
   RegisterKernelFunction(GX2SetDRCBuffer);
   RegisterKernelFunction(GX2SetDRCScale);
   RegisterKernelFunction(GX2GetSystemTVScanMode);
   RegisterKernelFunction(GX2GetSystemDRCMode);
}

void
GX2::registerDisplayListFunctions()
{
   RegisterKernelFunction(GX2BeginDisplayListEx);
   RegisterKernelFunction(GX2BeginDisplayList);
   RegisterKernelFunction(GX2EndDisplayList);
   RegisterKernelFunction(GX2DirectCallDisplayList);
   RegisterKernelFunction(GX2CallDisplayList);
   RegisterKernelFunction(GX2GetDisplayListWriteStatus);
   RegisterKernelFunction(GX2GetCurrentDisplayList);
   RegisterKernelFunction(GX2CopyDisplayList);
}

void
GX2::registerDrawFunctions()
{
   RegisterKernelFunction(GX2ClearBuffersEx);
   RegisterKernelFunction(GX2ClearColor);
   RegisterKernelFunction(GX2ClearDepthStencilEx);
   RegisterKernelFunction(GX2SetAttribBuffer);
   RegisterKernelFunction(GX2SetClearDepthStencil);
   RegisterKernelFunction(GX2SetPrimitiveRestartIndex);
   RegisterKernelFunction(GX2DrawEx);
   RegisterKernelFunction(GX2DrawIndexedEx);
}

void
GX2::registerEventFunctions()
{
   RegisterKernelFunction(GX2DrawDone);
   RegisterKernelFunction(GX2WaitForVsync);
   RegisterKernelFunction(GX2WaitForFlip);
   RegisterKernelFunction(GX2SetEventCallback);
   RegisterKernelFunction(GX2GetEventCallback);
   RegisterKernelFunction(GX2GetRetiredTimeStamp);
   RegisterKernelFunction(GX2GetLastSubmittedTimeStamp);
   RegisterKernelFunction(GX2WaitTimeStamp);
   RegisterKernelFunctionName("VsyncAlarmHandler", gx2::internal::vsyncAlarmHandler);
}

void
GX2::registerMemFunctions()
{
   RegisterKernelFunction(GX2Invalidate);
}

void
GX2::registerRegisterFunctions()
{
   RegisterKernelFunction(GX2InitBlendControlReg);
   RegisterKernelFunction(GX2InitDepthStencilControlReg);
   RegisterKernelFunction(GX2SetBlendControl);
   RegisterKernelFunction(GX2SetBlendControlReg);
   RegisterKernelFunction(GX2SetDepthStencilControl);
   RegisterKernelFunction(GX2SetDepthStencilControlReg);
}

void
GX2::registerSamplerFunctions()
{
   RegisterKernelFunction(GX2InitSampler);
   RegisterKernelFunction(GX2InitSamplerBorderType);
   RegisterKernelFunction(GX2InitSamplerClamping);
   RegisterKernelFunction(GX2InitSamplerDepthCompare);
   RegisterKernelFunction(GX2InitSamplerFilterAdjust);
   RegisterKernelFunction(GX2InitSamplerLOD);
   RegisterKernelFunction(GX2InitSamplerLODAdjust);
   RegisterKernelFunction(GX2InitSamplerRoundingMode);
   RegisterKernelFunction(GX2InitSamplerXYFilter);
   RegisterKernelFunction(GX2InitSamplerZMFilter);
}

void
GX2::registerShaderFunctions()
{
   RegisterKernelFunction(GX2CalcGeometryShaderInputRingBufferSize);
   RegisterKernelFunction(GX2CalcGeometryShaderOutputRingBufferSize);
   RegisterKernelFunction(GX2CalcFetchShaderSizeEx);
   RegisterKernelFunction(GX2InitFetchShaderEx);
   RegisterKernelFunction(GX2SetFetchShader);
   RegisterKernelFunction(GX2SetVertexShader);
   RegisterKernelFunction(GX2SetPixelShader);
   RegisterKernelFunction(GX2SetGeometryShader);
   RegisterKernelFunction(GX2SetPixelSampler);
   RegisterKernelFunction(GX2SetVertexUniformReg);
   RegisterKernelFunction(GX2SetPixelUniformReg);
   RegisterKernelFunction(GX2SetVertexUniformBlock);
   RegisterKernelFunction(GX2SetPixelUniformBlock);
   RegisterKernelFunction(GX2SetShaderModeEx);
   RegisterKernelFunction(GX2GetPixelShaderGPRs);
   RegisterKernelFunction(GX2GetPixelShaderStackEntries);
   RegisterKernelFunction(GX2GetVertexShaderGPRs);
   RegisterKernelFunction(GX2GetVertexShaderStackEntries);
}

void
GX2::registerStateFunctions()
{
   RegisterKernelFunction(GX2Init);
   RegisterKernelFunction(GX2Shutdown);
   RegisterKernelFunction(GX2Flush);
   RegisterKernelFunction(GX2GetLastSubmittedTimeStamp);
   RegisterKernelFunction(GX2GetRetiredTimeStamp);
}

void
GX2::registerSurfaceFunctions()
{
   RegisterKernelFunction(GX2CalcSurfaceSizeAndAlignment);
   RegisterKernelFunction(GX2CalcDepthBufferHiZInfo);
   RegisterKernelFunction(GX2SetColorBuffer);
   RegisterKernelFunction(GX2SetDepthBuffer);
   RegisterKernelFunction(GX2InitColorBufferRegs);
   RegisterKernelFunction(GX2InitDepthBufferRegs);
}

void
GX2::registerSwapFunctions()
{
   RegisterKernelFunction(GX2CopyColorBufferToScanBuffer);
   RegisterKernelFunction(GX2SwapBuffers);
   RegisterKernelFunction(GX2SwapScanBuffers);
   RegisterKernelFunction(GX2GetLastFrame);
   RegisterKernelFunction(GX2GetLastFrameGamma);
   RegisterKernelFunction(GX2GetSwapStatus);
   RegisterKernelFunction(GX2GetSwapInterval);
   RegisterKernelFunction(GX2SetSwapInterval);
}

void
GX2::registerTempFunctions()
{
   RegisterKernelFunction(GX2TempGetGPUVersion);
}

void
GX2::registerTextureFunctions()
{
   RegisterKernelFunction(GX2InitTextureRegs);
   RegisterKernelFunction(GX2SetPixelTexture);
}

void
GX2::registerResourceFunctions()
{
   RegisterKernelFunction(GX2RSetAllocator);
}
