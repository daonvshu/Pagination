#include "pagingstyle2.h"

void PagingStyle2::reCacheNumbers() {

    curCacheNumbers.clear();

    int pageSize = getPageSize();

    if (pageSize < 8) {
        for (int i = 0; i < pageSize; i++) {
            curCacheNumbers << i + 1;
        }
        return;
    }

    bool leftEllipsisVisible = false;
    bool rightEllipsisVisible = false;

    int leftLimitNumber = 1 + 2;
    int rightLimitNumber = pageSize - 2;

    //put 1
    curCacheNumbers << 1;

    int currentLeftEdge = curSelectNumber - 1; //display 1 number side of left
    int currentRightEdge = curSelectNumber + 1;

    if (currentLeftEdge > leftLimitNumber) {
        curCacheNumbers << -1;
        leftEllipsisVisible = true;
    } else {
        curCacheNumbers << 2;
    }

    for (int k = qMax(leftLimitNumber, currentLeftEdge); k <= qMin(rightLimitNumber, currentRightEdge); k++) {
        curCacheNumbers << k;
    }

    if (currentRightEdge < rightLimitNumber) {
        curCacheNumbers << -1;
        rightEllipsisVisible = true;
    } else {
        curCacheNumbers << pageSize - 1;
    }

    curCacheNumbers << pageSize;

    int minSize = minDisplaySize();
    Q_ASSERT_X(minSize <= 7, "paging style 2", "paging style 2 min size need <= 7");
    if (curCacheNumbers.size() < minSize) {
        int remaining = minSize - curCacheNumbers.size();
        if (leftEllipsisVisible && !rightEllipsisVisible) {
            for (int i = 0; i < remaining; i++) {
                curCacheNumbers.insert(2, curCacheNumbers.at(2) - 1);
            }
        } else if (!leftEllipsisVisible && rightEllipsisVisible) {
            for (int i = 0; i < remaining; i++) {
                curCacheNumbers.insert(curCacheNumbers.size() - 2, curCacheNumbers.at(curCacheNumbers.size() - 3) + 1);
            }
        }
    }
}

int PagingStyle2::minDisplaySize() const {
    return 7;
}
