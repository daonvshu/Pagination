#pragma once

#include <qobject.h>

class PagingUtil : public QObject {
    Q_OBJECT

public:
    PagingUtil(QObject* parent = nullptr);
    virtual ~PagingUtil();

    int getTotalSize() const;
    void setTotalSize(int size);

    int getSizeofPerPage() const;
    void setSizeofPerPage(int size);

    void numberSelected(int number);

    virtual void reCacheNumbers() = 0;

    bool isSelectedNumber(int number) const;

    const QList<int>& getCurCacheNumbers() const {
        return curCacheNumbers;
    }

    int getCurrentSelectedNumber() const {
        return curSelectNumber;
    }

    int getPageSize() const;

signals:
    void numberArraySizeChanged();
    void numbersChanged();
    void numberSelectChanged();

protected:
    //total data size
    int totalSize;
    //max number of pages to display
    int sizeofPerPage;
    //current selected number value
    int curSelectNumber;

    //current display numbers
    QList<int> curCacheNumbers;

private:
    void pre2ReCacheNumbers();
};