/*!
	@file
	@author		Albert Semenov
	@date		04/2009
	@module
*/

#ifndef __MYGUI_OGRE_VERTEX_BUFFER_H__
#define __MYGUI_OGRE_VERTEX_BUFFER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"

#include <Ogre/OgreHardwareBufferManager.h>
#include <Ogre/OgreHardwareVertexBuffer.h>
#include <Ogre/OgrePrerequisites.h>
#include <Ogre/OgreRenderOperation.h>
#include <Ogre/OgreRenderSystem.h>
#include <Ogre/OgreTextureManager.h>
#include <Ogre/OgreTextureUnitState.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	class OgreVertexBuffer : public IVertexBuffer
	{
	public:
		OgreVertexBuffer();
		virtual ~OgreVertexBuffer();

		virtual void setVertextCount(size_t _count);
		virtual size_t getVertextCount();

		virtual Vertex* lock();
		virtual void unlock();

		Ogre::RenderOperation* getRenderOperation() { return &mRenderOperation; }

	private:
		void createVertexBuffer();
		void destroyVertexBuffer();
		void resizeVertexBuffer();

	private:
		size_t mVertexCount;
		size_t mNeedVertexCount;

		Ogre::RenderOperation mRenderOperation;
		Ogre::HardwareVertexBufferSharedPtr mVertexBuffer;
	};

} // namespace MyGUI

#endif // __MYGUI_OGRE_VERTEX_BUFFER_H__
