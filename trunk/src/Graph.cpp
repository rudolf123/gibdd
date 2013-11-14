#include "Precompiled.h"
#include "Graph.h"

#pragma warning (disable : 4996)

namespace Gibdd
{
    Graph::Graph(const std::string &name):
    WPGraphicLinks(0),
    sName(name),
    nodesCount(0),
    edgesCount(0),
    bEnableDebug(false),
    bFloyd(false)
    {

    }

    Graph::~Graph()
    {
        std::vector<GraphNode*>::iterator NodeItr = vNodes.begin();
        for(; NodeItr != vNodes.end(); ++NodeItr)
        {
            delete *NodeItr;
        }
        std::vector<GraphEdge*>::iterator EdgeItr = vEdges.begin();
        for(; EdgeItr != vEdges.end(); ++EdgeItr)
        {
            delete *EdgeItr;
        }

        if (WPGraphicLinks)
        {
            pSceneMgr->destroySceneNode(WPGraphicLinksNode);
            delete WPGraphicLinks;
        }
    }

    void Graph::setVisualDebug(bool enable)
    {
        if (enable)
        {
            bEnableDebug = true;

            pSceneMgr = Root::GetInstance().GetSceneManager();
            WPGraphicLinks = new DynamicLines(Ogre::RenderOperation::OT_LINE_LIST, DEBUG_WAYPOINTS);
            WPGraphicLinksNode = pSceneMgr->getRootSceneNode()->createChildSceneNode(sName + "WPGraphicLinksNode");
            WPGraphicLinksNode->attachObject(WPGraphicLinks);

            RenderDebugLines();
        }
        else
        {
            bEnableDebug = false;
            if (WPGraphicLinks)
            {
                delete WPGraphicLinks;
                pSceneMgr->destroySceneNode(WPGraphicLinksNode);
            }
        }
    }


    void Graph::linkNodes(uint16 Numnode1, uint16 Numnode2)
    {
        /*std::vector<GraphNode*>::iterator NodeItr = vNodes.begin();
        GraphNode *node1;
        GraphNode *node2;
        for(; NodeItr != vNodes.end(); ++NodeItr)
        {
            if (((GraphNode*)*NodeItr)->num == Numnode1)
                node1 = ((GraphNode*)*NodeItr);
            if (((GraphNode*)*NodeItr)->num == Numnode2)
                node2 = ((GraphNode*)*NodeItr);
        }
        node1->addLink(node2);
        node2->addLink(node1);
        addGraphEdge(node1, node2);*/
        linkNodesinOneway(Numnode1, Numnode2);
        linkNodesinOneway(Numnode2, Numnode1);
    }

    void Graph::linkNodesinOneway(uint16 Numnode1, uint16 Numnode2)
    {
        std::vector<GraphNode*>::iterator NodeItr = vNodes.begin();
        GraphNode *node1;
        GraphNode *node2;
        for(; NodeItr != vNodes.end(); ++NodeItr)
        {
            if (((GraphNode*)*NodeItr)->num == Numnode1)
                node1 = ((GraphNode*)*NodeItr);
            if (((GraphNode*)*NodeItr)->num == Numnode2)
                node2 = ((GraphNode*)*NodeItr);
        }
        node1->addLink(node2);
        //node2->addLink(node1);
        addGraphEdge(node1, node2);
    }

    GraphNode* Graph::addGraphNode(Ogre::Vector3 pos)
    {
        GraphNode *node = new GraphNode(pos);
        node->num = nodesCount;
        vNodes.push_back(node);
        ++nodesCount;

        return node;
    }

    GraphNode* Graph::addGraphNode(uint16 num, Ogre::Vector3 pos)
    {
        GraphNode *node = new GraphNode(pos);
        node->num = num;
        vNodes.push_back(node);
        ++nodesCount;

        return node;
    }

