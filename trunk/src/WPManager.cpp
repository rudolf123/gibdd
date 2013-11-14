#include "Precompiled.h"
#include "WPManager.h"
#include "Root.h"
#include <fstream>

namespace Gibdd
{
    WPManager::WPManager(const std::string &name):
    sName(name),
    bEnableDebug (false),
    iGraphsCount(0)
    {
    }

    WPManager::~WPManager()
    {
        std::vector<Graph*>::iterator GraphItr = vGraphs.begin();
        for(; GraphItr != vGraphs.end(); ++GraphItr)
        {
            delete (*GraphItr);
        }
    }

    void WPManager::setVisualDebug( bool enable)
    {
        bEnableDebug = enable;
        std::vector<Graph*>::iterator GraphItr = vGraphs.begin();
        for(; GraphItr != vGraphs.end(); ++GraphItr)
        {
            ((Graph*)*GraphItr)->setVisualDebug(enable);
        }
    }

    Graph* WPManager::CreateGraph(const std::string &graphname)
    {
        Graph* graph = new Graph(graphname);
        vGraphs.push_back(graph);
        ++iGraphsCount;

        if (bEnableDebug)
            graph->setVisualDebug(bEnableDebug);

        return graph;
    }

    Graph* WPManager::GetGraph(const std::string &graphname)
    {
        std::vector<Graph*>::iterator GraphItr = vGraphs.begin();
        for(; GraphItr != vGraphs.end(); ++GraphItr)
        {
           if (((Graph*)*GraphItr)->GetName() == graphname)
               return ((Graph*)*GraphItr);
        }

        return NULL;
    }

    bool WPManager::HasGraph(const std::string &graphname)
    {
        std::vector<Graph*>::iterator GraphItr = vGraphs.begin();
        for(; GraphItr != vGraphs.end(); ++GraphItr)
        {
            if (((Graph*)*GraphItr)->GetName() == graphname)
                return true;
        }

        return false;
    }

    void WPManager::writeGraphToFile(const std::string &filename, Graph* graph)
    {
        graph->writeGraphToFile(filename);
    }
    void WPManager::writeGraphToFile(const std::string &filename, const std::string &graphname)
    {
        Graph* graph = GetGraph(graphname);
        graph->writeGraphToFile(filename);
    }
    Graph* WPManager::loadGraphFromFile(const std::string& filename, const std::string &graphname)
    {
        Graph* graph = CreateGraph(graphname);
        graph->loadGraphFromFile(filename);
     //   if (bEnableDebug)
       //     graph->RenderDebugLines();
        
        return graph;
    }

}