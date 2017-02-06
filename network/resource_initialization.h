#pragma once

#include <memory>

namespace Network
{
  class ResourceInitialization
  {
  public:
    typedef std::shared_ptr<ResourceInitialization> Sptr;
    virtual ~ResourceInitialization() {}
  };

  ResourceInitialization::Sptr CreateResourceInitialization();
}
