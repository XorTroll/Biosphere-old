
#pragma once
#include <bio/biocoenosis/fauna/hipc/Request.hpp>
#include <type_traits>

namespace bio::hipc
{
    enum class ObjectType
    {
        Uninitialized,
        Normal,
        Domain,
        SubService,
        HomebrewOverride,
    };

    class Object
    {
        public:
            Object();
            Object(u32 Handle, bool Override = false);
            Object(u32 Handle, u32 ObjectId, ObjectType Type);
            Object(Object *Parent, u32 ObjectId);
            ~Object();
            u32 GetHandle();
            u32 GetObjectId();
            ObjectType GetType();
            bool IsValid();
            bool IsActive();
            bool IsNormal();
            bool IsDomain();
            bool IsSubService();
            Result ConvertToDomain();
            ResultWrap<size_t> QueryPointerBufferSize();
            template<u32 CommandId, typename ...Arguments>
            Result ProcessRequest(Arguments &&...Args);
            void Close();
        protected:
            u32 handle;
            u32 objid;
            ObjectType type;
        private:
            template<typename Argument>
            void ProcessArgument(RequestData &Data, u8 Part, Argument &&Arg);
            template<typename Argument, typename ...Arguments>
            void ProcessArgument(RequestData &Data, u8 Part, Argument &&Arg, Arguments &&...Args);
    };

    class ServiceObject
    {
        public:
            ServiceObject(const char *Name);
            const char *GetName();
        private:
            const char *name;
    };
}

#include <bio/biocoenosis/fauna/hipc/Object.ipp>