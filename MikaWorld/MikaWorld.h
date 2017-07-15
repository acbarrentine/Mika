#pragma once

class MikaWorld
{
protected:
	irr::IrrlichtDevice* mVideoDevice;
	irr::scene::ITriangleSelector* mTriangleSelector;

	struct ParticleImpact
	{
		irr::u32 mWhen;
		irr::core::vector3df mPos;
		irr::core::vector3df mOutVector;
	};

public:
	MikaWorld()
		: mVideoDevice(nullptr)
		, mTriangleSelector(nullptr)
	{

	}

	~MikaWorld()
	{
		mTriangleSelector->drop();
		mVideoDevice->drop();
	}

	void Fire();
	void Run();
};
