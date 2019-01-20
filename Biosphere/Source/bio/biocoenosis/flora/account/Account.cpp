#include <bio/biocoenosis/flora/account/Account.hpp>

namespace bio::account
{
    ResultWrap<u32> AccountService::GetUserCount()
    {
        u32 ucount = 0;
        Result rc = this->ProcessRequest<0>(hipc::OutRaw<u32>(ucount));
        return ResultWrap<u32>(rc, ucount);
    }

    ResultWrap<bool> AccountService::GetUserExistence(Uid UserId)
    {
        bool ex = 0;
        Result rc = this->ProcessRequest<1>(hipc::InRaw<Uid>(UserId), hipc::OutRaw<bool>(ex));
        return ResultWrap<bool>(rc, ex);
    }

    ResultWrap<std::vector<Uid>> AccountService::ListAllUsers()
    {
        Uid uids[8] = { 0 };
        memset(uids, 0, sizeof(uids));
        Result rc = this->ProcessRequest<2>(hipc::OutStaticBuffer(uids, (sizeof(Uid) * 8), 0));
        std::vector<Uid> vuids;
        if(rc.IsSuccess()) for(u32 i = 0; i < 8; i++)
        {
            if(!uids[i]) break;
            vuids.push_back(uids[i]);
        }
        return ResultWrap<std::vector<Uid>>(rc, vuids);
    }

    ResultWrap<std::vector<Uid>> AccountService::ListOpenUsers()
    {
        Uid uids[8] = { 0 };
        memset(uids, 0, sizeof(uids));
        Result rc = this->ProcessRequest<3>(hipc::OutStaticBuffer(uids, (sizeof(Uid) * 8), 0));
        std::vector<Uid> vuids;
        if(rc.IsSuccess()) for(u32 i = 0; i < 8; i++)
        {
            if(!uids[i]) break;
            vuids.push_back(uids[i]);
        }
        return ResultWrap<std::vector<Uid>>(rc, vuids);
    }

    ResultWrap<Uid> AccountService::GetLastOpenedUser()
    {
        Uid uid = 0;
        Result rc = this->ProcessRequest<4>(hipc::OutRaw<Uid>(uid));
        return ResultWrap<Uid>(rc, uid);
    }

    ResultWrap<AccountService*> Initialize()
    {
        auto srv = sm::GetService("acc:u0");
        Result rc = srv;
        hipc::Object *osrv = srv.AssertOk();
        return ResultWrap<AccountService*>(rc, static_cast<AccountService*>(osrv));
    }

    ResultWrap<AccountSystemService*> InitializeSystemService()
    {
        auto srv = sm::GetService("acc:u1");
        Result rc = srv;
        hipc::Object *osrv = srv.AssertOk();
        return ResultWrap<AccountSystemService*>(rc, static_cast<AccountSystemService*>(osrv));
    }

    ResultWrap<AccountAdministratorService*> InitializeAdministratorService()
    {
        auto srv = sm::GetService("acc:su");
        Result rc = srv;
        hipc::Object *osrv = srv.AssertOk();
        return ResultWrap<AccountAdministratorService*>(rc, static_cast<AccountAdministratorService*>(osrv));
    }
}