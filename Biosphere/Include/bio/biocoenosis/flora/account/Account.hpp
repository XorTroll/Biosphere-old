
#pragma once
#include <bio/biocoenosis/flora/account/Types.hpp>

namespace bio::account
{
    class AccountService : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<u32> GetUserCount();
            ResultWrap<bool> GetUserExistence(Uid UserId);
            ResultWrap<std::vector<Uid>> ListAllUsers();
            ResultWrap<std::vector<Uid>> ListOpenUsers();
            ResultWrap<Uid> GetLastOpenedUser();
            ResultWrap<Profile*> GetProfile(Uid UserId);
    };

    class AccountSystemService : public hipc::Object
    {
        public:
            using Object::Object;
    };

    class AccountAdministratorService : public hipc::Object
    {
        public:
            using Object::Object;
            ResultWrap<ProfileEditor*> GetProfileEditor(Uid UserId);
    };

    ResultWrap<AccountService*> Initialize();
    ResultWrap<AccountSystemService*> InitializeSystemService();
    ResultWrap<AccountAdministratorService*> InitializeAdministratorService();
}