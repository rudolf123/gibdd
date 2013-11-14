#ifndef DbcStoreH
#define DbcStoreH

#include "DBCFileLoader.h"

namespace Gibdd
{
    template<class T>
    class DBCStorage
    {
        typedef std::list<char*> StringPoolList;
        public:
            explicit DBCStorage(const char *f) : nCount(0), fieldCount(0), fmt(f), indexTable(NULL), m_dataTable(NULL) { }
            ~DBCStorage() { Clear(); }

            T const* LookupEntry(uint32 id) const { return (id>=nCount)?NULL:indexTable[id]; }

            uint32  GetNumRows() const { return nCount; }
            char const* GetFormat() const { return fmt; }
            uint32 GetFieldCount() const { return fieldCount; }

            bool Load(char const* fn)
            {
                DBCFileLoader dbc;
                if(!dbc.Load(fn, fmt))
                    return false;

                fieldCount = dbc.GetCols();
                m_dataTable = (T*)dbc.AutoProduceData(fmt,nCount,(char**&)indexTable);
                m_stringPoolList.push_back(dbc.AutoProduceStrings(fmt,(char*)m_dataTable));

                return indexTable != NULL;
            }

            bool LoadStringsFrom(char const* fn)
            {
                if(!indexTable)
                    return false;

                DBCFileLoader dbc;
                if(!dbc.Load(fn, fmt))
                    return false;

                m_stringPoolList.push_back(dbc.AutoProduceStrings(fmt,(char*)m_dataTable));

                return true;
            }

            void Clear()
            {
                if (!indexTable)
                    return;

                delete[] ((char*)indexTable);
                indexTable = NULL;
                delete[] ((char*)m_dataTable);
                m_dataTable = NULL;

                while(!m_stringPoolList.empty())
                {
                    delete[] m_stringPoolList.front();
                    m_stringPoolList.pop_front();
                }
                nCount = 0;
            }

        private:
            uint32 nCount;
            uint32 fieldCount;
            char const* fmt;
            T** indexTable;
            T* m_dataTable;
            StringPoolList m_stringPoolList;
    };
}

#endif
