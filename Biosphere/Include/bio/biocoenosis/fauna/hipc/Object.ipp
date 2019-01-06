namespace bio::hipc
{
    template<u32 CommandId, typename ...Arguments>
    Result Object::ProcessRequest(Arguments &&...Args)
    {
        bool domainmode = (this->IsDomain() || this->IsSubService());
        u64 orawsz = 0;
        RequestData rq;
        u32 *tls = (u32*)arm::GetThreadLocalStorage();
        rq.InRawSize += (alignof(u64) - 1);
        rq.InRawSize -= (rq.InRawSize % alignof(u64));
        u64 magicoff = rq.InRawSize;
        rq.InRawSize += sizeof(u64);
        rq.InRawSize += (alignof(u64) - 1);
        rq.InRawSize -= (rq.InRawSize % alignof(u64));
        u64 cmdidoff = rq.InRawSize;
        rq.InRawSize += sizeof(u64);
        this->ProcessArgument(rq, 0, Args...);
        if(domainmode)
        {
            orawsz = rq.InRawSize;
            rq.InRawSize += sizeof(DomainHeader) + (rq.InObjectIds.size() * sizeof(u32));
        }
        *tls++ = (4 | (rq.InStaticBuffers.size() << 16) | (rq.InNormalBuffers.size() << 20) | (rq.OutNormalBuffers.size() << 24) | (rq.ExchangeBuffers.size() << 28));
        u32 *fillsz = tls;
        if(rq.OutStaticBuffers.size() > 0) *tls = ((rq.OutStaticBuffers.size() + 2) << 10);
        else *tls = 0;
        if(rq.InProcessId || (!rq.InCopyHandles.empty()) || (!rq.InMoveHandles.empty()))
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
        void *vraw = (void*)raw;
        if(domainmode)
        {
            DomainHeader *dh = (DomainHeader*)vraw;
            u32 *ooids = (u32*)(((uintptr_t)vraw) + sizeof(DomainHeader) + orawsz);
            dh->Type = 1;
            dh->ObjectIdCount = (u8)rq.InObjectIds.size();
            dh->Size = orawsz;
            dh->ObjectId = this->GetObjectId();
            dh->Pad[0] = dh->Pad[1] = 0;
            if(!rq.InObjectIds.empty()) for(u32 i = 0; i < rq.InObjectIds.size(); i++) ooids[i] = rq.InObjectIds[i];
            vraw = (void*)(((uintptr_t)vraw) + sizeof(DomainHeader));
        }
        rq.InRawData = (u8*)vraw;
        *((u64*)(((u8*)rq.InRawData) + magicoff)) = hipc::SFCI;
        *((u64*)(((u8*)rq.InRawData) + cmdidoff)) = CommandId;
        this->ProcessArgument(rq, 2, Args...);
        Result rc = svc::SendSyncRequest(this->GetHandle());
        if(rc.IsFailure()) return rc;
        u32 *otls = (u32*)arm::GetThreadLocalStorage();
        rq.OutRawSize += (alignof(u64) - 1);
        rq.OutRawSize -= (rq.OutRawSize % alignof(u64));
        rq.OutRawSize += sizeof(u64);
        rq.OutRawSize += (alignof(u64) - 1);
        rq.OutRawSize -= (rq.OutRawSize % alignof(u64));
        u64 resoff = rq.OutRawSize;
        rq.OutRawSize += sizeof(u64);
        this->ProcessArgument(rq, 3, Args...);
        u32 ctrl0 = *otls++;
        u32 ctrl1 = *otls++;
        if(ctrl1 & 0x80000000)
        {
            u32 ctrl2 = *otls++;
            if(ctrl2 & 1)
            {
                u64 pid = *otls++;
                pid |= (((u64)(*otls++)) << 32);
                rq.OutProcessId = pid;
            }
            size_t ohcopy = ((ctrl2 >> 1) & 15);
            size_t ohmove = ((ctrl2 >> 5) & 15);
            size_t oh = (ohcopy + ohmove);
            u32 *aftoh = (otls + oh);
            if(oh > 8) oh = 8;
            if(oh > 0)
            {
                rq.OutHandles.reserve(oh);
                for(u32 i = 0; i < oh; i++)
                {
                    u32 hdl = *(otls + i);
                    rq.OutHandles.push_back(hdl);
                }
            }
            otls = aftoh;
        }
        this->ProcessArgument(rq, 4, Args...);
        size_t nst = ((ctrl0 >> 16) & 15);
        u32 *aftst = otls + (nst * 2);
        if(nst > 8) nst = 8;
        if(nst > 0) for(u32 i = 0; i < nst; i++, otls += 2)
        {
            BufferSendData *bsd = (BufferSendData*)otls;
            BIO_IGNORE(bsd);
        }
        otls = aftst;
        size_t bsend = ((ctrl0 >> 20) & 15);
        size_t brecv = ((ctrl0 >> 24) & 15);
        size_t bexch = ((ctrl0 >> 28) & 15);
        size_t bnum = bsend + brecv + bexch;
        void *ovraw = (void*)(((uintptr_t)(otls + (bnum * 3)) + 15) &~ 15);
        if(bnum > 8) bnum = 8;
        if(bnum > 0) for(u32 i = 0; i < bnum; i++, otls += 3)
        {
            BufferCommandData *bcd = (BufferCommandData*)otls;
            BIO_IGNORE(bcd);
        }       
        if(domainmode)
        {
            DomainResponse *dr = (DomainResponse*)ovraw;
            ovraw = (void*)(((uintptr_t)ovraw) + sizeof(DomainResponse));
            u32 *ooids = (u32*)(((uintptr_t)ovraw) + rq.OutRawSize);
            u32 ooidcount = dr->ObjectIdCount;
            if(ooidcount > 8) ooidcount = 8;
            if(ooidcount > 0)
            {
                rq.OutObjectIds = std::vector<u32>(ooidcount);
                for(u32 i = 0; i < ooidcount; i++) rq.OutObjectIds.push_back(ooids[i]);
            }
        }
        rq.OutRawData = (u8*)ovraw;
        this->ProcessArgument(rq, 5, Args...);
        rc = (u32)(*((u64*)(((u8*)rq.OutRawData) + resoff)));
        return rc;
    }

    template<typename Argument>
    void Object::ProcessArgument(RequestData &Data, u8 Part, Argument &&Arg)
    {
        Arg.Process(Data, Part);
    }

    template<typename Argument, typename ...Arguments>
    void Object::ProcessArgument(RequestData &Data, u8 Part, Argument &&Arg, Arguments &&...Args)
    {
        this->ProcessArgument(Data, Part, Arg);
        this->ProcessArgument(Data, Part, Args...);
    }
}