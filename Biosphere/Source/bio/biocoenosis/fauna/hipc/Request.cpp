#include <bio/biocoenosis/fauna/hipc/Request.hpp>
#include <cstdlib>

namespace bio::hipc
{
    BufferInfo BufferInfo::Normal(u32 Type)
    {
        return { BufferMode::Normal, Type, 0, 0 };
    }

    BufferInfo BufferInfo::Static(u32 Index)
    {
        return { BufferMode::Static, 0, Index, 0 };
    }

    BufferInfo BufferInfo::Smart(size_t BufferSize, u32 Index)
    {
        return { BufferMode::Smart, 0, Index, BufferSize };
    }

    Buffer::Buffer(BufferInfo Info, const void *Buffer, size_t Size)
    {
        this->Info = Info;
        this->Data = Buffer;
        this->Size = Size;
    }

    RequestData::RequestData()
    {
        this->InProcessId = false;
        this->InRawSize = 0;
        this->OutRawSize = 0;
        this->OutProcessId = 0;
    }

    void Simple::Process(RequestData &Data, u8 Part)
    {
    }

    void InProcessId::Process(RequestData &Data, u8 Part)
    {
        switch(Part)
        {
            case 0:
                Data.InProcessId = true;
                break;
        }
    }

    InObjectId::InObjectId(u32 ObjectId)
    {
        this->ObjectId = ObjectId;
    }

    void InObjectId::Process(RequestData &Data, u8 Part)
    {
        switch(Part)
        {
            case 0:
                Data.InObjectIds.push_back(this->ObjectId);
                break;
        }
    }

    InNormalBuffer::InNormalBuffer(const void *Data, size_t Size, u32 Type) : InNormal(BufferInfo::Normal(Type), Data, Size)
    {
    }

    void InNormalBuffer::Process(RequestData &Data, u8 Part)
    {
        switch(Part)
        {
            case 0:
                Data.InNormalBuffers.push_back(this->InNormal);
                break;
        }
    }

    InStaticBuffer::InStaticBuffer(const void *Data, size_t Size, u32 Index) : InStatic(BufferInfo::Static(Index), Data, Size)
    {
    }

    void InStaticBuffer::Process(RequestData &Data, u8 Part)
    {
        switch(Part)
        {
            case 0:
                Data.InStaticBuffers.push_back(this->InStatic);
                break;
        }
    }

    InSmartBuffer::InSmartBuffer(const void *Data, size_t Size, u32 Index, u64 ExpectedSize) : InNormal({ BufferMode::Normal, 0, 0, 0 }, Data, Size), InStatic({ BufferMode::Static, 0, Index, 0 }, NULL, 0)
    {
        if((ExpectedSize != 0) && (Size <= ExpectedSize))
        {
            this->InNormal = Buffer({ BufferMode::Normal, 0, 0, 0 }, NULL, 0);
            this->InStatic = Buffer({ BufferMode::Static, 0, Index, 0 }, Data, Size);
        }
    }

    void InSmartBuffer::Process(RequestData &Data, u8 Part)
    {
        switch(Part)
        {
            case 0:
                Data.InNormalBuffers.push_back(this->InNormal);
                Data.InStaticBuffers.push_back(this->InStatic);
                break;
        }
    }

    OutProcessId::OutProcessId(u64 &ProcessId) : ProcessId(ProcessId)
    {
    }

    void OutProcessId::Process(RequestData &Data, u8 Part)
    {
        switch(Part)
        {
            case 4:
                this->ProcessId = Data.OutProcessId;
                break;
        }
    }

    OutNormalBuffer::OutNormalBuffer(const void *Data, size_t Size, u32 Type) : OutNormal(BufferInfo::Normal(Type), Data, Size)
    {
    }

    void OutNormalBuffer::Process(RequestData &Data, u8 Part)
    {
        switch(Part)
        {
            case 0:
                Data.OutNormalBuffers.push_back(this->OutNormal);
                break;
        }
    }

    OutStaticBuffer::OutStaticBuffer(const void *Data, size_t Size, u32 Index) : OutStatic(BufferInfo::Static(Index), Data, Size)
    {
    }

    void OutStaticBuffer::Process(RequestData &Data, u8 Part)
    {
        switch(Part)
        {
            case 0:
                Data.OutStaticBuffers.push_back(this->OutStatic);
                break;
        }
    }

    OutSmartBuffer::OutSmartBuffer(const void *Data, size_t Size, u32 Index, u64 ExpectedSize) : OutNormal({ BufferMode::Normal, 0, 0, 0 }, Data, Size), OutStatic({ BufferMode::Static, 0, Index, 0 }, NULL, 0)
    {
        if((ExpectedSize != 0) && (Size <= ExpectedSize))
        {
            this->OutNormal = Buffer({ BufferMode::Normal, 0, 0, 0 }, NULL, 0);
            this->OutStatic = Buffer({ BufferMode::Static, 0, Index, 0 }, Data, Size);
        }
    }

    void OutSmartBuffer::Process(RequestData &Data, u8 Part)
    {
        switch(Part)
        {
            case 0:
                Data.OutNormalBuffers.push_back(this->OutNormal);
                Data.OutStaticBuffers.push_back(this->OutStatic);
                break;
        }
    }
}