    void Graph::addGraphEdge(Ogre::Vector3 point1, Ogre::Vector3 point2)
    {
        GraphEdge *edge = new GraphEdge(addGraphNode(point1), addGraphNode(point2));
        edge->num = edgesCount;
        vEdges.push_back(edge);
        ++edgesCount;

        if (bEnableDebug)
            RenderDebugLines();
    }

    void Graph::addGraphEdge(GraphNode *point1, GraphNode *point2)
    {
        GraphEdge *edge = new GraphEdge(point1, point2);
        edge->num = edgesCount;
        vEdges.push_back(edge);
        ++edgesCount;

        if (bEnableDebug)
            RenderDebugLines();
    }

    void Graph::RenderDebugLines()
    {
        std::vector<GraphEdge*>::iterator itr = vEdges.begin();
        for (;itr!=vEdges.end();++itr)
        {
            WPGraphicLinks->addPoint(((GraphEdge*)*itr)->point1->position);
            WPGraphicLinks->addPoint(((GraphEdge*)*itr)->point2->position);
        };
        WPGraphicLinks->update();
        WPGraphicLinksNode->needUpdate(true);
    }

    void Graph::writeGraphToFile(const std::string &filename)
    {
        std::ofstream outStream;
        outStream.open(filename.c_str(), std::ios_base::trunc);
        outStream << "#Nodes" << std::flush;
        outStream << "\n" << std::flush;
        std::vector<GraphNode*>::iterator nodeitr = vNodes.begin();
        if (nodeitr == vNodes.end())
        {
            outStream.close();
            return;
        }

        for (;nodeitr!=vNodes.end();++nodeitr)
        {
            outStream << ((GraphNode*)*nodeitr)->num << ";" 
                << ((GraphNode*)*nodeitr)->position.x << ";" 
                << ((GraphNode*)*nodeitr)->position.y << ";" 
                << ((GraphNode*)*nodeitr)->position.z << ";" 
                << std::flush;
            outStream << "\n" << std::flush;
        }

        outStream << "#Edges" << std::flush;
        outStream << "\n" << std::flush;

        std::vector<GraphEdge*>::iterator edgeitr = vEdges.begin();

        if (edgeitr == vEdges.end())
        {
            outStream.close();
            return;
        }

        for (;edgeitr!=vEdges.end();++edgeitr)
        {
            outStream << ((GraphEdge*)*edgeitr)->num << ";" 
                << ((GraphEdge*)*edgeitr)->point1->num << ";" 
                << ((GraphEdge*)*edgeitr)->point2->num << ";" 
                << ((GraphEdge*)*edgeitr)->weight << ";" 
                << std::flush;
            outStream << "\n" << std::flush;
        }
        outStream.close();
    }

    void Graph::filladjacencyMatrix()
    {
        for (uint32 i = 0; i < nodesCount; ++i)
        {
            std::vector<uint32> vec;
            for (uint32 j = 0; j < nodesCount; ++j)
            {
                vec.push_back(INFINITE);
            }     
            adjacencyMatr.push_back(vec);
            FloydPar.push_back(vec);
            FloydD.push_back(vec);
        }

        std::vector<GraphEdge*>::iterator itr;
        itr = vEdges.begin();

        for (;itr!=vEdges.end(); ++itr)
        {
            adjacencyMatr[((GraphEdge*)*itr)->point1->num][((GraphEdge*)*itr)->point2->num] = ((GraphEdge*)*itr)->weight;
            adjacencyMatr[((GraphEdge*)*itr)->point2->num][((GraphEdge*)*itr)->point1->num] = ((GraphEdge*)*itr)->weight;
        }

        for (uint32 i = 0; i < nodesCount; ++i)
            for (uint32 j = 0; j < nodesCount; ++j)
            {
                if (i == j)
                    FloydD[i][j] = 0;
                else
                    FloydD[i][j] = adjacencyMatr[i][j];
            }    
        for (uint32 i = 0; i < nodesCount; ++i)
            for (uint32 j = 0; j < nodesCount; ++j)
                    FloydPar[i][j] = i;
    }

