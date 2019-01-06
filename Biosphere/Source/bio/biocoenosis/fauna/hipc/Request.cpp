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

    InBuffer::InBuffer(const void *Data, size_t Size, u32 Type) : In(BufferInfo::Normal(Type), Data, Size)
    {
    }

    void InBuffer::Process(RequestData &Data, u8 Part)
    {
        switch(Part)
        {
            case 0:
                Data.InBuffers.push_back(this->In);
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

    InSmartBuffer::InSmartBuffer(const void *Data, size_t Size, u32 Index, u64 ExpectedSize) : In({ BufferMode::Normal, 0, 0, 0 }, Data, Size), InStatic({ BufferMode::Static, 0, Index, 0 }, NULL, 0)
    {
        if((ExpectedSize != 0) && (Size <= ExpectedSize))
        {
            this->In = Buffer({ BufferMode::Normal, 0, 0, 0 }, NULL, 0);
            this->InStatic = Buffer({ BufferMode::Static, 0, Index, 0 }, Data, Size);
        }
    }

    void InSmartBuffer::Process(RequestData &Data, u8 Part)
    {
        switch(Part)
        {
            case 0:
                Data.InBuffers.push_back(this->In);
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

    OutBuffer::OutBuffer(const void *Data, size_t Size, u32 Type) : Out(BufferInfo::Normal(Type), Data, Size)
    {
    }

    void OutBuffer::Process(RequestData &Data, u8 Part)
    {
        switch(Part)
        {
            case 0:
                Data.OutBuffers.push_back(this->Out);
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

    OutSmartBuffer::OutSmartBuffer(const void *Data, size_t Size, u32 Index, u64 ExpectedSize) : Out({ BufferMode::Normal, 0, 0, 0 }, Data, Size), OutStatic({ BufferMode::Static, 0, Index, 0 }, NULL, 0)
    {
        if((ExpectedSize != 0) && (Size <= ExpectedSize))
        {
            this->Out = Buffer({ BufferMode::Normal, 0, 0, 0 }, NULL, 0);
            this->OutStatic = Buffer({ BufferMode::Static, 0, Index, 0 }, Data, Size);
        }
    }

    void OutSmartBuffer::Process(RequestData &Data, u8 Part)
    {
        switch(Part)
        {
            case 0:
                Data.OutBuffers.push_back(this->Out);
                Data.OutStaticBuffers.push_back(this->OutStatic);
                break;
        }
    }
}