
#pragma once
#include <bio/biotope/arm.hpp>
#include <bio/biotope/svc.hpp>
#include <cstdlib>
#include <vector>

namespace bio::hipc
{
    class Object;

    enum class HandleMode
    {
        Copy,
        Move
    };

    enum class BufferMode
    {
        Normal,
        Static,
        Smart,
    };

    struct BufferInfo
    {
        BufferMode Mode;
        u32 Type;
        u32 Index;
        size_t BufferSize;
        static BufferInfo Normal(u32 Type);
        static BufferInfo Static(u32 Index);
        static BufferInfo Smart(size_t BufferSize, u32 Index);
    };

    struct Buffer
    {
        BufferInfo Info;
        const void *Data;
        size_t Size;

        Buffer(BufferInfo Info, const void *Buffer, size_t Size);
    };

    struct BufferCommandData
    {
        u32 Size;
        u32 Address;
        u32 Packed;
    };

    struct BufferReceiveData
    {
        u32 Address;
        u32 Packed;
    };

    struct BufferSendData
    {
        u32 Packed;
        u32 Address;
    };

    struct DomainHeader
    {
        u8 Type;
        u8 ObjectIdCount;
        u16 Size;
        u32 ObjectId;
        u32 Pad[2];
    };

    struct DomainResponse
    {
        u32 ObjectIdCount;
        u32 Pad[3];
    };

    struct RequestData
    {
        bool InProcessId;
        u8 *InRawData;
        u64 InRawSize;
        std::vector<u32> InCopyHandles;
        std::vector<u32> InMoveHandles;
        std::vector<u32> InObjectIds;
        std::vector<Buffer> InBuffers;
        std::vector<Buffer> InStaticBuffers;
        u64 OutProcessId;
        std::vector<u32> OutHandles;
        std::vector<u32> OutObjectIds;
        u8 *OutRawData;
        u64 OutRawSize;
        std::vector<Buffer> OutBuffers;
        std::vector<Buffer> OutStaticBuffers;
        std::vector<Buffer> ExchangeBuffers;

        RequestData();
    };

    struct RequestArgument
    {
        virtual void Process(RequestData &Data, u8 Part) = 0;
    };

    struct Simple : RequestArgument
    {
        void Process(RequestData &Data, u8 Part) override;
    };

    template<typename RawType>
    struct InRaw : RequestArgument
    {
        RawType Value;
        u64 DataOffset;

        InRaw(RawType Type);
        void Process(RequestData &Data, u8 Part) override;
    };

    struct InProcessId : RequestArgument
    {
        void Process(RequestData &Data, u8 Part) override;
    };

    template<HandleMode HMode>
    struct InHandle : RequestArgument
    {
        u32 Handle;
        HandleMode Mode;

        InHandle(u32 Handle);
        void Process(RequestData &Data, u8 Part) override;
    };

    struct InObjectId : RequestArgument
    {
        u32 ObjectId;

        InObjectId(u32 ObjectId);
        void Process(RequestData &Data, u8 Part) override;
    };

    struct InBuffer : RequestArgument
    {
        Buffer In;

        InBuffer(const void *Data, size_t Size, u32 Type);
        void Process(RequestData &Data, u8 Part) override;
    };

    struct InStaticBuffer : RequestArgument
    {
        Buffer InStatic;

        InStaticBuffer(const void *Data, size_t Size, u32 Index);
        void Process(RequestData &Data, u8 Part) override;
    };

    struct InSmartBuffer : RequestArgument
    {
        Buffer In;
        Buffer InStatic;

        InSmartBuffer(const void *Data, size_t Size, u32 Index, u64 ExpectedSize);
        void Process(RequestData &Data, u8 Part) override;
    };

    template<typename RawType>
    struct OutRaw : RequestArgument
    {
        RawType &Value;
        u64 DataOffset;

        OutRaw(RawType &ValueOut);
        void Process(RequestData &Data, u8 Part) override;
    };

    struct OutProcessId : RequestArgument
    {
        u64 &ProcessId;

        OutProcessId(u64 &ProcessIdOut);
        void Process(RequestData &Data, u8 Part) override;
    };

    template<u32 OIndex>
    struct OutHandle : RequestArgument
    {
        u32 Index;
        u32 &Handle;
        HandleMode Mode;

        OutHandle(u32 &HandleOut);
        void Process(RequestData &Data, u8 Part) override;
    };

    template<u32 OIndex>
    struct OutObjectId : RequestArgument
    {
        u32 Index;
        u32 &ObjectId;

        OutObjectId(u32 &OutObjectId);
        void Process(RequestData &Data, u8 Part) override;
    };

    struct OutBuffer : RequestArgument
    {
        Buffer Out;

        OutBuffer(const void *Data, size_t Size, u32 Type);
        void Process(RequestData &Data, u8 Part) override;
    };

    struct OutStaticBuffer : RequestArgument
    {
        Buffer OutStatic;

        OutStaticBuffer(const void *Data, size_t Size, u32 Index);
        void Process(RequestData &Data, u8 Part) override;
    };

    struct OutSmartBuffer : RequestArgument
    {
        Buffer Out;
        Buffer OutStatic;

        OutSmartBuffer(const void *Data, size_t Size, u32 Index, u64 ExpectedSize);
        void Process(RequestData &Data, u8 Part) override;
    };

    static const u32 SFCI = 0x49434653;
    static const u32 SFCO = 0x4f434653;

    static const u32 Module = 11;
    static const Result ResultUnsupportedOperation(Module, 1);
    static const Result ResultOutOfSessionMemory(Module, 102);
    static const Result ResultMarshallingPastMaximum(Module, 141);
    static const Result ResultUnsupportedDomains(Module, 200);
    static const Result ResultRemoteProcessDead(Module, 301);
    static const Result ResultUnknownRequestType(Module, 403);
    static const Result ResultQuery1Fail(Module, 491);
}

#include <bio/biocoenosis/fauna/hipc/Request.ipp>