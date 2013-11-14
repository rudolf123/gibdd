#include "Precompiled.h"
#include "PhysicManager.h"
#include "Stream.h"
//#include "NxDebugRenderable.h"

namespace Gibdd
{
    PhysicManager::PhysicManager(Ogre::SceneManager *sceneMgr)
    {
        mSceneMgr=sceneMgr;
        initializePhysX();
        createPhysicScene();

        lines = new DynamicLines(RenderOperation::OT_LINE_LIST, DEBUG_PHYSX);
        linesNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("DebugLines");
        bDebugEnable = false;

        allocator = new UserAllocator;
        pControllerManager = NxCreateControllerManager(allocator);

      //  enableDebugLines();
    }

    PhysicManager::~PhysicManager()
    {
        if(PxSDK != NULL)
        {
            if(PxScene != NULL) 
                PxSDK->releaseScene(*PxScene); 
            PxScene = NULL;
            NxReleasePhysicsSDK(PxSDK);
            PxSDK = NULL;

            delete lines;
        }
    }

    void PhysicManager::showDebugLines(bool show)
    {
        if (show)
        {
            bDebugEnable = true;
            linesNode->attachObject(lines);
        }
        else
        {
            bDebugEnable = false;
            lines->detachFromParent();
        }

    }

    void PhysicManager::initializePhysX()
    {
        PxSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, NULL);
        //PxSDK->getFoundationSDK().getRemoteDebugger()->connect ("localhost", 5425);
    }

    void PhysicManager::createPhysicScene()
    {
        NxSceneDesc sceneDesc;
        sceneDesc.gravity.set(0,-9.8,0);
        sceneDesc.simType = NX_SIMULATION_SW;
      //  sceneDesc.groundPlane = true;
        PxScene = PxSDK->createScene(sceneDesc);

        PxSDK->setParameter(NX_SKIN_WIDTH, 0.01);
        PxSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
        PxSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
        PxSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);

        NxMaterial* defaultMaterial = PxScene->getMaterialFromIndex(0); 
        defaultMaterial->setRestitution(0.5);
        defaultMaterial->setStaticFriction(0.5);
        defaultMaterial->setDynamicFriction(0.5);
    }

    NxActor* PhysicManager::CreateBox(NxVec3 dim, NxVec3 pos, NxQuat orient)
    {
        NxActorDesc actorDesc;
        NxBodyDesc bodyDesc;
        NxBoxShapeDesc boxDesc;

        boxDesc.dimensions.set(dim.x, dim.y, dim.z);
        boxDesc.localPose.t = pos;

        actorDesc.shapes.pushBack(&boxDesc);
        actorDesc.body = &bodyDesc;
        actorDesc.density = 10;
        actorDesc.globalPose.t = pos;
        assert(actorDesc.isValid());

        return PxScene->createActor(actorDesc);
    }

    NxActor* PhysicManager::CreateSphere(const NxVec3& pos, const NxReal radius, const NxReal density)
    {
        assert(0 != gScene);

        NxSphereShapeDesc sphereDesc;
        sphereDesc.radius = radius;
        sphereDesc.localPose.t = NxVec3(0, radius, 0);

        NxActorDesc actorDesc;
        actorDesc.shapes.pushBack(&sphereDesc);
        actorDesc.globalPose.t = pos;

        NxBodyDesc bodyDesc;
        if (density)
        {
            actorDesc.body = &bodyDesc;
            actorDesc.density = density;
        }
        else
        {
            actorDesc.body = 0;
        }
        return PxScene->createActor(actorDesc);	
    }

    NxActor* PhysicManager::CreateBox( NxVec3 pos, NxVec3 dim, float density)
    {
        NxActorDesc actorDesc;
        NxBodyDesc bodyDesc;
        NxBoxShapeDesc boxDesc;

        boxDesc.dimensions.set(dim.x, dim.y, dim.z);

        actorDesc.shapes.pushBack(&boxDesc);
        actorDesc.body = &bodyDesc;
        actorDesc.density = density;
        actorDesc.globalPose.t = pos;
        assert(actorDesc.isValid());

        NxActor *actor =  PxScene->createActor(actorDesc);

        return actor;
    }

    NxActor* PhysicManager::CreateCapsule(float height, float radius)
    {

        /*
        NxActorDesc actorDesc;
        NxBodyDesc bodyDesc;
        NxCapsuleShapeDesc capsDesc;

        capsDesc.height = height;
        capsDesc.radius = radius;

        actorDesc.shapes.pushBack(&capsDesc);
        actorDesc.body = &bodyDesc;
        actorDesc.density = 10;
        assert(actorDesc.isValid());

        return PxScene->createActor(actorDesc);*/
    }

    NxActor* PhysicManager::CreateCapsule(NxVec3 pos, float height, float radius, float density)
    {

        NxCapsuleShapeDesc capsuleDesc;
        capsuleDesc.height = height;
        capsuleDesc.radius = radius;
        capsuleDesc.localPose.t = NxVec3(0, radius + 0.5f * height, 0);

        NxActorDesc actorDesc;
        actorDesc.shapes.pushBack(&capsuleDesc);
        actorDesc.globalPose.t = pos;

        NxBodyDesc bodyDesc;
        if (density)
        {
            actorDesc.body = &bodyDesc;
            actorDesc.density = density;
        }
        else
        {
            actorDesc.body = 0;
        }
        return PxScene->createActor(actorDesc);	
    }

    NxRevoluteJoint* PhysicManager::CreateRevoluteJoint(NxActor* a0, NxActor* a1, const NxVec3& globalAnchor, const NxVec3& globalAxis)
    {
        NxRevoluteJointDesc revDesc;
        revDesc.actor[0] = a0;
        revDesc.actor[1] = a1;
        revDesc.setGlobalAnchor(globalAnchor);
        revDesc.setGlobalAxis(globalAxis);

        return (NxRevoluteJoint*)PxScene->createJoint(revDesc);
    }

    NxRevoluteJoint* PhysicManager::CreateRevoluteJoint2(NxActor* a0, NxActor* a1, const NxVec3& localAnchor0,const NxVec3& localAnchor1, const NxVec3& localAxis0, const NxVec3& localAxis1)
    {
        NxRevoluteJointDesc revDesc;
        revDesc.actor[0] = a0;
        revDesc.actor[1] = a1;
        //revDesc.setGlobalAnchor(globalAnchor);
        //revDesc.setGlobalAxis(globalAxis);
        revDesc.localAnchor[0]=localAnchor0;
        revDesc.localAnchor[1]=localAnchor1;
        revDesc.localAxis[0]  =localAxis0;
        revDesc.localAxis[1]  =localAxis1;
        revDesc.projectionMode = NX_JPM_POINT_MINDIST;
        return (NxRevoluteJoint*)PxScene->createJoint(revDesc);
    }

    NxSphericalJoint* PhysicManager::CreateBodySphericalJoint(NxActor* a0, NxActor* a1, const NxVec3& globalAnchor, const NxVec3& globalAxis)
    {
        NxSphericalJointDesc sphericalDesc;
        sphericalDesc.actor[0] = a0;
        sphericalDesc.actor[1] = a1;
        sphericalDesc.setGlobalAnchor(globalAnchor);
        sphericalDesc.setGlobalAxis(globalAxis);

        sphericalDesc.flags |= NX_SJF_TWIST_LIMIT_ENABLED;
        sphericalDesc.twistLimit.low.value = -(NxReal)0.025*NxPi;
        sphericalDesc.twistLimit.low.hardness = 0.5;
        sphericalDesc.twistLimit.low.restitution = 0.5;
        sphericalDesc.twistLimit.high.value = (NxReal)0.025*NxPi;
        sphericalDesc.twistLimit.high.hardness = 0.5;
        sphericalDesc.twistLimit.high.restitution = 0.5;

        sphericalDesc.flags |= NX_SJF_SWING_LIMIT_ENABLED;
        sphericalDesc.swingLimit.value = (NxReal)0.25*NxPi;
        sphericalDesc.swingLimit.hardness = 0.5;
        sphericalDesc.swingLimit.restitution = 0.5;

        sphericalDesc.flags |= NX_SJF_TWIST_SPRING_ENABLED;
        sphericalDesc.twistSpring.spring = 0.5;
        sphericalDesc.twistSpring.damper = 1;

        sphericalDesc.flags |= NX_SJF_SWING_SPRING_ENABLED;
        sphericalDesc.swingSpring.spring = 0.5;
        sphericalDesc.swingSpring.damper = 1;

        //	sphericalDesc.flags |= NX_SJF_JOINT_SPRING_ENABLED;
        //	sphericalDesc.jointSpring.spring = 0.5;
        //	sphericalDesc.jointSpring.damper = 1;

        sphericalDesc.projectionDistance = (NxReal)0.15;
        sphericalDesc.projectionMode = NX_JPM_POINT_MINDIST;

        return (NxSphericalJoint*)PxScene->createJoint(sphericalDesc);
    }


    CharacterController* PhysicManager::CreateCharacterController(Vector3 position, float height, float radius)
    {
        return new CharacterController(pControllerManager, PxScene, toNx(position), radius, height);
    }

    void PhysicManager::DebugRender(const NxDebugRenderable&  data)
    {
        NxU32 lineCount = data.getNbLines();
        const NxDebugLine* line = data.getLines();

        lines->clear();
        while (lineCount--)
        {
        lines->addPoint(toOgre(line->p0));
        lines->addPoint(toOgre(line->p1));
        line++;
        }

        lines->update();
        linesNode->needUpdate(true);
    }

    void PhysicManager::simulate(float time)
    {
        PxScene->simulate(time);
        PxScene->flushStream();
        PxScene->fetchResults(NX_ALL_FINISHED, true);
        pControllerManager->updateControllers();

        if (bDebugEnable)
            DebugRender(*PxScene->getDebugRenderable());
    }

    NxActor* PhysicManager::createTriangleMesh(Entity *e)
    {
        NxTriangleMeshShapeDesc mShapeDesc;
        unsigned int mVertexCount = 0;
        unsigned int mIndexCount = 0;
        size_t vertex_count;
        Ogre::Vector3* vertices;
        size_t index_count;
        unsigned long* indices;
        bool added_shared = false;
        vertex_count = 0;
        index_count = 0;
        size_t current_offset = 0;
        size_t shared_offset = 0;
        size_t next_offset = 0;
        size_t index_offset = 0;
        for ( unsigned short i = 0; i < e->getMesh()->getNumSubMeshes(); ++i) 
        {
            Ogre::SubMesh* submesh = e->getMesh()->getSubMesh( i );

            if(submesh->useSharedVertices) 
            {
                if( !added_shared ) 
                {
                    mVertexCount += e->getMesh()->sharedVertexData->vertexCount;
                    added_shared = true;
                }
            }
            else 
            {
                mVertexCount += submesh->vertexData->vertexCount;
            }

            mIndexCount += submesh->indexData->indexCount;
        }

        vertices = new Vector3[mVertexCount];
        indices = new unsigned long[mIndexCount];

        NxVec3* mMeshVertices = new NxVec3[mVertexCount];
        NxU32* mMeshFaces = new NxU32[mIndexCount];
        NxMaterialIndex* mMaterials = new NxMaterialIndex[mIndexCount];

        NxMaterialIndex currentMaterialIndex = 0;
        added_shared = false;

        for ( unsigned short i = 0; i < e->getMesh()->getNumSubMeshes();i++) 
        {
            SubMesh* submesh = e->getMesh()->getSubMesh(i);
            VertexData* vertex_data=submesh->useSharedVertices ? e->getMesh()->sharedVertexData:submesh->vertexData;

            if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
            {
                if(submesh->useSharedVertices)
                {
                    added_shared = true;
                    shared_offset = current_offset;
                }

                const VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(VES_POSITION);
                HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
                unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(HardwareBuffer::HBL_READ_ONLY));

                float* pReal;

                for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize()) 
                {
                    posElem->baseVertexPointerToElement(vertex, &pReal);
                    mMeshVertices[current_offset + j] = NxVec3(pReal[0],pReal[1],pReal[2]);
                }

                vbuf->unlock();
                next_offset += vertex_data->vertexCount;
            }

            IndexData* index_data = submesh->indexData;

            size_t numTris = index_data->indexCount / 3;
            HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

            bool use32bitindexes = (ibuf->getType() == HardwareIndexBuffer::IT_32BIT);


            if ( use32bitindexes ) 
            {
                unsigned int* pInt = static_cast<unsigned int*>(ibuf->lock(HardwareBuffer::HBL_READ_ONLY));

                size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;


                for ( size_t k = 0; k < numTris*3; ++k) {
                    mMeshFaces[index_offset] = pInt[k] + static_cast<unsigned int>(offset);
                    mMaterials[index_offset++] = currentMaterialIndex;
                }
            }
            else 
            {
                unsigned short* pShort = reinterpret_cast<unsigned short*>(ibuf->lock(HardwareBuffer::HBL_READ_ONLY));
                size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

                for ( size_t k = 0; k < numTris*3; ++k) 
                {
                    mMeshFaces[index_offset] = static_cast<unsigned int>(pShort[k]) + static_cast<unsigned int>(offset);
                    mMaterials[index_offset++] = currentMaterialIndex;
                }

            }

            ibuf->unlock();
            current_offset = next_offset;
        }

        NxTriangleMeshDesc triangleDesc;
        triangleDesc.numVertices = mVertexCount;
        triangleDesc.points = mMeshVertices;
        triangleDesc.pointStrideBytes = sizeof(NxVec3);

        triangleDesc.numTriangles = mIndexCount / 3;
        triangleDesc.triangleStrideBytes = 3 * sizeof(NxU32);
        triangleDesc.triangles = mMeshFaces;
        triangleDesc.flags = 0;

        NxInitCooking();

        MemoryWriteBuffer buf;
        bool status = NxCookTriangleMesh(triangleDesc, buf);
        mShapeDesc.meshData = PxSDK->createTriangleMesh(MemoryReadBuffer(buf.data));
        NxCloseCooking();
        mShapeDesc.materialIndex = 0;
        NxActorDesc actorDesc;
        NxBodyDesc bodyDesc;

        actorDesc.shapes.push_back(&mShapeDesc);
        actorDesc.body = &bodyDesc;
        actorDesc.density = 500;

        Quaternion q = e->getParentSceneNode()->getOrientation();
        NxActor* actor = PxScene->createActor(actorDesc);
        actor->setGlobalPosition(toNx(e->getParentSceneNode()->getPosition()));
        actor->setGlobalOrientationQuat(toNx(q));
        actor->raiseBodyFlag(NX_BF_KINEMATIC);
        SetActorCollisionGroup(actor, GROUP_COLLIDABLE_NON_PUSHABLE);
       
        return actor;
    }

    void PhysicManager::SetActorCollisionGroup(NxActor *actor, NxCollisionGroup group)
    {
        NxU32 nbShapes = actor->getNbShapes();
        NxShape*const* shapes = actor->getShapes();

        while (nbShapes--){
            shapes[nbShapes]->setGroup(group);
        }
    } 
}