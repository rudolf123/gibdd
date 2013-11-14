#ifndef DBCFileLoaderH
#define DBCFileLoaderH

enum
{
    FT_NA='x',
    FT_NA_BYTE='X',
    FT_STRING='s',
    FT_FLOAT='f',
    FT_INT='i',
    FT_BYTE='b',
    FT_SORT='d',
    FT_IND='n',
    FT_LOGIC='l'
};

namespace Gibdd
{
    class DBCFileLoader
    {
        public:
            DBCFileLoader();
            ~DBCFileLoader();

            bool Load(const char *filename, const char *fmt);

            template<class T>
            static bool Save(const char *filename, T* t, Gibdd::uint8 idx);

            class Record
            {
                public:
                    float getFloat(size_t field) const
                    {
                        assert(field < file.fieldCount);
                        float val = *reinterpret_cast<float*>(offset+file.GetOffset(field));
                        return val;
                    }
                    uint32 getUInt(size_t field) const
                    {
                        assert(field < file.fieldCount);
                        uint32 val = *reinterpret_cast<uint32*>(offset+file.GetOffset(field));
                        return val;
                    }
                    uint8 getUInt8(size_t field) const
                    {
                        assert(field < file.fieldCount);
                        return *reinterpret_cast<uint8*>(offset+file.GetOffset(field));
                    }

                    const char *getString(size_t field) const
                    {
                        assert(field < file.fieldCount);
                        size_t stringOffset = getUInt(field);
                        assert(stringOffset < file.stringSize);
                        return reinterpret_cast<char*>(file.stringTable + stringOffset);
                    }

                private:
                    Record(DBCFileLoader &file_, unsigned char *offset_): offset(offset_), file(file_) {}
                    unsigned char *offset;
                    DBCFileLoader &file;

                    friend class DBCFileLoader;

            };

            Record getRecord(size_t id);

            uint32 GetNumRows() const { return recordCount;}
            uint32 GetCols() const { return fieldCount; }
            uint32 GetOffset(size_t id) const { return (fieldsOffset != NULL && id < fieldCount) ? fieldsOffset[id] : 0; }
            bool IsLoaded() {return (data!=NULL);}
            char* AutoProduceData(const char* fmt, uint32& count, char**& indexTable);
            char* AutoProduceStrings(const char* fmt, char* dataTable);
            static uint32 GetFormatRecordSize(const char * format, int32 * index_pos = NULL);

        private:
            uint32 recordSize;
            uint32 recordCount;
            uint32 fieldCount;
            uint32 stringSize;
            uint32 *fieldsOffset;
            unsigned char *data;
            unsigned char *stringTable;
    };
}

#endif
