#ifndef GOD_CAM_H
#define GOD_CAM_H

#include "Camera.h"

void GodCam(Camera& mCam, const float camSpeed, const float rotSpeed, bool& keyPressed)
{
	// TRANSLATION >>>>>
	if (GetKeyState('W') & 0x08000)
	{
		mCam.TranslateFwdBack(camSpeed);
		keyPressed = true;
	}
	else if (GetKeyState('S') & 0x08000)
	{
		mCam.TranslateFwdBack(-camSpeed);
		keyPressed = true;
	}

	if (GetKeyState('A') & 0x08000)
	{
		mCam.TranslateLeftRight(-camSpeed);
		keyPressed = true;
	}
	else if (GetKeyState('D') & 0x08000)
	{
		mCam.TranslateLeftRight(camSpeed);
		keyPressed = true;
	}

	// ROTATION >>>>>
	if (GetKeyState(VK_LEFT) & 0x08000)
	{
		mCam.TurnLeftRight(rotSpeed);
		keyPressed = true;
	}
	else if (GetKeyState(VK_RIGHT) & 0x08000)
	{
		mCam.TurnLeftRight(-rotSpeed);
		keyPressed = true;
	}

	if (GetKeyState(VK_UP) & 0x08000)
	{
		mCam.TiltUpDown(rotSpeed);
		keyPressed = true;
	}
	else if (GetKeyState(VK_DOWN) & 0x08000)
	{
		mCam.TiltUpDown(-rotSpeed);
		keyPressed = true;
	}

	mCam.updateCamera();
}

#endif GOD_CAM_H