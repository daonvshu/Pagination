#pragma once

#include <qframe.h>
#include <qpushbutton.h>

#include "pagingutil.h"

class Pagination : public QFrame {
    Q_OBJECT

    Q_PROPERTY(int spacing READ getBoxSpacing WRITE setBoxSpacing)
    Q_PROPERTY(int totalsize READ getTotalSize WRITE setTotalSize)
    Q_PROPERTY(int sizeofperpage READ getSizeofPerPage WRITE setSizeofPerPage)

    Q_PROPERTY(bool pageUpEnable READ isPageUpEnabled WRITE setPageUpEnable)
    Q_PROPERTY(QString pageUpText READ getPageUpText WRITE setPageUpText DESIGNABLE isPageUpEnabled)

    Q_PROPERTY(bool pageDownEnable READ isPageDownEnabled WRITE setPageDownEnable)
    Q_PROPERTY(QString pageDownText READ getPageDownText WRITE setPageDownText DESIGNABLE isPageDownEnabled)

public:
    explicit Pagination(QWidget* parent = nullptr);

    int getBoxSpacing() const;
    void setBoxSpacing(int spacing);

    int getTotalSize() const;
    void setTotalSize(int size);

    int getSizeofPerPage() const;
    void setSizeofPerPage(int size);

    bool isPageUpEnabled() const;
    void setPageUpEnable(bool enable);

    QString getPageUpText() const;
    void setPageUpText(const QString& text);

    bool isPageDownEnabled() const;
    void setPageDownEnable(bool enable);

    QString getPageDownText() const;
    void setPageDownText(const QString& text);

    void setCurrentPage(int number);

    void pageUp();
    void pageDown();
    void pageFirst();
    void pageLast();

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
    PagingUtil pagingUtil;

    int boxSpacing;

    bool pageUpEnable, pageDownEnable;
    QString pageUpText, pageDownText;
    bool pageUpPressed, pageDownPressed;

    //-3: pageUp -2: pageDown -1: none
    int hoverNumber;

    QPushButton* pushButton;

private:
    QSize getBoxSize() const;

    int getPageUpBtnWidth() const;
    int getPageDownBtnWidth() const;
};