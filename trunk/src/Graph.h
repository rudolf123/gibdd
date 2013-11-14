#ifndef GraphH
#define GraphH
#include "Root.h"
#include "DynamicLines.h"

namespace Gibdd
{
    enum PathFindingMethod
    {
        PFM_FLOYD,
        PFM_ASTAR
    };

    class GraphNode
    {
        public:
            GraphNode(Ogre::Vector3 pos)
            {
                position = pos;
                inOpen = false;
                inClose = false;
            }
            ~GraphNode()
            {

            }

            float f,g,h;
            void setfgh(Ogre::Vector3 targetpos, Ogre::Vector3 parentpos);
            void addLink(GraphNode* node){ linkedNodes.push_back(node); };

            //private:
            Ogre::Vector3 position;

            uint16 num;

            GraphNode *parent;
            GraphNode *next;
            bool inOpen;
            bool inClose;

            std::vector<GraphNode*> linkedNodes; 
    };

    class GraphEdge
    {
        public:
            GraphEdge(GraphNode *node1, GraphNode *node2)
            {
                point1 = node1;
                point2 = node2;
                weight = point1->position.squaredDistance(point2->position);
            }
            ~GraphEdge()
            {

            }

            GraphNode *point1;
            GraphNode *point2;
            float      weight;

            uint16 num;
    };

    class Graph
    {
        public:
            Graph(const std::string &name);
            ~Graph();


            void linkNodes(uint16 Numnode1, uint16 Numnode2);
            void linkNodesinOneway(uint16 Numnode1, uint16 Numnode2);
            void removeGraphNode(uint16 num);
            GraphNode* addGraphNode(Ogre::Vector3 pos);
            GraphNode* addGraphNode(uint16 num, Ogre::Vector3 pos);
            void addGraphEdge(Ogre::Vector3 point1, Ogre::Vector3 point2);
            void addGraphEdge(GraphNode *point1, GraphNode *point2);
            void writeGraphToFile(const std::string &filename);
            bool findPath(uint32 NumPoint1, uint32 NumPoint2, PathFindingMethod method);
            bool loadGraphFromFile(const std::string& filename);
            uint16 getNodesCount(){ return nodesCount; };
            Ogre::Vector3 getNextPointOfPath();
            uint16 getStartNode(){ return StartNode ;};
            uint16 getFinishNode(){ return FinishNode ;};
            void setVisualDebug( bool enable);
            bool getVisualDebug (){return bEnableDebug;};
            const std::string& GetName(){ return sName;};

            void RenderDebugLines();

        private:

            //floyd
            void FloydTravel(int f, int s);
            void filladjacencyMatrix();
            //\floyd

            //astar
            bool isNodeinCloseList(int NodeNum);
            bool isNodeinOpenList(int NodeNum);
            GraphNode* findMinimalinOpen();
            //\astar

            std::vector<GraphNode*> vNodes;
            std::vector<GraphEdge*> vEdges;
            std::string sName;

            DynamicLines *WPGraphicLinks;
            Ogre::SceneNode *WPGraphicLinksNode;
            Ogre::SceneManager *pSceneMgr;

            uint32 nodesCount;
            uint32 edgesCount;
            bool bEnableDebug;

            //astar
            std::priority_queue<GraphNode*, std::vector<GraphNode*>> AstarPrioritylist;
            std::vector<GraphNode*> AstarOpenlist;
            std::vector<GraphNode*> AstarCloselist;
            //\astar

            //floyd
            std::vector< std::vector<uint32> > adjacencyMatr;
            std::vector< std::vector<uint32> > FloydPar;
            std::vector< std::vector<uint32> > FloydD;
            //\floyd

            std::vector<GraphNode*> FoundingPath;

            std::vector<Ogre::Vector3> path;

            Ogre::Vector3 NextPointofPath;
            uint16 StartNode;
            uint16 FinishNode;
            bool bFloyd;
    };
}

#endif