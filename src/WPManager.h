#ifndef WPManagerH
#define WPManagerH
#include <queue>
#include <deque>
#include "Graph.h"


namespace Gibdd
{
    class WPManager
    {
        public:
            WPManager(const std::string &name);
            ~WPManager();

            Graph* CreateGraph(const std::string &graphname);
            Graph* GetGraph(const std::string &graphname);
            void writeGraphToFile(const std::string &filename, Graph* graph);
            void writeGraphToFile(const std::string &filename, const std::string &graphname);
            Graph* loadGraphFromFile(const std::string& filename, const std::string &graphname);
            uint16 GetGraphsCount(){ return iGraphsCount; };
            void setVisualDebug( bool enable);
            bool getVisualDebug (){return bEnableDebug;};

            bool HasGraph(const std::string &graphname);

        private:

            std::vector<Graph*> vGraphs;
            std::string sName;
            
            Ogre::SceneManager *pSceneMgr;

            uint32 iGraphsCount;
            bool bEnableDebug;
    };
}
#endif