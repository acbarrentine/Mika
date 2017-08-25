// MikaWorld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../MikaVM/MikaVM.h"
#include "../Compiler/Compiler.h"
#include "MikaWorld.h"

using namespace irr;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

static char const* const SScriptName = "../Asset/Quake.miko";
static char const* const SLoadedFunctionName = "Quake:Loaded";

MikaWorld GWorld;


class ScriptCallbackAnimator : public irr::scene::ISceneNodeAnimator
{
protected:
	irr::u32 mTimeToRun;
	irr::core::stringc mCallback;
	bool mDone;

public:
	ScriptCallbackAnimator(irr::u32 time, irr::core::stringc cb)
		: irr::scene::ISceneNodeAnimator()
		, mTimeToRun(time)
		, mCallback(cb)
		, mDone(false)
	{
	}

	virtual void animateNode(irr::scene::ISceneNode* node, u32 timeMs)
	{
		if (!mDone && timeMs > mTimeToRun)
		{
			GWorld.Execute(mCallback.c_str());
			mDone = true;
		}
	}

	virtual ISceneNodeAnimator* createClone(irr::scene::ISceneNode* node, irr::scene::ISceneManager* newManager = nullptr)
	{
		return new ScriptCallbackAnimator(mTimeToRun, mCallback);
	}

	virtual bool hasFinished() const
	{
		return mDone;
	}
};

enum
{
	// I use this ISceneNode ID to indicate a scene node that is
	// not pickable by getSceneNodeAndCollisionPointFromRay()
	ID_IsNotPickable = 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be picked by ray selection.
	IDFlag_IsPickable = 1 << 0,
};

class EventReceiver : public IEventReceiver
{
public:
	enum Bits
	{
		None = 0,
		LeftMouse = 1,
		KeyEscape = 1 << 1,
		KeyR = 1 << 2,
	};

protected:
	int mCurrent;
	int mPrevious;

public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == irr::EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown)
		{
			switch (event.KeyInput.Key)
			{
				case KEY_ESCAPE:
					mCurrent |= Bits::KeyEscape;
					break;
					
				case KEY_KEY_R:
					mCurrent |= Bits::KeyR;
					break;
			}
		}
		else if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
			if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
			{
				mCurrent |= Bits::LeftMouse;
			}
		}

		return false;
	}

	void Update()
	{
		mPrevious = mCurrent;
		mCurrent = Bits::None;
	}

	bool CheckSwitchEdge(Bits bits) const
	{
		int masked = (mCurrent & bits);
		return masked != 0 && masked != (mPrevious & bits);
	}

	bool CheckEscape() const
	{
		return CheckSwitchEdge(Bits::KeyEscape);
	}

	bool CheckLeftMouse() const
	{
		return CheckSwitchEdge(Bits::LeftMouse);
	}

	bool CheckReload() const
	{
		return CheckSwitchEdge(Bits::KeyR);
	}

	EventReceiver()
		: mCurrent(Bits::None)
		, mPrevious(Bits::None)
	{
	}
};

void MikaWorld::Fire()
{
	scene::ISceneManager* sm = mVideoDevice->getSceneManager();
	scene::ICameraSceneNode* camera = sm->getActiveCamera();

	if (!camera || !mTriangleSelector)
		return;

	ParticleImpact imp;
	imp.mWhen = 0;

	// get line of camera

	core::vector3df start = camera->getPosition();
	core::vector3df end = (camera->getTarget() - start);
	end.normalize();
	start += end*8.0f;
	end = start + (end * camera->getFarValue());

	core::triangle3df triangle;

	core::line3d<f32> line(start, end);

	// get intersection point with map
	scene::ISceneNode* hitNode;
	core::vector3df collisionPoint(end);
	if (sm->getSceneCollisionManager()->getCollisionPoint(
		line, mTriangleSelector, collisionPoint, triangle, hitNode))
	{
		// collides with wall
		core::vector3df out = triangle.getNormal();
		out.setLength(0.03f);

		imp.mWhen = 1;
		imp.mOutVector = out;
		imp.mPos = collisionPoint;
		end = collisionPoint;
	}

	// create fire ball
	scene::ISceneNode* node = 0;
	node = sm->addBillboardSceneNode(0,
		core::dimension2d<f32>(25, 25), start);

	node->setMaterialFlag(video::EMF_LIGHTING, false);
	node->setMaterialTexture(0, mVideoDevice->getVideoDriver()->getTexture("../Asset/fireball.bmp"));
	node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

	f32 length = (f32)(end - start).getLength();
	const f32 speed = 0.6f;
	u32 time = (u32)(length / speed);
	
	scene::ISceneNodeAnimator* anim = nullptr;

	// set flight line

	anim = sm->createFlyStraightAnimator(start, end, time);
	node->addAnimator(anim);
	anim->drop();

	// delete the billboard on impact
	anim = sm->createDeleteAnimator(time);
	node->addAnimator(anim);
	anim->drop();

	if (imp.mWhen)
	{
		// create impact note
		imp.mWhen = mVideoDevice->getTimer()->getTime() + (time - 100);
		//mImpacts.push_back(imp);

		ScriptCallbackAnimator* anim = new ScriptCallbackAnimator(imp.mWhen, "Quake:Hit");
		node->addAnimator(anim);
		anim->drop();
	}
}

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

int main()
{
	GWorld.Run();
}

