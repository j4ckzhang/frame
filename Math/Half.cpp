#include "stdafx.h"
#include "Half.h"

#define _unsigned_neg(a) (~(a) + 1)

static inline unsigned _unsigned_sels(unsigned test, unsigned a, unsigned b)
{
	unsigned mask = ((int)test) >> 31;
	unsigned sel_a = a & mask;
	unsigned sel_b = b & (~mask);
	unsigned result = sel_a | sel_b;
	return result;
}

static inline unsigned _unsigned_cntlz(unsigned x)
{
	unsigned x0 = x >> 1l;
	unsigned x1 = x | x0;
	unsigned x2 = x1 >> 2l;
	unsigned x3 = x1 | x2;
	unsigned x4 = x3 >> 4l;
	unsigned x5 = x3 | x4;
	unsigned x6 = x5 >> 8l;
	unsigned x7 = x5 | x6;
	unsigned x8 = x7 >> 16l;
	unsigned x9 = x7 | x8;
	unsigned xA = ~x9;
	unsigned xB = xA >> 1l;
	unsigned xC = xB & 0x55555555;
	unsigned xD = xA - xC;
	unsigned xE = xD & 0x33333333;
	unsigned xF = xD >> 2l;
	unsigned x10 = xF & 0x33333333;
	unsigned x11 = xE + x10;
	unsigned x12 = x11 >> 4l;
	unsigned x13 = x11 + x12;
	unsigned x14 = x13 & 0x0f0f0f0f;
	unsigned x15 = x14 >> 8l;
	unsigned x16 = x14 + x15;
	unsigned x17 = x16 >> 16l;
	unsigned x18 = x16 + x17;
	unsigned x19 = x18 & 0x0000003f;
	return x19;
}

unsigned short FloatToHalf(float x)
{
	union
	{
		float f32;
		unsigned ui32;
	} u;

	u.f32 = x;
	unsigned f = u.ui32;

	unsigned one = 0x00000001;
	unsigned f_e_mask = 0x7f800000;
	unsigned f_m_mask = 0x007fffff;
	unsigned f_s_mask = 0x80000000;
	unsigned h_e_mask = 0x00007c00;
	unsigned f_e_pos = 0x00000017;
	unsigned f_m_round_bit = 0x00001000;
	unsigned h_nan_em_min = 0x00007c01;
	unsigned f_h_s_pos_offset = 0x00000010;
	unsigned f_m_hidden_bit = 0x00800000;
	unsigned f_h_m_pos_offset = 0x0000000d;
	unsigned f_h_bias_offset = 0x38000000;
	unsigned f_m_snan_mask = 0x003fffff;
	unsigned h_snan_mask = 0x00007e00;
	unsigned f_e = f & f_e_mask;
	unsigned f_m = f & f_m_mask;
	unsigned f_s = f & f_s_mask;
	unsigned f_e_h_bias = f_e - f_h_bias_offset;
	unsigned f_e_h_bias_amount = f_e_h_bias >> (int)f_e_pos;
	unsigned f_m_round_mask = f_m & f_m_round_bit;
	unsigned f_m_round_offset = f_m_round_mask << 1;
	unsigned f_m_rounded = f_m + f_m_round_offset;
	unsigned f_m_rounded_overflow = f_m_rounded & f_m_hidden_bit;
	unsigned f_m_denorm_sa = one - f_e_h_bias_amount;
	unsigned f_m_with_hidden = f_m_rounded | f_m_hidden_bit;
	unsigned f_m_denorm = f_m_with_hidden >> (int)f_m_denorm_sa;
	unsigned f_em_norm_packed = f_e_h_bias | f_m_rounded;
	unsigned f_e_overflow = f_e_h_bias + f_m_hidden_bit;
	unsigned h_s = f_s >> (int)f_h_s_pos_offset;
	unsigned h_m_nan = f_m >> (int)f_h_m_pos_offset;
	unsigned h_m_denorm = f_m_denorm >> (int)f_h_m_pos_offset;
	unsigned h_em_norm = f_em_norm_packed >> (int)f_h_m_pos_offset;
	unsigned h_em_overflow = f_e_overflow >> (int)f_h_m_pos_offset;
	unsigned is_e_eqz_msb = f_e - 1;
	unsigned is_m_nez_msb = _unsigned_neg(f_m);
	unsigned is_h_m_nan_nez_msb = _unsigned_neg(h_m_nan);
	unsigned is_e_nflagged_msb = f_e - f_e_mask;
	unsigned is_ninf_msb = is_e_nflagged_msb | is_m_nez_msb;
	unsigned is_underflow_msb = is_e_eqz_msb - f_h_bias_offset;
	unsigned is_nan_nunderflow_msb = is_h_m_nan_nez_msb | is_e_nflagged_msb;
	unsigned is_m_snan_msb = f_m_snan_mask - f_m;
	unsigned is_snan_msb = is_m_snan_msb & (~is_e_nflagged_msb);
	unsigned is_overflow_msb = _unsigned_neg(f_m_rounded_overflow);
	unsigned h_nan_underflow_result = _unsigned_sels(is_nan_nunderflow_msb, h_em_norm, h_nan_em_min);
	unsigned h_inf_result = _unsigned_sels(is_ninf_msb, h_nan_underflow_result, h_e_mask);
	unsigned h_underflow_result = _unsigned_sels(is_underflow_msb, h_m_denorm, h_inf_result);
	unsigned h_overflow_result = _unsigned_sels(is_overflow_msb, h_em_overflow, h_underflow_result);
	unsigned h_em_result = _unsigned_sels(is_snan_msb, h_snan_mask, h_overflow_result);
	unsigned h_result = h_em_result | h_s;
	return h_result;
}

