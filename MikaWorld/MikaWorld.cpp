// MikaWorld.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace irr;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

enum
{
	// I use this ISceneNode ID to indicate a scene node that is
	// not pickable by getSceneNodeAndCollisionPointFromRay()
	ID_IsNotPickable = 0,

	// I use this flag in ISceneNode IDs to indicate that the
	// scene node can be picked by ray selection.
	IDFlag_IsPickable = 1 << 0,
};

class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}

	MyEventReceiver()
		: KeyIsDown{}
	{
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

int main()
{
	MyEventReceiver eventReceiver;

	// ask user for driver
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(640, 480), 16, false, false, true, &eventReceiver);
	if (!device)
		return -1;

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	device->getFileSystem()->addFileArchive("../Asset/map-20kdm2.pk3");
	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	scene::IMeshSceneNode* node = nullptr;

	if (mesh)
		node = smgr->addOctreeSceneNode(mesh->getMesh(0), nullptr, IDFlag_IsPickable);

	SKeyMap cameraKeys[] = {
		SKeyMap(EKA_MOVE_FORWARD, KEY_KEY_W),
		SKeyMap(EKA_MOVE_BACKWARD, KEY_KEY_S),
		SKeyMap(EKA_STRAFE_LEFT, KEY_KEY_A),
		SKeyMap(EKA_STRAFE_RIGHT, KEY_KEY_D),
	};
	
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(nullptr, 100.0f, 0.3f, ID_IsNotPickable, cameraKeys, ARRAY_SIZE(cameraKeys), true, 0.3f);
	camera->setPosition(core::vector3df(50, 50, -60));
	camera->setTarget(core::vector3df(-70, 30, -60));

	if (node)
	{
		node->setPosition(core::vector3df(-1350, -130, -1400));

		scene::ITriangleSelector* selector = nullptr;

		if (node)
		{
			selector = smgr->createOctreeTriangleSelector(node->getMesh(), node, 128);
			node->setTriangleSelector(selector);

			if (selector)
			{
				core::vector3df ellipsoidRadius(30.0f, 50.0f, 30.0f);
				core::vector3df gravity(0, -10.0f, 0);
				core::vector3df ellipsoidTranslation(0.0f, 30.0f, 0.0f);
				scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
					selector, camera, ellipsoidRadius, gravity, ellipsoidTranslation);
				selector->drop(); // As soon as we're done with the selector, drop it.
				camera->addAnimator(anim);
				anim->drop();  // And likewise, drop the animator when we're done referring to it.
			}
		}
	}

	// disable mouse cursor
	device->getCursorControl()->setVisible(false);

	s32 lastFPS = -1;

	while (device->run())
	{
		if (eventReceiver.IsKeyDown(irr::KEY_ESCAPE))
			break;

		if (device->isWindowActive())
		{
			driver->beginScene(true, true, 0);

			smgr->drawAll();

			driver->endScene();

			const s32 fps = driver->getFPS();

			if (lastFPS != fps)
			{
				core::stringw str = L"MikaWorld [";
				str += fps;
				str += " FPS]";

				device->setWindowCaption(str.c_str());
				lastFPS = fps;
			}
		}
	}

	device->drop();

	return 0;
}