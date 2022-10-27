#pragma once

#include <qframe.h>
#include <qpushbutton.h>

#include "pagingutil.h"

class Pagination : public QFrame {
    Q_OBJECT

    Q_PROPERTY(int spacing READ getBoxSpacing WRITE setBoxSpacing)
    Q_PROPERTY(int totalsize READ getTotalSize WRITE setTotalSize)
    Q_PROPERTY(int sizeofperpage READ getSizeofPerPage WRITE setSizeofPerPage)

    Q_PROPERTY(QString pageUpText READ getPageUpText WRITE setPageUpText)
    Q_PROPERTY(QString pageDownText READ getPageDownText WRITE setPageDownText)

public:
    explicit Pagination(QWidget* parent = nullptr);
    ~Pagination();

    int getBoxSpacing() const;
    void setBoxSpacing(int spacing);

    int getTotalSize() const;
    void setTotalSize(int size);

    int getSizeofPerPage() const;
    void setSizeofPerPage(int size);

    bool isPageUpEnabled() const;

    QString getPageUpText() const;
    void setPageUpText(const QString& text);

    bool isPageDownEnabled() const;

    QString getPageDownText() const;
    void setPageDownText(const QString& text);

    void setPagingStyle(PagingUtil* pagingStyle);

    void setCurrentPage(int number);

    void pageUp();
    void pageDown();
    void pageFirst();
    void pageLast();

    int getCurrentPageIndex() const;

signals:
    void pageIndexChanged(int index);

protected:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    bool event(QEvent* e) override;

    void paintEvent(QPaintEvent* event) override;

    virtual QString numberFormat(int number) const;

    virtual void drawNumber(const QRect& rect, int number, QPainter* painter);
    virtual void drawEllipsis(const QRect& rect, int number, QPainter* painter);

    virtual void drawPageUpBtn(const QRect& rect, QPainter* painter);
    virtual void drawPageDownBtn(const QRect& rect, QPainter* painter);

protected:
    PagingUtil* pagingStyle;

    int lastSelectedIndex;

    int boxSpacing;

    QString pageUpText, pageDownText;
    bool pageUpPressed, pageDownPressed;

    //-3: pageUp -2: pageDown -1: none
    int hoverNumber;

    QPushButton* pushButton;

private:
    QSize getBoxSize() const;

    int getPageUpBtnWidth() const;
    int getPageDownBtnWidth() const;

    int centerTop(int boxHeight) const;

    void readContentMargins(int& l, int& t, int& r, int& b) const;
};