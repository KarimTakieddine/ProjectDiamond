#pragma once

namespace project_diamond
{
	enum class CollisionState2D : unsigned short
	{
		NONE	= 0,
		GROUND	= 1
	};

	inline CollisionState2D operator|(CollisionState2D a, CollisionState2D b)
	{
		return static_cast<CollisionState2D>(
			static_cast<std::underlying_type_t<CollisionState2D>>(a) |
			static_cast<std::underlying_type_t<CollisionState2D>>(b));
	}

	inline CollisionState2D& operator|=(CollisionState2D& a, CollisionState2D b)
	{
		a = (a | b);

		return a;
	}

	inline CollisionState2D operator&(CollisionState2D a, CollisionState2D b)
	{
		return static_cast<CollisionState2D>(
			static_cast<std::underlying_type_t<CollisionState2D>>(a) &
			static_cast<std::underlying_type_t<CollisionState2D>>(b));
	}

	inline CollisionState2D& operator&=(CollisionState2D& a, CollisionState2D b)
	{
		a = (a & b);

		return a;
	}

	inline CollisionState2D operator~(CollisionState2D a)
	{
		return static_cast<CollisionState2D>(~(static_cast<std::underlying_type_t<CollisionState2D>>(a)));
	}
}
