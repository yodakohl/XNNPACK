// Auto-generated file. Do not edit!
//   Template: src/f32-qs8-vcvt/neon.c.in
//   Generator: tools/xngen
//
// Copyright 2021 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <arm_neon.h>

#include <xnnpack/common.h>
#include <xnnpack/intrinsics-polyfill.h>
#include <xnnpack/vcvt.h>


void xnn_f32_qu8_vcvt_ukernel__neon_x16(
    size_t n,
    const float* x,
    uint8_t* y,
    const union xnn_f32_qu8_cvt_params params[restrict XNN_MIN_ELEMENTS(1)]) XNN_DISABLE_TSAN
{
  assert(n != 0);
  assert(x != NULL);
  assert(y != NULL);

  const float32x4_t vscale = vld1q_dup_f32(&params->neon.scale);
  const float32x4_t vmagic_bias = vld1q_dup_f32(&params->neon.magic_bias);
  const int32x4_t vmagic_min = vld1q_dup_s32(&params->neon.magic_min);
  const int32x4_t vmagic_bias_less_zero_point = vld1q_dup_s32(&params->neon.magic_bias_less_zero_point);
  const uint8x16_t voutput_max = vld1q_dup_u8(&params->neon.output_max);
  for (; n >= 16 * sizeof(uint8_t); n -= 16 * sizeof(uint8_t)) {
    float32x4_t vx0123 = vld1q_f32(x); x += 4;
    float32x4_t vx4567 = vld1q_f32(x); x += 4;
    float32x4_t vx89AB = vld1q_f32(x); x += 4;
    float32x4_t vxCDEF = vld1q_f32(x); x += 4;

    vx0123 = vmulq_f32(vx0123, vscale);
    vx4567 = vmulq_f32(vx4567, vscale);
    vx89AB = vmulq_f32(vx89AB, vscale);
    vxCDEF = vmulq_f32(vxCDEF, vscale);

    vx0123 = vaddq_f32(vx0123, vmagic_bias);
    vx4567 = vaddq_f32(vx4567, vmagic_bias);
    vx89AB = vaddq_f32(vx89AB, vmagic_bias);
    vxCDEF = vaddq_f32(vxCDEF, vmagic_bias);

    int32x4_t vacc0123 = vmaxq_s32(vreinterpretq_s32_f32(vx0123), vmagic_min);
    int32x4_t vacc4567 = vmaxq_s32(vreinterpretq_s32_f32(vx4567), vmagic_min);
    int32x4_t vacc89AB = vmaxq_s32(vreinterpretq_s32_f32(vx89AB), vmagic_min);
    int32x4_t vaccCDEF = vmaxq_s32(vreinterpretq_s32_f32(vxCDEF), vmagic_min);

    vacc0123 = vsubq_s32(vacc0123, vmagic_bias_less_zero_point);
    vacc4567 = vsubq_s32(vacc4567, vmagic_bias_less_zero_point);
    vacc89AB = vsubq_s32(vacc89AB, vmagic_bias_less_zero_point);
    vaccCDEF = vsubq_s32(vaccCDEF, vmagic_bias_less_zero_point);

    const int16x8_t vacc01234567 = vcombine_s16(vqmovn_s32(vacc0123), vqmovn_s32(vacc4567));
    const int16x8_t vacc89ABCDEF = vcombine_s16(vqmovn_s32(vacc89AB), vqmovn_s32(vaccCDEF));

    uint8x16_t vy0123456789ABCDEF = vcombine_u8(vqmovun_s16(vacc01234567), vqmovun_s16(vacc89ABCDEF));

    vy0123456789ABCDEF = vminq_u8(vy0123456789ABCDEF, voutput_max);

    vst1q_u8(y, vy0123456789ABCDEF); y += 16;
  }
  for (; n >= 8 * sizeof(uint8_t); n -= 8 * sizeof(uint8_t)) {
    float32x4_t vx_lo = vld1q_f32(x); x += 4;
    float32x4_t vx_hi = vld1q_f32(x); x += 4;

    vx_lo = vmulq_f32(vx_lo, vscale);
    vx_hi = vmulq_f32(vx_hi, vscale);

    vx_lo = vaddq_f32(vx_lo, vmagic_bias);
    vx_hi = vaddq_f32(vx_hi, vmagic_bias);

    int32x4_t vacc_lo = vmaxq_s32(vreinterpretq_s32_f32(vx_lo), vmagic_min);
    int32x4_t vacc_hi = vmaxq_s32(vreinterpretq_s32_f32(vx_hi), vmagic_min);

    vacc_lo = vsubq_s32(vacc_lo, vmagic_bias_less_zero_point);
    vacc_hi = vsubq_s32(vacc_hi, vmagic_bias_less_zero_point);

    const int16x8_t vacc = vcombine_s16(vqmovn_s32(vacc_lo), vqmovn_s32(vacc_hi));

    uint8x8_t vy = vqmovun_s16(vacc);
    vy = vmin_u8(vy, vget_low_u8(voutput_max));
    vst1_u8(y, vy); y += 8;
  }
  if XNN_UNLIKELY(n != 0) {
    assert(n >= 1 * sizeof(uint8_t));
    assert(n <= 7 * sizeof(uint8_t));
    float32x4_t vx_lo = vld1q_f32(x);
    const float* x_hi = x + 4;
    if XNN_UNPREDICTABLE((n & (4 * sizeof(uint8_t))) == 0) {
      x_hi = x;
    }
    float32x4_t vx_hi = vld1q_f32(x_hi);

    vx_lo = vmulq_f32(vx_lo, vscale);
    vx_hi = vmulq_f32(vx_hi, vscale);

    vx_lo = vaddq_f32(vx_lo, vmagic_bias);
    vx_hi = vaddq_f32(vx_hi, vmagic_bias);

    int32x4_t vacc_lo = vmaxq_s32(vreinterpretq_s32_f32(vx_lo), vmagic_min);
    int32x4_t vacc_hi = vmaxq_s32(vreinterpretq_s32_f32(vx_hi), vmagic_min);

    vacc_lo = vsubq_s32(vacc_lo, vmagic_bias_less_zero_point);
    vacc_hi = vsubq_s32(vacc_hi, vmagic_bias_less_zero_point);

    const int16x8_t vacc = vcombine_s16(vqmovn_s32(vacc_lo), vqmovn_s32(vacc_hi));

    uint8x8_t vy = vqmovun_s16(vacc);
    vy = vmin_u8(vy, vget_low_u8(voutput_max));

    if (n & (4 * sizeof(uint8_t))) {
      vst1_lane_u32((void*) y, vreinterpret_u32_u8(vy), 0); y += 4;
      vy = vext_u8(vy, vy, 4);
    }
    if (n & (2 * sizeof(uint8_t))) {
      vst1_lane_u16((void*) y, vreinterpret_u16_u8(vy), 0); y += 2;
      vy = vext_u8(vy, vy, 2);
    }
    if (n & (1 * sizeof(uint8_t))) {
      vst1_lane_u8(y, vy, 0);
    }
  }
}
