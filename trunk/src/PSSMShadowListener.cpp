#include "Precompiled.h"
#include "PSSMShadowListener.h"

namespace Gibdd
{
    PSSMShadowListener::PSSMShadowListener(Ogre::SceneManager *sm, Ogre::Light *l, Ogre::ShadowCameraSetupPtr s, Ogre::Camera *cam)
    {
        sceneMgr=sm;
        light=l;
        setup=s;
        view_camera=cam;
        split_index=0;
    }

    void PSSMShadowListener::shadowTextureCasterPreViewProj(Ogre::Light* , Ogre::Camera* camera)
    {
        static bool update=true;
        static float split_dist[]={0.1,10,10,70,70,500,0,0};
        float old_near=view_camera->getNearClipDistance();
        float old_far=view_camera->getFarClipDistance();
        if(split_index>0)
            view_camera->setNearClipDistance(split_dist[2*split_index]);
        view_camera->setFarClipDistance(split_dist[2*split_index+1]);
        if(update)
            setup->getShadowCamera(sceneMgr,view_camera, NULL, light, camera, 0);
        view_camera->setNearClipDistance(old_near);
        view_camera->setFarClipDistance(old_far);
        split_index++;
        if(split_index>2)
            split_index=0;
    }

    bool PSSMShadowListener::sortLightsAffectingFrustum(Ogre::LightList& lightList)
    {
        std::stable_sort(
            lightList.begin(), lightList.end(), 
            lightsLess());
        return true;
    }
}