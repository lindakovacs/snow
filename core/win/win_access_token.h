#pragma once

#include "win_handle.h"
#include <list>
#include <string>
#include <vector>

namespace Core
{
  class AccessToken
  {
  public:
    typedef std::shared_ptr<AccessToken> Sptr;

    enum ElevationType
    {
      NotElevated,
      DefaultElevation,
      FullElevation,
      LimitedElevation,
    };

    enum ImpersonationLevel
    {
      NotImpersonated,
      Anonymous,
      Identification,
      Impersonation,
      Delegation,
    };

    AccessToken(DWORD desiredAccess = TOKEN_QUERY, HANDLE token = 0); // see TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY

    std::wstring GetSid();
    std::wstring GetAccountName();
    std::list<std::wstring> GetGroupNames();
    unsigned GetSessionId();
    ElevationType GetElevationType();
    ImpersonationLevel GetImpersonationLevel();
    std::list<std::wstring> GetEnabledPrivileges();

    DWORD Impersonate();
    DWORD RevertToSelf();

    static std::wstring ConvertSidToString(PSID sid);

  private:
    HandleUptr TokenHandle;
    int CachedClass;
    std::vector<char> CachedBuffer;

    template <typename T> const T* GetInformation(int information_class);
    void GetRawInformation(int information_class, std::vector<char>& buffer);
    std::wstring LookupSid(PSID sid) const;
  };

  class ImpersonationToUser
  {
  public:
    ImpersonationToUser(AccessToken::Sptr token);
    ~ImpersonationToUser();

  private:
    AccessToken::Sptr Token;
  };
}
