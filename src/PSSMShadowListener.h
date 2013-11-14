#ifndef PSSMShadowListenerH
#define PSSMShadowListenerH

namespace Gibdd
{
    class PSSMShadowListener: public Ogre::SceneManager::Listener
    {
        struct lightsLess
        {
            bool operator()(const Ogre::Light* l1, const Ogre::Light* l2) const
            {
                if (l1 == l2)
                    return false;
                return l1->tempSquareDist < l2->tempSquareDist;
            }
        };

        public:
            PSSMShadowListener(Ogre::SceneManager *sm,Ogre::Light *l,Ogre::ShadowCameraSetupPtr s,Ogre::Camera *cam);
            virtual ~PSSMShadowListener() {}
            //virtual void shadowTexturesUpdated(size_t numberOfShadowTextures);
            virtual void shadowTextureCasterPreViewProj(Ogre::Light* light,Ogre::Camera* camera);
           // virtual void shadowTextureReceiverPreViewProj(Ogre::Light* light,Ogre::Frustum* frustum);
            virtual bool sortLightsAffectingFrustum(Ogre::LightList& lightList);
        private:
            Ogre::Light *light;
            Ogre::ShadowCameraSetupPtr setup;
            Ogre::Camera *view_camera;        // NOT shadow camera!
            Ogre::SceneManager *sceneMgr;
            mutable int split_index;
    };
}
#endif