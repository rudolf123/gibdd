#include "Precompiled.h"
#include "DBCStore.h"
#include "DBCStores.h"
#include "DBCFileLoader.h"

#pragma warning(disable: 4996)

namespace Gibdd
{
    DBCFileLoader::DBCFileLoader()
    {
        data = NULL;
        fieldsOffset = NULL;
    }

    DBCFileLoader::~DBCFileLoader()
    {
        if(data)
            delete [] data;

        if(fieldsOffset)
            delete [] fieldsOffset;
    }

    bool DBCFileLoader::Load(const char *filename, const char *fmt)
    {
        uint32 header;
        if(data)
        {
            delete [] data;
            data=NULL;
        }

        FILE * f=fopen(filename,"rb");
        if(!f)
            return false;

        if(fread(&header,4,1,f)!=1)                             // Number of records
            return false;

        if(header != 0x43424457)
            return false;                                       //'WDBC'

        if(fread(&recordCount,4,1,f)!=1)                        // Number of records
            return false;

        if(fread(&fieldCount,4,1,f)!=1)                         // Number of fields
            return false;

        if(fread(&recordSize,4,1,f)!=1)                         // Size of a record
            return false;

        if(fread(&stringSize,4,1,f)!=1)                         // String size
            return false;

        fieldsOffset = new uint32[fieldCount];
        fieldsOffset[0] = 0;
        for(uint32 i = 1; i < fieldCount; i++)
        {
            fieldsOffset[i] = fieldsOffset[i - 1];
            if (fmt[i - 1] == 'b' || fmt[i - 1] == 'X')         // byte fields
                fieldsOffset[i] += 1;
            else                                                // 4 byte fields (int32/float/strings)
                fieldsOffset[i] += 4;
        }

        data = new unsigned char[recordSize*recordCount+stringSize];
        stringTable = data + recordSize*recordCount;

        if(fread(data,recordSize*recordCount+stringSize,1,f)!=1)
            return false;

        fclose(f);
        return true;
    }

    /* DEAD CODE */
    template<class T>
    bool DBCFileLoader::Save(const char *filename, T* t, uint8 idx)
    {
        FILE* f = fopen(filename, "wb");
        if(!f)
            return false;

        char* header[4];
        memcpy(header, "WDBC", 4);
        fwrite(&header, 4, 1, f);

        uint32 _numRows = sObject.getObjectCountWithSimulation(idx);
        fwrite(&_numRows, 4, 1, f);

        uint32 _fieldCount = t->GetFieldCount();
        fwrite(&_fieldCount, 4, 1, f);

    //        shit hack
    //    uint8 multiplier = 1;
    //    if(filename == std::string("StaticObjectTemplate.dbc"))
    //        multiplier = 2;

        uint32 _recordSize = 0;
        uint32 _stringSize = 1;

        const char* fmt = t->GetFormat();
        for(uint8 i = 0; i < strlen(fmt); ++i)
        {
            switch(fmt[i])
            {
                case 'f':
                case 'i':
                    _recordSize += 4;
                    break;
                case 's':
                    _stringSize += sizeof(char*);
            }
        }
        //val *= multiplier;
        fwrite(&_recordSize, 4, 1, f);
        fwrite(&_stringSize, 4, 1, f);

        char* data = new char[_recordSize * _numRows + _stringSize];

        stdext::hash_map<uint32, GameObject*>::iterator itr = sObject.getObjectMap().begin();
        for(int c = 0; itr != sObject.getObjectMap().end(); ++itr)
        {
            assert(itr->second);
            if(itr->second->getCategory() != idx)
                continue;

            int32 i = itr->second->prototypeEntry;
            memcpy(data + c, &i, 4);
            c+=4;

            i = int32(-20);
            memcpy(data + c, &i, 4);
            c+=4;

            Ogre::Vector3 pos = itr->second->getNode()->_getDerivedPosition();
            float f = pos.x;
            memcpy(data + c, &f, 4);
            c+=4;

            f = pos.y;
            memcpy(data + c, &f, 4);
            c+=4;

            f = pos.z;
            memcpy(data + c, &f, 4);
            c+=4;
        }

        fwrite(data, _recordSize * _numRows + _stringSize, 1, f);

        delete [] data;

        fclose(f);
        return true;
    }

