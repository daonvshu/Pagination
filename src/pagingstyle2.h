#pragma once

#include "pagingutil.h"

class PagingStyle2 : public PagingUtil {
public:
    using PagingUtil::PagingUtil;

    void reCacheNumbers() override;

    virtual int minDisplaySize() const;
};