    void Graph::FloydTravel(int f, int s)
    {
        LOG("par ", FloydPar[s][f]);
        LOG("f ", f);
        LOG("s ", s);
        if (f == s)
        {
           // FoundingPath.push_back( vNodes[f]);
        }
        else
        {
            FloydTravel(FloydPar[s][f], s);
          //  FoundingPath.push_back( vNodes[f]);
           
        }
    }

    bool Graph::findPath(uint32 NumPoint1, uint32 NumPoint2, PathFindingMethod method)
    {
        if (method == PFM_FLOYD)
        {
            if (!bFloyd)
            {
                filladjacencyMatrix();

                for (uint32 k = 0; k < nodesCount; ++k)
                    for (uint32 i = 0; i < nodesCount; ++i)
                        for (uint32 j = 0; j < nodesCount; ++j)
                            if (FloydD[i][j] > FloydD[i][k] + FloydD[k][j])
                            {
                                FloydD[i][j] = FloydD[i][k] + FloydD[k][j];
                                FloydPar[i][j] = FloydPar[k][j];
                            }

                bFloyd = true;
                if (FloydD[NumPoint1][NumPoint2] < INFINITE)
                    FloydTravel(NumPoint2, NumPoint1);
            }
            else
            {
                if (FloydD[NumPoint1][NumPoint2] < INFINITE)
                    FloydTravel(NumPoint2, NumPoint1);
            }

            return true;
        }

        if (method == PFM_ASTAR)
        {
            StartNode = NumPoint1;
            FinishNode = NumPoint2;
            GraphNode* currNode;
            std::vector<GraphNode*>::iterator nodeChildItr;
            currNode = vNodes[NumPoint1];
            currNode->f = 0;
            currNode->g = 0;
            currNode->h = 0;
            AstarOpenlist.clear();
            AstarCloselist.clear();
           // LOG("StartNode ", NumPoint1);
           // LOG("FinishPoint ", NumPoint2);
            AstarOpenlist.push_back(currNode);

            if (StartNode!=FinishNode)
            {
                while (true)
                {
                    GraphNode* currNode1 = findMinimalinOpen();
                    if (currNode->num == currNode1->num && currNode->num!=StartNode)
                        return false;
                    currNode = currNode1;

                    if (currNode->num == NumPoint2)
                        break;

                    nodeChildItr = currNode->linkedNodes.begin();
                    for(; nodeChildItr != currNode->linkedNodes.end(); ++nodeChildItr)
                    {
                        if (!isNodeinCloseList(((GraphNode*)*nodeChildItr)->num))
                        {
                            if(!isNodeinOpenList(((GraphNode*)*nodeChildItr)->num))
                            {
                                AstarOpenlist.push_back((GraphNode*)*nodeChildItr);
                                ((GraphNode*)*nodeChildItr)->parent = currNode;
                                ((GraphNode*)*nodeChildItr)->g = currNode->g + currNode->position.squaredDistance(((GraphNode*)*nodeChildItr)->position);
                                ((GraphNode*)*nodeChildItr)->h = vNodes[NumPoint2]->position.squaredDistance(((GraphNode*)*nodeChildItr)->position);
                                ((GraphNode*)*nodeChildItr)->f = ((GraphNode*)*nodeChildItr)->g + ((GraphNode*)*nodeChildItr)->h;
                            }
                            else
                            {
                                if (((GraphNode*)*nodeChildItr)->g < currNode->g)
                                {
                                    ((GraphNode*)*nodeChildItr)->parent = currNode;
                                    ((GraphNode*)*nodeChildItr)->g = currNode->g + currNode->position.squaredDistance(((GraphNode*)*nodeChildItr)->position);
                                    ((GraphNode*)*nodeChildItr)->h = vNodes[NumPoint2]->position.squaredDistance(((GraphNode*)*nodeChildItr)->position);
                                    ((GraphNode*)*nodeChildItr)->f = ((GraphNode*)*nodeChildItr)->g + ((GraphNode*)*nodeChildItr)->h;
                                }
                            }
                        }
                    }
                }

                path.clear();
               // path.push_back(vNodes[NumPoint1]->position);
                while (currNode->num!=NumPoint1)
                {
                    path.push_back(currNode->position);
                    currNode = currNode->parent;
                }
            }
            else
            {
                path.push_back(vNodes[NumPoint1]->position);
            }

        }

        return true;
    }
    GraphNode* Graph::findMinimalinOpen()
    {
        std::vector<GraphNode*>::iterator itr;
        std::vector<GraphNode*>::iterator eraseitr;
        GraphNode *node = 0;
        itr = AstarOpenlist.begin();
        eraseitr = itr;
        float min;
        min = AstarOpenlist.front()->f;
        node = AstarOpenlist.front();
        for(; itr!=AstarOpenlist.end(); ++itr)
            if (((GraphNode*)*itr)->f < min)
            {
                node = ((GraphNode*)*itr);
                eraseitr = itr;
            }
        if (eraseitr!=AstarOpenlist.end())
        {
            AstarOpenlist.erase(eraseitr);
            AstarCloselist.push_back(node);
        }

        return node;
    }

