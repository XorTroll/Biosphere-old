namespace bio::hipc
{
    template<typename RawType>
    InRaw<RawType>::InRaw(RawType Type)
    {
        this->Value = Type;
    }

    template<typename RawType>
    void InRaw<RawType>::Process(RequestData &Data, u8 Part)
    {
        switch(Part)
        {
            case 0:
                Data.InRawSize += (alignof(RawType) - 1);
                Data.InRawSize -= (Data.InRawSize % alignof(RawType));
                this->DataOffset = Data.InRawSize;
                Data.InRawSize += sizeof(RawType);
                break;
            case 2:
                *((RawType*)(((u8*)Data.InRawData) + this->DataOffset)) = this->Value;
                break;
        }
    }

    template<HandleMode HMode>
    InHandle<HMode>::InHandle(u32 Handle)
    {
        this->Handle = Handle;
        this->Mode = HMode;
    }

    template<HandleMode HMode>
    void InHandle<HMode>::Process(RequestData &Data, u8 Part)
    {
        switch(Part)
        {
            case 0:
                switch(this->Mode)
                {
                    case HandleMode::Copy:
                        Data.InCopyHandles.push_back(this->Handle);
                        break;
                    case HandleMode::Move:
                        Data.InMoveHandles.push_back(this->Handle);
                        break;
                }
                break;
        }
    }

    template<typename RawType>
    OutRaw<RawType>::OutRaw(RawType &ValueOut) : Value(ValueOut)
    {
    }

    template<typename RawType>
    void OutRaw<RawType>::Process(RequestData &Data, u8 Part)
    {
        switch(Part)
        {
            case 3:
                Data.OutRawSize += (alignof(RawType) - 1);
                Data.OutRawSize -= (Data.OutRawSize % alignof(RawType));
                this->DataOffset = Data.OutRawSize;
                Data.OutRawSize += sizeof(RawType);
                break;
            case 5:
                this->Value = *((RawType*)(((u8*)Data.OutRawData) + this->DataOffset));
                break;
        }
    }

    template<u32 OIndex>
    OutHandle<OIndex>::OutHandle(u32 &HandleOut) : Handle(HandleOut)
    {
        this->Index = OIndex;
    }

    template<u32 OIndex>
    void OutHandle<OIndex>::Process(RequestData &Data, u8 Part)
    {
        switch(Part)
        {
            case 4:
                if(Data.OutHandles.size() >= (this->Index + 1)) this->Handle = Data.OutHandles[this->Index];
                break;
        }
    }

    template<u32 OIndex>
    OutObjectId<OIndex>::OutObjectId(u32 &ObjectIdOut) : ObjectId(ObjectIdOut)
    {
        this->Index = OIndex;
    }

    template<u32 OIndex>
    void OutObjectId<OIndex>::Process(RequestData &Data, u8 Part)
    {
        switch(Part)
        {
            case 4:
                this->ObjectId = Data.OutObjectIds[this->Index];
                break;
        }
    }
}