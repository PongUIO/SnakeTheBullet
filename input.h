#ifndef INPUT_H
#define INPUT_H
#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>
#include <OIS/OISEvents.h>

#include <QWidget>
#include <QString>

#define OIS_KEYCODE_MAX OIS::KC_MEDIASELECT

/// Stores pressed/released keys/mousebuttons and updates them on request
class cmInput : public OIS::KeyListener, public OIS::MouseListener {
	public:
		cmInput() {}
		~cmInput() {}
		
		void init(QWidget *source);
		void shutdown();
		void update(void);
		void frame_reset(void);
		
		bool getPKey(OIS::KeyCode code) { return pkey[code]; }
		bool getRKey(OIS::KeyCode code) { return rkey[code]; }
		bool getKey(OIS::KeyCode code)	{ return key[code]; }
		
		int get_mouse_tdx() { int tmp=mouse_tdx; mouse_tdx=0; return tmp; }
		int get_mouse_tdy() { int tmp=mouse_tdy; mouse_tdy=0; return tmp; }
		
		/// Is set when a mouse button is pressed
		int mouse_pb;
		
		/// Is set when a mouse button is released
		int mouse_rb;
		
		/// The relative position (compared to previous poll) of the scroll wheel
		int m_scr;
		
		/// @name Portability stuff
		//@{
			int mouse_x,mouse_y;
			int mouse_dx, mouse_dy;
			int mouse_tdx, mouse_tdy;
			int mouse_b;
			int mouse_z;
			
			int keyrep;
		//@}
		
		/// @name OIS required functions
		//@{
			bool keyPressed(const OIS::KeyEvent &arg);
			bool keyReleased(const OIS::KeyEvent &arg);
			
			bool mouseMoved(const OIS::MouseEvent &arg);
			bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
			bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		//@}
	private:
		/// Is only true when a given key is hit 
		bool pkey[OIS_KEYCODE_MAX];
		
		/// Is only true when a given key is released
		bool rkey[OIS_KEYCODE_MAX];
		
		bool key[OIS_KEYCODE_MAX];
		
		// Input manager
		OIS::InputManager *inputMgr;
		OIS::Keyboard *keyboard;
		OIS::Mouse *mouse;
};

extern class cmInput mIn;

#endif
