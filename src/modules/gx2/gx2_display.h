#pragma once
#include "modules/coreinit/coreinit_time.h"
#include "modules/gx2/gx2_enum.h"
#include "modules/gx2/gx2_surface.h"
#include "types.h"
#include "utils/be_val.h"

struct GX2Texture;
struct GX2ColorBuffer;

void
GX2SetTVEnable(BOOL enable);

void
GX2SetDRCEnable(BOOL enable);

void
GX2CalcTVSize(GX2TVRenderMode::Value tvRenderMode,
              GX2SurfaceFormat::Value surfaceFormat,
              GX2BufferingMode::Value bufferingMode,
              be_val<uint32_t> *size,
              be_val<uint32_t> *unkOut);

void
GX2CalcDRCSize(GX2DrcRenderMode::Value drcRenderMode,
               GX2SurfaceFormat::Value surfaceFormat,
               GX2BufferingMode::Value bufferingMode,
               be_val<uint32_t> *size,
               be_val<uint32_t> *unkOut);

void
GX2SetTVBuffer(void *buffer,
               uint32_t size,
               GX2TVRenderMode::Value tvRenderMode,
               GX2SurfaceFormat::Value surfaceFormat,
               GX2BufferingMode::Value bufferingMode);

void
GX2SetDRCBuffer(void *buffer,
                uint32_t size,
                GX2DrcRenderMode::Value drcRenderMode,
                GX2SurfaceFormat::Value surfaceFormat,
                GX2BufferingMode::Value bufferingMode);

void
GX2SetTVScale(uint32_t x, uint32_t y);

void
GX2SetDRCScale(uint32_t x, uint32_t y);

GX2TVScanMode::Value
GX2GetSystemTVScanMode();

GX2DrcRenderMode::Value
GX2GetSystemDRCMode();
