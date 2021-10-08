#include "pagination.h"

#include <qcoreevent.h>
#include <qvariant.h>
#include <qstyle.h>
#include <qpainter.h>
#include <qstyleoption.h>
#include <qevent.h>

Pagination::Pagination(QWidget* parent)
    : QFrame(parent)
    , boxSpacing(0)
    , pageUpEnable(true)
    , pageDownEnable(true)
    , pageUpText("<")
    , pageDownText(">")
    , pageUpPressed(false)
    , pageDownPressed(false)
    , hoverNumber(-1)
    , pushButton(new QPushButton(this))
{
    setAttribute(Qt::WA_Hover);

    pushButton->setVisible(false);

    lastSelectedIndex = pagingUtil.getCurrentSelectedNumber() - 1;

    connect(&pagingUtil, &PagingUtil::numberArraySizeChanged, [&] {
        updateGeometry();
    });

    connect(&pagingUtil, &PagingUtil::numbersChanged, [&] {
        update();
    });

    connect(&pagingUtil, &PagingUtil::numberSelectChanged, [&] {
        auto index = pagingUtil.getCurrentSelectedNumber() - 1;
        if (lastSelectedIndex != index) {
            lastSelectedIndex = index;
            emit pageIndexChanged(index);
        }
    });
}

int Pagination::getBoxSpacing() const {
    return boxSpacing;
}

void Pagination::setBoxSpacing(int spacing) {
    boxSpacing = spacing;
    updateGeometry();
}

int Pagination::getTotalSize() const {
    return pagingUtil.getTotalSize();
}

void Pagination::setTotalSize(int size) {
    pagingUtil.setTotalSize(size);
}

int Pagination::getSizeofPerPage() const {
    return pagingUtil.getSizeofPerPage();
}

void Pagination::setSizeofPerPage(int size) {
    Q_ASSERT(size > 0);
    pagingUtil.setSizeofPerPage(size);
}

bool Pagination::isPageUpEnabled() const {
    return pageUpEnable;
}

void Pagination::setPageUpEnable(bool enable) {
    pageUpEnable = enable;
    updateGeometry();
}

QString Pagination::getPageUpText() const {
    return pageUpText;
}

void Pagination::setPageUpText(const QString& text) {
    pageUpText = text;
    updateGeometry();
}

bool Pagination::isPageDownEnabled() const {
    return pageDownEnable;
}

void Pagination::setPageDownEnable(bool enable) {
    pageDownEnable = enable;
    updateGeometry();
}

QString Pagination::getPageDownText() const {
    return pageDownText;
}

void Pagination::setPageDownText(const QString& text) {
    pageDownText = text;
    updateGeometry();
}



void Pagination::setCurrentPage(int number) {
    pagingUtil.numberSelected(number);
}

void Pagination::pageDown() {
    pagingUtil.numberSelected(pagingUtil.getCurrentSelectedNumber() + 1);
}

void Pagination::pageUp() {
    pagingUtil.numberSelected(pagingUtil.getCurrentSelectedNumber() - 1);
}

void Pagination::pageFirst() {
    pagingUtil.numberSelected(1);
}

void Pagination::pageLast() {
    pagingUtil.numberSelected(pagingUtil.getPageSize());
}



QSize Pagination::sizeHint() const {
    return minimumSizeHint();
}

QSize Pagination::minimumSizeHint() const {

    int minWidth = getPageUpBtnWidth();
    minWidth += boxSpacing;

    auto boxSize = getBoxSize();

    //calc numbers width
    auto numbers = pagingUtil.getCurCacheNumbers();
    for (int i = 0; i < numbers.size(); i++) {
        minWidth += boxSize.width();
        minWidth += boxSpacing;
    }
    minWidth += getPageDownBtnWidth();

    return { minWidth, boxSize.height() };
}

bool Pagination::event(QEvent* e) {
    if (e->type() == QEvent::HoverLeave) {
        hoverNumber = -1;
        update();
        return true;
    }
    if (e->type() == QEvent::HoverMove || e->type() == QEvent::MouseButtonPress || e->type() == QEvent::MouseButtonRelease) {
        QPoint mousePos;
        if (e->type() == QEvent::HoverMove) {
            mousePos = dynamic_cast<QHoverEvent*>(e)->pos();
        } else {
            mousePos = dynamic_cast<QMouseEvent*>(e)->pos();
        }

        auto boxSize = getBoxSize();
        auto top = centerTop(boxSize.height());

        hoverNumber = -1;

        do {
            int pageUpBtnWidth = getPageUpBtnWidth();
            if (pageUpBtnWidth != 0) {
                auto boxRect = QRect(
                    0,
                    top,
                    pageUpBtnWidth,
                    boxSize.height()
                );

                if (boxRect.contains(mousePos)) {
                    if (e->type() == QEvent::MouseButtonPress) {
                        pageUpPressed = true;
                    } else if (e->type() == QEvent::MouseButtonRelease) {
                        pageUpPressed = false;
                        pageUp();
                    }
                    hoverNumber = -3;
                    break;
                }
            }

            auto numbers = pagingUtil.getCurCacheNumbers();

            int pageDownBtnWidth = getPageDownBtnWidth();
            if (pageDownBtnWidth != 0) {
                auto boxRect = QRect(
                    getPageUpBtnWidth() + boxSpacing + (boxSize.width() + boxSpacing) * numbers.size(),
                    top,
                    pageDownBtnWidth,
                    boxSize.height()
                );

                if (boxRect.contains(mousePos)) {
                    if (e->type() == QEvent::MouseButtonPress) {
                        pageDownPressed = true;
                    } else if (e->type() == QEvent::MouseButtonRelease) {
                        pageDownPressed = false;
                        pageDown();
                    }
                    hoverNumber = -2;
                    break;
                }
            }

            for (int i = 0; i < numbers.size(); i++) {

                auto boxRect = QRect(
                    pageUpBtnWidth + boxSpacing + (boxSize.width() + boxSpacing) * i,
                    top,
                    boxSize.width(),
                    boxSize.height()
                );

                if (boxRect.contains(mousePos)) {
                    if (e->type() == QEvent::MouseButtonPress) {
                        setCurrentPage(numbers.at(i));
                    } else {
                        hoverNumber = numbers.at(i);
                    }
                    break;
                }
            }
        } while (false);

        update();
        return true;
    }
    return QFrame::event(e);
}

