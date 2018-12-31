#include <bio/biocoenosis/fauna/hipc/Object.hpp>

namespace bio::hipc
{
    Object::Object()
    {
        this->handle = 0;
        this->objid = 0;
        this->type = ObjectType::Uninitialized;
    }

    Object::Object(u32 Handle)
    {
        this->handle = Handle;
        this->objid = UINT32_MAX;
        this->type = ObjectType::Normal;
    }

    Object::Object(u32 Handle, u32 ObjectId, ObjectType Type)
    {
        this->handle = Handle;
        this->objid = ObjectId;
        this->type = Type;
    }

    Object::Object(Object *Parent, u32 ObjectId)
    {
        this->handle = Parent->GetHandle();
        this->objid = ObjectId;
        this->type = ObjectType::SubService;
    }

    Object::~Object()
    {
        this->Close();
    }

    u32 Object::GetHandle()
    {
        return this->handle;
    }

    u32 Object::GetObjectId()
    {
        return this->objid;
    }

    ObjectType Object::GetType()
    {
        return this->type;
    }

    bool Object::IsValid()
    {
        return (this->type != ObjectType::Uninitialized);
    }

    bool Object::IsActive()
    {
        return this->IsValid();
    }

    bool Object::IsNormal()
    {
        return (this->type == ObjectType::Normal);
    }

    bool Object::IsDomain()
    {
        return (this->type == ObjectType::Domain);
    }

    bool Object::IsSubService()
    {
        return (this->type == ObjectType::SubService);
    }

    Result Object::ConvertToDomain()
    {
        Result rc = 0;
        if((!this->IsDomain()) && (!this->IsSubService()))
        {
            u32 *tls = (u32*)arm::GetThreadLocalStorage();
            tls[1] = 8;
            tls[4] = SFCI;
            tls[5] = 0;
            tls[6] = 0;
            tls[7] = 0;
            rc = svc::SendSyncRequest(this->handle);
            if(rc.IsFailure()) return rc;
            u32 ctrl0 = *tls++;
            u32 ctrl1 = *tls++;
            bool gotpid = false;
            u64 outpid = 0;
            std::vector<u32> outhdls;
            if(ctrl1 & 0x80000000)
            {
                u32 ctrl2 = *tls++;
                if(ctrl2 & 1)
                {
                    gotpid = true;
                    u64 opid = *tls++;
                    opid |= (((u64)(*tls++)) << 32);
                    outpid = opid;
                }
                size_t shcopy = ((ctrl2 >> 1) & 15);
                size_t shmove = ((ctrl2 >> 5) & 15);
                size_t sh = shcopy + shmove;
                u32 *ashbuf = tls + sh;
                if(sh > 8) sh = 8;
                if(sh > 0) for(u32 i = 0; i < sh; i++) outhdls.push_back((u32)(*(tls + i)));
                tls = ashbuf;
            }
            size_t s_st = ((ctrl0 >> 16) & 15);
            u32 *abst = (tls + s_st * 2);
            if(s_st > 8) s_st = 8;
            for(u32 i = 0; i < s_st; i++, tls += 2)
            {
                BufferSendData *bsd = (BufferSendData*)tls;
                BIO_IGNORE(bsd);
            }
            tls = abst;
            size_t sends = ((ctrl0 >> 20) & 15);
            size_t recvs = ((ctrl0 >> 24) & 15);
            size_t exchs = ((ctrl0 >> 28) & 15);
            size_t bufs = (sends + recvs + exchs);
            void *outraw = (void*)(((uintptr_t)(tls + bufs * 3) + 15) &~ 15);
            void *wpadraw = (void*)((uintptr_t)(tls + bufs * 3));
            BIO_IGNORE(wpadraw);
            if(bufs > 8) bufs = 8;
            for(u32 i = 0; i < bufs; i++, tls += 3)
            {
                BufferCommandData *bcd = (BufferCommandData*)tls;
                BIO_IGNORE(bcd);
            }
            struct ConvertRaw
            {
                u64 magic;
                u64 res;
                u32 oid;
            } *oraw = (ConvertRaw*)outraw;
            rc = oraw->res;
            if(rc.IsSuccess())
            {
                this->objid = oraw->oid;
                this->type = ObjectType::Domain;
            }
        }
        return rc;
    }