void MikaWorld::LoadScript()
{
	// Optionally, we could either skip writing the .miko file to disk, or
	// timestamp check it and skip compilation if the output is newer than
	// the sources. For now, just taking the easy way out.

	std::string glueHeader = "MikaGlue.mikah";
	std::string sourceFile = "Quake.mika";
	std::string objectPath = "../Asset/Quake.miko";
	std::string debugPath = "../Asset/Quake.mikd";

	GCompiler.Reset();
	if (GCompiler.GetErrorCount() == 0) GCompiler.ReadGlueHeader(glueHeader.c_str());
	if (GCompiler.GetErrorCount() == 0) GCompiler.ParseGlueHeader();
	if (GCompiler.GetErrorCount() == 0) GCompiler.ReadScript(sourceFile.c_str());
	if (GCompiler.GetErrorCount() == 0) GCompiler.ParseScript();
	if (GCompiler.GetErrorCount() == 0) GCompiler.AnalyzeScript();
	if (GCompiler.GetErrorCount() == 0) GCompiler.WriteObjectFile(objectPath.c_str(), debugPath.c_str());

	mVM.Import(objectPath.c_str());

	if (GCompiler.GetErrorCount() == 0)
	{
		mVM.Reset();
		mVM.Import(objectPath.c_str());
		mVM.Execute(SLoadedFunctionName);
	}
}

void MikaWorld::AddScriptMessage(const irr::core::stringw& msg)
{
	core::dimension2d<u32> size = mFont->getDimension(msg.c_str());
	mScriptMessages.push_back(DebugMessage(msg, size));
	mVideoDevice->getLogger()->log(msg.c_str());

	mDebugTextLength += size.Height;
	while (mDebugTextLength > WindowHeight)
	{
		DebugMessage& msg = mScriptMessages[0];
		mDebugTextLength -= msg.Size.Height;
		mScriptMessages.erase(0);
	}
}

void MikaWorld::UpdateScriptMessages()
{
	u32 firstKeeper = 0;
	for (u32 i = 0; i < mScriptMessages.size(); ++i)
	{
		DebugMessage& msg = mScriptMessages[i];
		--msg.Duration;
		if (msg.Duration == 0)
		{
			++firstKeeper;
			mDebugTextLength -= msg.Size.Height;
		}
	}

	if (firstKeeper > 0)
	{
		mScriptMessages.erase(0, firstKeeper);
	}
}

void MikaWorld::Execute(const char* function)
{
	mVM.Execute(function);
}

void MikaWorld::Run()
{
	EventReceiver eventReceiver;

	// ask user for driver
	mVideoDevice = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(WindowWidth, WindowHeight), 16, false, false, true, &eventReceiver);
	if (!mVideoDevice)
		return;

	video::IVideoDriver* driver = mVideoDevice->getVideoDriver();
	scene::ISceneManager* smgr = mVideoDevice->getSceneManager();
	gui::IGUIEnvironment* gui = mVideoDevice->getGUIEnvironment();
	mFont = gui->getFont("../Asset/fontcourier.bmp");

	mVideoDevice->getFileSystem()->addFileArchive("../Asset/map-20kdm2.pk3");
	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	scene::IMeshSceneNode* node = nullptr;

	if (!mesh)
		return;

	node = smgr->addOctreeSceneNode(mesh->getMesh(0), nullptr, IDFlag_IsPickable);
	if (!node)
		return;

	SKeyMap cameraKeys[] = {
		SKeyMap(EKA_MOVE_FORWARD, KEY_KEY_W),
		SKeyMap(EKA_MOVE_BACKWARD, KEY_KEY_S),
		SKeyMap(EKA_STRAFE_LEFT, KEY_KEY_A),
		SKeyMap(EKA_STRAFE_RIGHT, KEY_KEY_D),
	};
	
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(nullptr, 100.0f, 0.3f, ID_IsNotPickable, cameraKeys, ARRAY_SIZE(cameraKeys), true, 0.3f);
	if (!camera)
		return;

	camera->setPosition(core::vector3df(50, 50, -60));
	camera->setTarget(core::vector3df(-70, 30, -60));

	node->setPosition(core::vector3df(-1350, -130, -1400));

	mTriangleSelector = smgr->createOctreeTriangleSelector(node->getMesh(), node, 128);
	if (!mTriangleSelector)
		return;

	node->setTriangleSelector(mTriangleSelector);
	core::vector3df ellipsoidRadius(30.0f, 50.0f, 30.0f);
	core::vector3df gravity(0, -10.0f, 0);
	core::vector3df ellipsoidTranslation(0.0f, 30.0f, 0.0f);
	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
		mTriangleSelector, camera, ellipsoidRadius, gravity, ellipsoidTranslation);
	camera->addAnimator(anim);
	anim->drop();  // And likewise, drop the animator when we're done referring to it.

	// disable mouse cursor
	mVideoDevice->getCursorControl()->setVisible(false);

	s32 lastFPS = -1;

	LoadScript();

	while (mVideoDevice->run())
	{
		if (eventReceiver.CheckEscape())
			break;

		if (mVideoDevice->isWindowActive())
		{
			driver->beginScene(true, true, 0);

			if (eventReceiver.CheckLeftMouse())
			{
				mVideoDevice->getLogger()->log("Fire!");
				Fire();
			}
			
			if (eventReceiver.CheckReload())
			{
				LoadScript();
			}

			smgr->drawAll();

			gui->drawAll();
			s32 line = 20;
			for (u32 i = 0; i < mScriptMessages.size(); ++i)
			{
				DebugMessage& msg = mScriptMessages[i];
				mFont->draw(msg.Str, core::rect<s32>(20, line, (20 + msg.Size.Width), (line + msg.Size.Height)), video::SColor(255, 255, 255, 255));
				line += msg.Size.Height;
			}

			driver->endScene();

			const s32 fps = driver->getFPS();

			if (lastFPS != fps)
			{
				core::stringw str = L"MikaWorld [";
				str += fps;
				str += " FPS]";

				mVideoDevice->setWindowCaption(str.c_str());
				lastFPS = fps;
			}
		}

		UpdateScriptMessages();

		eventReceiver.Update();
	}
}
