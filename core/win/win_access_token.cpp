#include "win_access_token.h"
#include <cstdlib>

#include <windows.h>
#include <sddl.h>

namespace Core
{
  AccessToken::AccessToken(DWORD desiredAccess, HANDLE token) : CachedClass(0)
  {
    if (token == 0)
    {
      if (!::OpenThreadToken(::GetCurrentThread(), desiredAccess, TRUE, &token))
      {
        if (!::OpenProcessToken(::GetCurrentProcess(), desiredAccess, &token))
        {
          throw std::runtime_error("Win error: " + std::to_string(::GetLastError()));
        }
      }
    }
    TokenHandle.reset(token);
  }

  std::wstring AccessToken::GetSid()
  {
    const TOKEN_USER* user = GetInformation<TOKEN_USER>(TokenUser);
    return ConvertSidToString(user->User.Sid);
  }

  std::wstring AccessToken::GetAccountName()
  {
    const TOKEN_USER* user = GetInformation<TOKEN_USER>(TokenUser);
    return LookupSid(user->User.Sid);
  }

  std::list<std::wstring> AccessToken::GetGroupNames()
  {
    std::list<std::wstring> groupNames;
    const TOKEN_GROUPS* groups = GetInformation<TOKEN_GROUPS>(TokenGroups);
    for (unsigned index = 0; index < groups->GroupCount; ++index)
    {
      groupNames.push_back(LookupSid(groups->Groups[index].Sid));
    }
    return groupNames;
  }

  unsigned AccessToken::GetSessionId()
  {
    return *GetInformation<unsigned>(TokenSessionId);
  }

  AccessToken::ElevationType AccessToken::GetElevationType()
  {
    const int TokenElevation = 20;
    unsigned elevation = *GetInformation<unsigned>(TokenElevation);
    if (elevation == 0)
    {
      return NotElevated;
    }
    const int TokenElevationType = 18;
    unsigned type = *GetInformation<unsigned>(TokenElevationType);
    return static_cast<ElevationType>(type);
  }

  AccessToken::ImpersonationLevel AccessToken::GetImpersonationLevel()
  {
    unsigned tokenType = *GetInformation<unsigned>(TokenType);
    if (tokenType != TokenImpersonation)
    {
      return NotImpersonated;
    }
    unsigned level = *GetInformation<unsigned>(TokenImpersonationLevel);
    return static_cast<ImpersonationLevel>(level + 1);
  }

  std::list<std::wstring> AccessToken::GetEnabledPrivileges()
  {
    std::list<std::wstring> enabledPrivileges;
    const TOKEN_PRIVILEGES* privileges = GetInformation<TOKEN_PRIVILEGES>(TokenPrivileges);
    wchar_t buffer[80] = {};
    for (unsigned k = 0; k < privileges->PrivilegeCount; ++k)
    {
      if ((privileges->Privileges[k].Attributes & (SE_PRIVILEGE_ENABLED | SE_PRIVILEGE_ENABLED_BY_DEFAULT)) != 0)
      {
        DWORD size = ARRAYSIZE(buffer);
        if (!::LookupPrivilegeNameW(0, const_cast<LUID*>(&privileges->Privileges[k].Luid), buffer, &size))
        {
          swprintf(buffer, L"Luid[%08X-%08X]", privileges->Privileges[k].Luid.HighPart, privileges->Privileges[k].Luid.LowPart);
        }
        enabledPrivileges.push_back(buffer);
      }
    }
    return enabledPrivileges;
  }

  template <typename T> 
  const T* AccessToken::GetInformation(int informationClass)
  {
    if (informationClass != CachedClass)
    {
      GetRawInformation(informationClass, CachedBuffer);
      CachedClass = informationClass;
    }
    if (CachedBuffer.size() < sizeof(T))
    {
      throw std::runtime_error("AccessToken error: small token size");
    }
    return reinterpret_cast<T*>(&CachedBuffer.front());
  }

  void AccessToken::GetRawInformation(int informationClass, std::vector<char>& buffer)
  {
    DWORD size = 0;
    if (!::GetTokenInformation(TokenHandle.get(), static_cast<TOKEN_INFORMATION_CLASS>(informationClass), buffer.empty() ? 0 : &buffer.front(), static_cast<DWORD>(buffer.size()), &size))
    {
      if (size > buffer.size())
      {
        buffer.resize(size);
      }
      if (!::GetTokenInformation(TokenHandle.get(), static_cast<TOKEN_INFORMATION_CLASS>(informationClass), buffer.empty() ? 0 : &buffer.front(), static_cast<DWORD>(buffer.size()), &size))
      {
        throw std::runtime_error("Win error: " + std::to_string(::GetLastError()));
      }
    }
    buffer.resize(size);
  }

  std::wstring AccessToken::LookupSid(PSID sid) const
  {
    DWORD nameLength = 0;
    DWORD domainLength = 0;
    SID_NAME_USE use;
    ::LookupAccountSidW(0, sid, 0, &nameLength, 0, &domainLength, &use);

    std::vector<wchar_t> nameBuffer(nameLength + 1);
    std::vector<wchar_t> domainBuffer(domainLength + 1);
    if (!::LookupAccountSidW(0, sid, &nameBuffer.front(), &nameLength, &domainBuffer.front(), &domainLength, &use))
    {
      const DWORD errorCode = ::GetLastError();
      if (errorCode == ERROR_NONE_MAPPED)
      {
        return ConvertSidToString(sid);
      }
      throw std::runtime_error("Win error: " + std::to_string(errorCode));
    }
    return std::wstring(&domainBuffer.front(), domainLength) + L'\\' + std::wstring(&nameBuffer.front(), nameLength);
  }

  std::wstring AccessToken::ConvertSidToString(PSID sid)
  {
    std::wstring textSid;
    WCHAR* systemString = 0;
    if (::ConvertSidToStringSidW(sid, &systemString))
    {
      textSid = std::wstring(systemString);
      ::LocalFree(systemString);
    }
    return textSid;
  }

  DWORD AccessToken::Impersonate()
  {
    return ::ImpersonateLoggedOnUser(TokenHandle.get()) ? ERROR_SUCCESS : ::GetLastError();
  }

  DWORD AccessToken::RevertToSelf()
  {
    return ::RevertToSelf() ? ERROR_SUCCESS : ::GetLastError();
  }

  ImpersonationToUser::ImpersonationToUser(AccessToken::Sptr token) : Token(token)
  {
    Token->Impersonate();
  }

  ImpersonationToUser::~ImpersonationToUser()
  {
    Token->RevertToSelf();
  }
}