    DBCFileLoader::Record DBCFileLoader::getRecord(size_t id)
    {
        assert(data);
        return Record(*this, data + id*recordSize);
    }

    uint32 DBCFileLoader::GetFormatRecordSize(const char * format,int32* index_pos)
    {
        uint32 recordsize = 0;
        int32 i = -1;
        for(uint32 x=0; format[x];++x)
            switch(format[x])
            {
                case FT_FLOAT:
                case FT_INT:
                    recordsize+=4;
                    break;
                case FT_STRING:
                    recordsize+=sizeof(char*);
                    break;
                case FT_SORT:
                    i=x;
                    break;
                case FT_IND:
                    i=x;
                    recordsize+=4;
                    break;
                case FT_BYTE:
                    recordsize += 1;
                    break;
            }

        if(index_pos)
            *index_pos = i;

        return recordsize;
    }

    char* DBCFileLoader::AutoProduceData(const char* format, uint32& records, char**& indexTable)
    {
        /*
        format STRING, NA, FLOAT,NA,INT <=>
        struct{
        char* field0,
        float field1,
        int field2
        }entry;

        this func will generate  entry[rows] data;
        */

        typedef char * ptr;
        if(strlen(format)!=fieldCount)
            return NULL;

        //get struct size and index pos
        int32 i;
        uint32 recordsize=GetFormatRecordSize(format,&i);

        if(i>=0)
        {
            uint32 maxi=0;
            //find max index
            for(uint32 y=0;y<recordCount;y++)
            {
                uint32 ind=getRecord(y).getUInt (i);
                if(ind>maxi)maxi=ind;
            }

            ++maxi;
            records=maxi;
            indexTable=new ptr[maxi];
            memset(indexTable,0,maxi*sizeof(ptr));
        }
        else
        {
            records = recordCount;
            indexTable = new ptr[recordCount];
        }

        char* dataTable= new char[recordCount*recordsize];

        uint32 offset=0;

        for(uint32 y =0;y<recordCount;y++)
        {
            if(i>=0)
            {
                indexTable[getRecord(y).getUInt(i)]=&dataTable[offset];
            }
            else
                indexTable[y]=&dataTable[offset];

            for(uint32 x=0;x<fieldCount;x++)
            {
                switch(format[x])
                {
                    case FT_FLOAT:
                        *((float*)(&dataTable[offset]))=getRecord(y).getFloat(x);
                        offset+=4;
                        break;
                    case FT_IND:
                    case FT_INT:
                        *((uint32*)(&dataTable[offset]))=getRecord(y).getUInt(x);
                        offset+=4;
                        break;
                    case FT_BYTE:
                        *((uint8*)(&dataTable[offset]))=getRecord(y).getUInt8(x);
                        offset+=1;
                        break;
                    case FT_STRING:
                        *((char**)(&dataTable[offset]))=NULL;   // will be replaces non-empty or "" strings in AutoProduceStrings
                        offset+=sizeof(char*);
                        break;
                }
            }
        }

        return dataTable;
    }

    char* DBCFileLoader::AutoProduceStrings(const char* format, char* dataTable)
    {
        if(strlen(format)!=fieldCount)
            return NULL;

        char* stringPool= new char[stringSize];
        memcpy(stringPool,stringTable,stringSize);

        uint32 offset=0;

        for(uint32 y =0;y<recordCount;y++)
        {
            for(uint32 x=0;x<fieldCount;x++)
                switch(format[x])
            {
                case FT_FLOAT:
                case FT_IND:
                case FT_INT:
                    offset+=4;
                    break;
                case FT_BYTE:
                    offset+=1;
                    break;
                case FT_STRING:
                    // fill only not filled entries
                    char** slot = (char**)(&dataTable[offset]);
                    if(!*slot || !**slot)
                    {
                        const char * st = getRecord(y).getString(x);
                        *slot=stringPool+(st-(const char*)stringTable);
                    }
                    offset+=sizeof(char*);
                    break;
            }
        }

        return stringPool;
    }
}