float HalfToFloat(unsigned short h)
{
	unsigned h_e_mask = 0x00007c00;
	unsigned h_m_mask = 0x000003ff;
	unsigned h_s_mask = 0x00008000;
	unsigned h_f_s_pos_offset = 0x00000010;
	unsigned h_f_e_pos_offset = 0x0000000d;
	unsigned h_f_bias_offset = 0x0001c000;
	unsigned f_e_mask = 0x7f800000;
	unsigned f_m_mask = 0x007fffff;
	unsigned h_f_e_denorm_bias = 0x0000007e;
	unsigned h_f_m_denorm_sa_bias = 0x00000008;
	unsigned f_e_pos = 0x00000017;
	unsigned h_e_mask_minus_one = 0x00007bff;
	unsigned h_e = h & h_e_mask;
	unsigned h_m = h & h_m_mask;
	unsigned h_s = h & h_s_mask;
	unsigned h_e_f_bias = h_e + h_f_bias_offset;
	unsigned h_m_nlz = _unsigned_cntlz(h_m);
	unsigned f_s = h_s << h_f_s_pos_offset;
	unsigned f_e = h_e_f_bias << h_f_e_pos_offset;
	unsigned f_m = h_m << h_f_e_pos_offset;
	unsigned f_em = f_e | f_m;
	unsigned h_f_m_sa = h_m_nlz - h_f_m_denorm_sa_bias;
	unsigned f_e_denorm_unpacked = h_f_e_denorm_bias - h_f_m_sa;
	unsigned h_f_m = h_m << h_f_m_sa;
	unsigned f_m_denorm = h_f_m & f_m_mask;
	unsigned f_e_denorm = f_e_denorm_unpacked << f_e_pos;
	unsigned f_em_denorm = f_e_denorm | f_m_denorm;
	unsigned f_em_nan = f_e_mask | f_m;
	unsigned is_e_eqz_msb = h_e - 1;
	unsigned is_m_nez_msb = _unsigned_neg(h_m);
	unsigned is_e_flagged_msb = h_e_mask_minus_one - h_e;
	unsigned is_zero_msb = is_e_eqz_msb & (~is_m_nez_msb);
	unsigned is_inf_msb = is_e_flagged_msb & (~is_m_nez_msb);
	unsigned is_denorm_msb = is_m_nez_msb & is_e_eqz_msb;
	unsigned is_nan_msb = is_e_flagged_msb & is_m_nez_msb;
	unsigned is_zero = ((int)is_zero_msb) >> 31;
	unsigned f_zero_result = f_em & (~is_zero);
	unsigned f_denorm_result = _unsigned_sels(is_denorm_msb, f_em_denorm, f_zero_result);
	unsigned f_inf_result = _unsigned_sels(is_inf_msb, f_e_mask, f_denorm_result);
	unsigned f_nan_result = _unsigned_sels(is_nan_msb, f_em_nan, f_inf_result);
	unsigned f_result = f_s | f_nan_result;

	union
	{
		float f32;
		unsigned ui32;
	} u;

	u.ui32 = f_result;
	return u.f32;
}