    bool Graph::isNodeinCloseList(int NodeNum)
    {
        std::vector<GraphNode*>::iterator itr;
        itr = AstarCloselist.begin();
        bool in;
        in = false;
        for(; itr!=AstarCloselist.end(); ++itr)
            if (((GraphNode*)*itr)->num == NodeNum)
                in = true;
        return in;
    }

    bool Graph::isNodeinOpenList(int NodeNum)
    {
        std::vector<GraphNode*>::iterator itr;
        itr = AstarOpenlist.begin();
        bool in;
        in = false;
        for(; itr!=AstarOpenlist.end(); ++itr)
            if (((GraphNode*)*itr)->num == NodeNum)
                in = true;
        return in;
    }

    Ogre::Vector3 Graph::getNextPointOfPath()
    {
        if (!path.empty())
        {   
            NextPointofPath = path.back();
            path.pop_back();
        }
        return NextPointofPath;
    }

    bool Graph::loadGraphFromFile(const std::string& filename)
    {
        std::string line;

        Ogre::Vector3 WP;

        std::ifstream ifs(filename.c_str());
        if(!ifs.is_open())
            return false;

        uint8 NodeNum = 0;
        uint8 pathsNum = 0;
        std::string Pathname;
        std::string WPName;

        bool ParseNodes = false;
        bool ParseEdges = false;

        nodesCount = 0;

        while(!ifs.eof())
        {
            std::getline(ifs, line);
            if(line.empty())
                continue;

            size_t found = line.find("#Nodes");
            if (found != std::string::npos)
            {
                ParseNodes = true;
                continue;
            }

            found = line.find("#Edges");
            if (found != std::string::npos)
            {
                ParseNodes = false;
                ParseEdges = true;
                continue;
            }

            if (ParseNodes)
            {
                float val[4];
                uint8 itr = 0;
                
                char* pch = strtok((char*)line.c_str(), ";");
                while(pch)
                {
                    if(itr > 3)
                        break;

                    val[itr] = atof(pch);

                    pch = strtok(NULL, ";");
                    ++itr;
                }
                addGraphNode(val[0], Ogre::Vector3(val[1], val[2], val[3]));
            }

            if (ParseEdges)
            {
                float val[4];
                uint8 itr = 0;

                char* pch = strtok((char*)line.c_str(), ";");
                while(pch)
                {
                    if(itr > 3)
                        break;

                    val[itr] = atof(pch);

                    pch = strtok(NULL, ";");
                    ++itr;
                }

                //LOGS("linking node: " + TO_STRING(val[1]) + " and " + TO_STRING(val[2]));
                linkNodesinOneway(val[1], val[2]);
            }
            if (bEnableDebug)
                RenderDebugLines();
            
        }
        ifs.close();

        return true;
    }
}