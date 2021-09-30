#include "pagingutil.h"

PagingUtil::PagingUtil(QObject* parent)
    : QObject(parent)
    , totalSize(0)
    , sizeofPerPage(1)
    , curSelectNumber(1)
{
    
}

int PagingUtil::getTotalSize() const {
    return totalSize;
}

void PagingUtil::setTotalSize(int size) {
    totalSize = size;
    reCacheNumbers();
}

int PagingUtil::getSizeofPerPage() const {
    return sizeofPerPage;
}

void PagingUtil::setSizeofPerPage(int size) {
    Q_ASSERT(size > 0);
    sizeofPerPage = size;
    reCacheNumbers();
}


void PagingUtil::numberSelected(int number) {
    if (number < 1) {
        return;
    }
    if (number > getPageSize()) {
        return;
    }

    curSelectNumber = number;
    reCacheNumbers();
    emit numberSelectChanged();
}

void PagingUtil::reCacheNumbers() {
    int oldNumberSize = curCacheNumbers.size();
    curCacheNumbers.clear();

    int pageSize = getPageSize();

    if (pageSize < 10) {
        for (int i = 0; i < pageSize; i++) {
            curCacheNumbers << i + 1;
        }
        return;
    }

    int leftLimitNumber = 1 + 3; //max display 4 number
    int rightLimitNumber = pageSize - 3;

    //left put 1,2
    curCacheNumbers << 1 << 2;
    
    int currentLeftEdge = curSelectNumber - 2; //display 2 number side of left
    int currentRightEdge = curSelectNumber + 2;

    //number 3 condition
    if (currentLeftEdge > leftLimitNumber) {
        curCacheNumbers << -1;
    } else {
        curCacheNumbers << 3;
    }

    //place 2 number to current selected number side of left and right
    for (int k = qMax(leftLimitNumber, currentLeftEdge); k <= qMin(rightLimitNumber, currentRightEdge); k++) {
        curCacheNumbers << k;
    }

    //number pageSize - 2 condition
    if (currentRightEdge < rightLimitNumber) {
        curCacheNumbers << -1;
    } else {
        curCacheNumbers << pageSize - 2;
    }

    //put pageSize-1, pageSize
    curCacheNumbers << pageSize - 1;
    curCacheNumbers << pageSize;

    if (curCacheNumbers.size() != oldNumberSize) {
        emit numberArraySizeChanged();
    }

    emit numbersChanged();

    if (curSelectNumber > pageSize) {
        curSelectNumber = pageSize;
        emit numberSelectChanged();
    }
}

bool PagingUtil::isSelectedNumber(int number) const {
    return curSelectNumber == number;
}

int PagingUtil::getPageSize() const {
    int pageSize = 1;
    if (totalSize != 0) {
        pageSize = (totalSize - 1) / sizeofPerPage + 1;
    }
    return pageSize;
}
