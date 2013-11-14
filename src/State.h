#ifndef StateH
#define StateH

namespace Gibdd
{
    class State
    {
        public:
            enum States
            {
                STATE_MENU = 0,
                STATE_PLAY = 1,
                STATE_DEBUG = 2
            };

        public:
            State() { }
            virtual ~State() { }

            virtual void mouseMove(const OIS::MouseEvent &e) = 0;
            virtual void mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) = 0;
            virtual void mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) = 0;

            virtual void keyPressed(const OIS::KeyEvent &e) = 0;
            virtual void keyReleased(const OIS::KeyEvent &e) = 0;

            virtual void povMoved(const OIS::JoyStickEvent &e, int pov) = 0;
            virtual void axisMoved(const OIS::JoyStickEvent &e, int axis) = 0;
            virtual void sliderMoved(const OIS::JoyStickEvent &e, int sliderID) = 0;
            virtual void buttonPressed(const OIS::JoyStickEvent &e, int button) = 0;
            virtual void buttonReleased(const OIS::JoyStickEvent &e, int button) = 0;

            virtual bool frameStarted(const Ogre::FrameEvent &evt) = 0;
            virtual bool frameEnded(const Ogre::FrameEvent &evt) = 0;
            virtual void preRenderTargetUpdate (const Ogre::RenderTargetEvent &evt) = 0;
            virtual void postRenderTargetUpdate (const Ogre::RenderTargetEvent &evt) = 0;

            virtual void Enter() = 0;
            virtual void Hide() = 0;
            virtual void Restore() = 0;
    };
}

#endif