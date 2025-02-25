#pragma once

namespace project_diamond
{
	enum class MovementState2D : unsigned short
	{
		NONE	= 0,
		LEFT	= 1,
		RIGHT	= 1 << 1
	};

	inline MovementState2D operator|(MovementState2D a, MovementState2D b)
	{
		return static_cast<MovementState2D>(
			static_cast<std::underlying_type_t<MovementState2D>>(a) |
			static_cast<std::underlying_type_t<MovementState2D>>(b));
	}

	inline MovementState2D& operator|=(MovementState2D& a, MovementState2D b)
	{
		a = (a | b);

		return a;
	}

	inline MovementState2D operator&(MovementState2D a, MovementState2D b)
	{
		return static_cast<MovementState2D>(
			static_cast<std::underlying_type_t<MovementState2D>>(a) &
			static_cast<std::underlying_type_t<MovementState2D>>(b));
	}

	inline MovementState2D& operator&=(MovementState2D& a, MovementState2D b)
	{
		a = (a & b);

		return a;
	}

	inline MovementState2D operator~(MovementState2D a)
	{
		return static_cast<MovementState2D>(~(static_cast<std::underlying_type_t<MovementState2D>>(a)));
	}
}