    void Object::Close()
    {
        if(this->IsActive())
        {
            if(this->IsSubService())
            {
                RequestData rq;
                rq.InRawSize = sizeof(DomainHeader);
                u32 *tls = (u32*)arm::GetThreadLocalStorage();
                *tls++ = (4 | (rq.InStaticBuffers.size() << 16) | (rq.InNormalBuffers.size() << 20) | (rq.OutNormalBuffers.size() << 24) | (rq.ExchangeBuffers.size() << 28));
                u32 *fillsz = tls;
                if(rq.OutStaticBuffers.size() > 0) *tls = ((rq.OutStaticBuffers.size() + 2) << 10);
                else *tls = 0;
                if(rq.InProcessId || !rq.InCopyHandles.empty() || !rq.InMoveHandles.empty())
                {
                    *tls++ |= 0x80000000;
                    *tls++ = ((!!rq.InProcessId) | (rq.InCopyHandles.size() << 1) | (rq.InMoveHandles.size() << 5));
                    if(rq.InProcessId) tls += 2;
                    if(!rq.InCopyHandles.empty()) for(u32 i = 0; i < rq.InCopyHandles.size(); i++) *tls++ = rq.InCopyHandles[i];
                    if(!rq.InMoveHandles.empty()) for(u32 i = 0; i < rq.InMoveHandles.size(); i++) *tls++ = rq.InMoveHandles[i];
                }
                else tls++;
                if(!rq.InStaticBuffers.empty()) for(u32 i = 0; i < rq.InStaticBuffers.size(); i++, tls += 2)
                {
                    Buffer ins = rq.InStaticBuffers[i];
                    BufferSendData *bsd = (BufferSendData*)tls;
                    uintptr_t uptr = (uintptr_t)ins.Data;
                    bsd->Address = uptr;
                    bsd->Packed = (ins.Info.Index | (ins.Size << 16) | (((uptr >> 32) & 15) << 12) | (((uptr >> 36) & 15) << 6));
                }
                if(!rq.InNormalBuffers.empty()) for(u32 i = 0; i < rq.InNormalBuffers.size(); i++, tls += 3)
                {
                    Buffer inn = rq.InNormalBuffers[i];
                    BufferCommandData *bcd = (BufferCommandData*)tls;
                    bcd->Size = inn.Size;
                    uintptr_t uptr = (uintptr_t)inn.Data;
                    bcd->Address = uptr;
                    bcd->Packed = (inn.Info.Type | (((uptr >> 32) & 15) << 28) | ((uptr >> 36) << 2));
                }
                if(!rq.OutNormalBuffers.empty()) for(u32 i = 0; i < rq.OutNormalBuffers.size(); i++, tls += 3)
                {
                    Buffer outn = rq.OutNormalBuffers[i];
                    BufferCommandData *bcd = (BufferCommandData*)tls;
                    bcd->Size = outn.Size;
                    uintptr_t uptr = (uintptr_t)outn.Data;
                    bcd->Address = uptr;
                    bcd->Packed = (outn.Info.Type | (((uptr >> 32) & 15) << 28) | ((uptr >> 36) << 2));
                }
                if(!rq.ExchangeBuffers.empty()) for(u32 i = 0; i < rq.ExchangeBuffers.size(); i++, tls += 3)
                {
                    Buffer ex = rq.ExchangeBuffers[i];
                    BufferCommandData *bcd = (BufferCommandData*)tls;
                    bcd->Size = ex.Size;
                    uintptr_t uptr = (uintptr_t)ex.Data;
                    bcd->Address = uptr;
                    bcd->Packed = (ex.Info.Type | (((uptr >> 32) & 15) << 28) | ((uptr >> 36) << 2));
                }
                u32 pad = (((16 - (((uintptr_t)tls) & 15)) & 15) / 4);
                u32 *raw = (u32*)(tls + pad);
                size_t rawsz = ((rq.InRawSize / 4) + 4);
                tls += rawsz;
                u16 *tls16 = (u16*)tls;
                if(!rq.OutStaticBuffers.empty()) for(u32 i = 0; i < rq.OutStaticBuffers.size(); i++)
                {
                    Buffer outs = rq.OutStaticBuffers[i];
                    size_t outssz = (uintptr_t)outs.Size;
                    tls16[i] = ((outssz > 0xffff) ? 0 : outssz);
                }
                size_t u16s = (((2 * rq.OutStaticBuffers.size()) + 3) / 4);
                tls += u16s;
                rawsz += u16s;
                *fillsz |= rawsz;
                if(!rq.OutStaticBuffers.empty()) for(u32 i = 0; i < rq.OutStaticBuffers.size(); i++, tls += 2)
                {
                    Buffer outs = rq.OutStaticBuffers[i];
                    BufferReceiveData *brd = (BufferReceiveData*)tls;
                    uintptr_t uptr = (uintptr_t)outs.Data;
                    brd->Address = uptr;
                    brd->Packed = ((uptr >> 32) | (outs.Size << 16));
                }
                DomainHeader *iraw = (DomainHeader*)raw;
                iraw->Type = 2;
                iraw->ObjectIdCount = 0;
                iraw->Size = 0;
                iraw->ObjectId = this->objid;
                iraw->Pad[0] = iraw->Pad[1] = 0;
                svc::SendSyncRequest(this->handle);
            }
            else if(this->IsDomain() || this->IsNormal())
            {
                u32 *tls = (u32*)arm::GetThreadLocalStorage();
                tls[0] = 2;
                tls[1] = 0;
                svc::SendSyncRequest(this->handle);
                svc::CloseHandle(this->handle);
            }
        }
        this->handle = 0;
        this->type = ObjectType::Uninitialized;
        this->objid = UINT32_MAX;
    }

    ServiceObject::ServiceObject(const char *Name, u32 ErrorModule)
    {
        this->name = Name;
        this->mod = ErrorModule;
    }

    const char *ServiceObject::GetName()
    {
        return this->name;
    }

    u32 ServiceObject::GetErrorModule()
    {
        return this->mod;
    }
}