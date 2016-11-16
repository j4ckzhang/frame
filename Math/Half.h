#pragma once

FRAME_API unsigned short FloatToHalf(float x);
FRAME_API float HalfToFloat(unsigned short h);

class Half
{
public:
	Half();
	Half(float value);

	inline float ToFloat() const;
	inline operator float() const;

	inline bool operator==(const Half& other) const;
	inline bool operator!=(const Half& other) const;

	inline Half operator+(const Half& other) const;
	inline Half operator-(const Half& other) const;
	inline Half operator*(const Half& other) const;
	inline Half operator/(const Half& other) const;

	inline bool IsNan() const;
	inline bool IsFinite() const;
	inline bool IsInfinite() const;
	inline bool IsNormal() const;

private:
	inline int exponent() const;
	inline int mantissa() const;

	unsigned short value_;
};

inline Half::Half()
{
}

inline Half::Half(float value)
	: value_(FloatToHalf(value))
{
}

inline float Half::ToFloat() const
{
	return HalfToFloat(value_);
}

inline Half::operator float() const
{
	return ToFloat();
}

inline bool Half::operator==(const Half& other) const
{
	return value_ == other.value_;
}

inline bool Half::operator!=(const Half& other) const
{
	return value_ != other.value_;
}

inline Half Half::operator+(const Half& other) const
{
	return Half(ToFloat() + other.ToFloat());
}

inline Half Half::operator-(const Half& other) const
{
	return Half(ToFloat() - other.ToFloat());
}

inline Half Half::operator*(const Half& other) const
{
	return Half(ToFloat() * other.ToFloat());
}

inline Half Half::operator/(const Half& other) const
{
	return Half(ToFloat() / other.ToFloat());
}

inline bool Half::IsNan() const
{
	return (exponent() == 31) && (mantissa() != 0);
}

inline bool Half::IsFinite() const
{
	return (exponent() != 31);
}

inline bool Half::IsInfinite() const
{
	return (exponent() == 31) && (mantissa() == 0);
}

inline bool Half::IsNormal() const
{
	return IsFinite() && ((exponent() != 0) || (mantissa() == 0));
}

inline int Half::exponent() const
{
	return (value_ & 0x7C00) >> 10;
}

inline int Half::mantissa() const
{
	return value_ & 0x03ff;
}

