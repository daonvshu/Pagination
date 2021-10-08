#include "pagingstyle1.h"

void PagingStyle1::reCacheNumbers() {
    
    curCacheNumbers.clear();

    int pageSize = getPageSize();

    if (pageSize < 10) {
        for (int i = 0; i < pageSize; i++) {
            curCacheNumbers << i + 1;
        }
        return;
    }

    bool leftEllipsisVisible = false;
    bool rightEllipsisVisible = false;

    int leftLimitNumber = 1 + 3; //max display 4 number
    int rightLimitNumber = pageSize - 3;

    //left put 1,2
    curCacheNumbers << 1 << 2;

    int currentLeftEdge = curSelectNumber - 2; //display 2 number side of left
    int currentRightEdge = curSelectNumber + 2;

    //number 3 condition
    if (currentLeftEdge > leftLimitNumber) {
        curCacheNumbers << -1;
        leftEllipsisVisible = true;
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
        rightEllipsisVisible = true;
    } else {
        curCacheNumbers << pageSize - 2;
    }

    //put pageSize-1, pageSize
    curCacheNumbers << pageSize - 1;
    curCacheNumbers << pageSize;

    int minSize = minDisplaySize();
    Q_ASSERT_X(minSize <= 11, "paging style 1", "paging style 1 min size need <= 11");
    if (curCacheNumbers.size() < minSize) {
        int remaining = minSize - curCacheNumbers.size();
        if (leftEllipsisVisible && !rightEllipsisVisible) {
            for (int i = 0; i < remaining; i++) {
                curCacheNumbers.insert(3, curCacheNumbers.at(3) - 1);
            }
        } else if (!leftEllipsisVisible && rightEllipsisVisible) {
            for (int i = 0; i < remaining; i++) {
                curCacheNumbers.insert(curCacheNumbers.size() - 3, curCacheNumbers.at(curCacheNumbers.size() - 4) + 1);
            }
        }
    }
}

int PagingStyle1::minDisplaySize() const {
    return 8;
}