void Pagination::paintEvent(QPaintEvent* event) {

    QPainter painter(this);

    auto boxSize = getBoxSize();
    auto top = centerTop(boxSize.height());

    auto numbers = pagingUtil.getCurCacheNumbers();

    int pageUpBtnWidth = getPageUpBtnWidth();
    if (pageUpBtnWidth != 0) {
        auto boxRect = QRect(
            0,
            top,
            pageUpBtnWidth,
            boxSize.height()
        );

        drawPageUpBtn(boxRect, &painter);
    }

    for (int i=0; i<numbers.size(); i++) {
        
        auto boxRect = QRect(
            pageUpBtnWidth + boxSpacing + (boxSize.width() + boxSpacing) * i,
            top,
            boxSize.width(),
            boxSize.height()
        );

        int num = numbers.at(i);
        if (num != -1) {
            drawNumber(boxRect, numbers.at(i), &painter);
        } else {
            drawEllipsis(boxRect, numbers.at(i), &painter);
        }
    }

    int pageDownBtnWidth = getPageDownBtnWidth();
    if (pageDownBtnWidth != 0) {
        auto boxRect = QRect(
            getPageUpBtnWidth() + boxSpacing + (boxSize.width() + boxSpacing) * numbers.size(),
            top,
            pageDownBtnWidth,
            boxSize.height()
        );

        drawPageDownBtn(boxRect, &painter);
    }
}


QString Pagination::numberFormat(int number) const {
    if (number == -1) {
        return "...";
    }
    return QString::number(number);
}

void Pagination::drawNumber(const QRect& rect, int number, QPainter* painter) {

    QStyleOptionButton opt;
    opt.initFrom(pushButton);
    opt.text = numberFormat(number);
    opt.rect = rect;
    opt.state.setFlag(QStyle::State_MouseOver, hoverNumber == number);
    opt.state.setFlag(QStyle::State_Selected, pagingUtil.isSelectedNumber(number));
    style()->drawControl(QStyle::CE_PushButton, &opt, painter, pushButton);
}

void Pagination::drawEllipsis(const QRect& rect, int number, QPainter* painter) {

    QStyleOptionButton opt;
    opt.initFrom(pushButton);
    opt.text = numberFormat(number);
    opt.rect = rect;
    opt.state.setFlag(QStyle::State_Enabled, false);
    style()->drawControl(QStyle::CE_PushButton, &opt, painter, pushButton);
}

void Pagination::drawPageUpBtn(const QRect& rect, QPainter* painter) {

    QStyleOptionButton opt;
    opt.initFrom(pushButton);
    opt.text = getPageUpText();
    opt.rect = rect;
    opt.state.setFlag(QStyle::State_MouseOver, hoverNumber == -3);
    opt.state.setFlag(QStyle::State_Sunken, pageUpPressed);
    style()->drawControl(QStyle::CE_PushButton, &opt, painter, pushButton);
}

void Pagination::drawPageDownBtn(const QRect& rect, QPainter* painter) {

    QStyleOptionButton opt;
    opt.initFrom(pushButton);
    opt.text = getPageDownText();
    opt.rect = rect;
    opt.state.setFlag(QStyle::State_MouseOver, hoverNumber == -2);
    opt.state.setFlag(QStyle::State_Sunken, pageDownPressed);
    style()->drawControl(QStyle::CE_PushButton, &opt, painter, pushButton);
}


QSize Pagination::getBoxSize() const {
    auto met = fontMetrics();

    int l, t, r, b;
    getContentsMargins(&l, &t, &r, &b);

    int boxWidth = l + r + met.height();
    int boxHeight = t + b + met.height();

    return { boxWidth, boxHeight };
}

int Pagination::getPageUpBtnWidth() const {
    if (!pageUpEnable) {
        return 0;
    }

    int l, t, r, b;
    getContentsMargins(&l, &t, &r, &b);

    auto met = fontMetrics();
    return qMax(t + b + met.height(), met.width(pageUpText) + l + r);
}

int Pagination::getPageDownBtnWidth() const {
    if (!pageDownEnable) {
        return 0;
    }

    int l, t, r, b;
    getContentsMargins(&l, &t, &r, &b);

    auto met = fontMetrics();
    return qMax(t + b + met.height(), met.width(pageDownText) + l + r);
}

int Pagination::centerTop(int boxHeight) const {
    return (height() - boxHeight) / 2;
}
