// MikaWorld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MikaWorld.h"
#include "MikaVM.h"

using namespace irr;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

static char const* const SScriptName = "../Asset/Quake.miko";
static char const* const SLoadedFunctionName = "Quake:Loaded";

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
		//Impacts.push_back(imp);
	}
}

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

int main()
{
	MikaWorld world;
	world.Run();
}

// Run shell command and capture output, from https://stackoverflow.com/a/35658917
std::string ExecCmd(const wchar_t* cmd)
{
	std::string strResult;
	HANDLE hPipeRead, hPipeWrite;

	SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) };
	saAttr.bInheritHandle = TRUE;   //Pipe handles are inherited by child process.
	saAttr.lpSecurityDescriptor = NULL;

	// Create a pipe to get results from child's stdout.
	if (!CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0))
		return strResult;

	STARTUPINFO si = { sizeof(STARTUPINFO) };
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.hStdOutput = hPipeWrite;
	si.hStdError = hPipeWrite;
	si.wShowWindow = SW_HIDE;       // Prevents cmd window from flashing. Requires STARTF_USESHOWWINDOW in dwFlags.

	PROCESS_INFORMATION pi = { 0 };

	wchar_t cmdBuffer[1024];
	wcscpy_s(cmdBuffer, cmd);
	BOOL fSuccess = CreateProcessW(NULL, cmdBuffer, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	if (!fSuccess)
	{
		CloseHandle(hPipeWrite);
		CloseHandle(hPipeRead);
		return strResult;
	}

	bool bProcessEnded = false;
	for (; !bProcessEnded;)
	{
		// Give some timeslice (50ms), so we won't waste 100% cpu.
		bProcessEnded = WaitForSingleObject(pi.hProcess, 50) == WAIT_OBJECT_0;

		// Even if process exited - we continue reading, if there is some data available over pipe.
		for (;;)
		{
			char buf[1024];
			DWORD dwRead = 0;
			DWORD dwAvail = 0;

			if (!::PeekNamedPipe(hPipeRead, NULL, 0, NULL, &dwAvail, NULL))
				break;

			if (!dwAvail) // no data available, return
				break;

			if (!::ReadFile(hPipeRead, buf, min(sizeof(buf) - 1, dwAvail), &dwRead, NULL) || !dwRead)
				// error, the child process might ended
				break;

			buf[dwRead] = 0;
			strResult += buf;
		}
	} //for

	CloseHandle(hPipeWrite);
	CloseHandle(hPipeRead);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return strResult;
} //ExecCmd

void LoadScript(MikaVM& vm)
{
	std::string result = ExecCmd(L"../Bin/Win/ninja.exe -f build.ninja.win");
	OutputDebugStringA(result.c_str());
	if (result.find("FAILED") == std::string::npos)
	{
		vm.Reset();
		vm.Import(SScriptName);
		vm.Execute(SLoadedFunctionName);
	}
}

void MikaWorld::Run()
{
	EventReceiver eventReceiver;

	// ask user for driver
	mVideoDevice = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(640, 480), 16, false, false, true, &eventReceiver);
	if (!mVideoDevice)
		return;

	video::IVideoDriver* driver = mVideoDevice->getVideoDriver();
	scene::ISceneManager* smgr = mVideoDevice->getSceneManager();

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

	MikaVM vm;
	LoadScript(vm);

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
				LoadScript(vm);
			}

			smgr->drawAll();

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

		eventReceiver.Update();
	}
}
