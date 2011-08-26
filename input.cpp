// Global includes
#include <string.h>
#include <stdio.h>
#include <QtGui>

// Other includes
#include "input.h"

class cmInput mIn;

/// Attaches mouse and keyboard handles to the event queue
void cmInput::init(QWidget *source)
{
	// Initialize
	OIS::ParamList pl;
	pl.insert(OIS::ParamList::value_type("WINDOW", QString::number( source->effectiveWinId() ).toStdString() ) );
	inputMgr = OIS::InputManager::createInputSystem( pl);
	
	//mouse = 	static_cast<OIS::Mouse*>(inputMgr->createInputObject( OIS::OISMouse, true) );
	keyboard = 	static_cast<OIS::Keyboard*>(inputMgr->createInputObject( OIS::OISKeyboard, false) );
	//mouse	->setEventCallback(this);
	keyboard->setEventCallback(this);
	
	// Set mouse area
	/*const OIS::MouseState &mouseState = mouse->getMouseState();
	mouseState.width = source->geometry().width();
	mouseState.height = source->geometry().height();*/
	
	// Clear keybuffer
	memset(key,0,sizeof(bool)*OIS_KEYCODE_MAX);
	memset(pkey,0,sizeof(bool)*OIS_KEYCODE_MAX);
	memset(rkey,0,sizeof(bool)*OIS_KEYCODE_MAX);
	
	mouse_b = 0;
	mouse_x = mouse_y = mouse_z = 0;
	mouse_dx = mouse_dy = 0;
	m_scr = 0;
	keyrep = 0;
}

bool cmInput::keyPressed(const OIS::KeyEvent &arg)
{	printf("Test\n");
	key[arg.key] = true; pkey[arg.key] = true;
	return true;
}

bool cmInput::keyReleased(const OIS::KeyEvent &arg)
{	printf("Test\n");
	key[arg.key] = false; rkey[arg.key] = true;	
	return true;
}

bool cmInput::mouseMoved(const OIS::MouseEvent &arg)
{	const OIS::MouseState &s = arg.state;
	mouse_x = s.X.abs;
	mouse_y = s.Y.abs;
	mouse_z = s.Z.rel;
	
	mouse_dx = s.X.rel;
	mouse_dy = s.Y.rel;
	mouse_tdx = mouse_dx;
	mouse_tdy = mouse_dy;
	return true;
}

bool cmInput::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{	mouse_b |= (1<<(id));
	mouse_pb |= (1<<(id));
	return true;
}

bool cmInput::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{	mouse_b &= ~(1<<(id));
	mouse_rb |= (1<<(id));
	return true;
}

void cmInput::frame_reset(void)
{
	for(int k=0;k<OIS_KEYCODE_MAX;k++) {
		pkey[k] = 0;
		rkey[k] = 0;
	}
	
	mouse_pb = 0;
	mouse_rb = 0;
	mouse_dx = mouse_dy = 0;
	mouse_z = 0;
}

void cmInput::update(void)
{
	//mouse->capture();
	keyboard->capture();
	
	// Empty pressed/released mouse buttons
	keyrep = 0;
}

void cmInput::shutdown(void)
{
	inputMgr->destroyInputObject(mouse);
	inputMgr->destroyInputObject(keyboard);
	inputMgr->destroyInputSystem( inputMgr );
}
