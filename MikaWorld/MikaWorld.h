#pragma once

class V3 : public irr::core::vector3df
{
public:
	V3() : irr::core::vector3df() {}
	V3(irr::core::vector3df& vec) : irr::core::vector3df(vec) {}
	V3(float x, float y, float z) : irr::core::vector3df(x, y, z) {}
	V3(V3* other) : irr::core::vector3df(*other) {}

	void Assign(V3* other) { *this = *other; }
	void Add(V3* other) { *this += *other; }
	void Subtract(V3* other) { *this -= *other; }
	void Multiply(V3* other) { *this *= *other; }
	void Divide(V3* other) { *this /= *other; }
	void Scale(float scalar) { *this *= scalar; }

	int Equals(V3* other) { return *this == *other; }
	int NotEquals(V3* other) { return *this != *other; }
	float GetLength() const { return getLength(); }
	float GetLengthSquared() const { return getLengthSQ(); }
	float DotProduct(V3* target) const { return dotProduct(*target); }
	V3* CrossProduct(V3* target) const
	{
		irr::core::vector3df cross = crossProduct(*target);
		V3* retval = new V3(cross.X, cross.Y, cross.Z);
		return retval;
	}
	void Normalize() { normalize(); }
};

struct ParticleImpact
{
	irr::u32 mWhen;
	V3 mPos;
	V3 mOutVector;

	V3* GetImpactPos() { return &mPos; }
	V3* GetOutVector() { return &mOutVector; }
};

class MikaWorld
{
protected:
	irr::IrrlichtDevice* mVideoDevice;
	irr::scene::ITriangleSelector* mTriangleSelector;
	irr::gui::IGUIFont* mFont;

	MikaVM mVM;

	struct DebugMessage
	{
	public:
		irr::core::stringw Str;
		irr::core::dimension2d<irr::u32> Size;
		int Duration;

	public:
		DebugMessage(const irr::core::stringw& string, irr::core::dimension2d<irr::u32>& size) : Str(string), Size(size), Duration(400) {}
		DebugMessage(const DebugMessage& other) : Str(other.Str), Size(other.Size), Duration(other.Duration) {}
		DebugMessage& operator=(const DebugMessage& other)
		{
			if (this != &other)
			{
				Str = other.Str;
				Size = other.Size;
				Duration = other.Duration;
			}
			return *this;
		}
		DebugMessage& operator=(const DebugMessage&& other)
		{
			if (this != &other)
			{
				Str = std::move(other.Str);
				Size = std::move(other.Size);
				Duration = std::move(Duration);
			}
			return *this;
		}
	};
	irr::core::array<DebugMessage> mScriptMessages;
	irr::u32 mDebugTextLength;

public:
	const static irr::u32 WindowWidth = 1024;
	const static irr::u32 WindowHeight = 768;

	MikaWorld()
		: mVideoDevice(nullptr)
		, mTriangleSelector(nullptr)
		, mDebugTextLength(0)
	{

	}

	~MikaWorld()
	{
		mTriangleSelector->drop();
		mVideoDevice->drop();
	}

	void Fire();
	void Run();

	void LoadScript();
	void AddScriptMessage(const irr::core::stringw& msg);
	void UpdateScriptMessages();
	void Execute(const char* function);
};

extern MikaWorld GWorld;
