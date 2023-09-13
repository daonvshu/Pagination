#include "pagination.h"

#include "pagingstyle1.h"

#include <qcoreevent.h>
#include <qvariant.h>
#include <qstyle.h>
#include <qpainter.h>
#include <qstyleoption.h>
#include <qevent.h>

Pagination::Pagination(QWidget* parent)
    : QFrame(parent)
    , pagingStyle(nullptr)
    , lastSelectedIndex(0)
    , boxSpacing(6)
    , pageUpText("<")
    , pageDownText(">")
    , pageUpPressed(false)
    , pageDownPressed(false)
    , hoverNumber(-1)
    , pushButton(new QPushButton(this))
{
    setAttribute(Qt::WA_Hover);

    pushButton->setVisible(false);

    setPagingStyle(new PagingStyle1);
    pagingStyle->pre2ReCacheNumbers();
}

Pagination::~Pagination() {
    delete pagingStyle;
}

int Pagination::getBoxSpacing() const {
    return boxSpacing;
}

void Pagination::setBoxSpacing(int spacing) {
    boxSpacing = spacing;
    updateGeometry();
}

int Pagination::getTotalSize() const {
    return pagingStyle->getTotalSize();
}

void Pagination::setTotalSize(int size) {
    pagingStyle->setTotalSize(size);
}

int Pagination::getSizeofPerPage() const {
    return pagingStyle->getSizeofPerPage();
}

void Pagination::setSizeofPerPage(int size) {
    Q_ASSERT(size > 0);
    pagingStyle->setSizeofPerPage(size);
}

bool Pagination::isPageUpEnabled() const {
    return !pageUpText.isEmpty();
}

QString Pagination::getPageUpText() const {
    return pageUpText;
}

void Pagination::setPageUpText(const QString& text) {
    pageUpText = text;
    updateGeometry();
}

bool Pagination::isPageDownEnabled() const {
    return !pageDownText.isEmpty();
}

QString Pagination::getPageDownText() const {
    return pageDownText;
}

void Pagination::setPageDownText(const QString& text) {
    pageDownText = text;
    updateGeometry();
}


void Pagination::setPagingStyle(PagingUtil* pagingStyle) {
    //copy data from old style
    if (this->pagingStyle != nullptr) {
        *pagingStyle = *this->pagingStyle;
    }
    //remove old style util
    delete this->pagingStyle;
    this->pagingStyle = pagingStyle;
    this->pagingStyle->pre2ReCacheNumbers();

    connect(pagingStyle, &PagingUtil::numberArraySizeChanged, [&] {
        updateGeometry();
    });

    connect(pagingStyle, &PagingUtil::numbersChanged, [&] {
        update();
    });

    connect(pagingStyle, &PagingUtil::numberSelectChanged, [&] {
        auto index = this->pagingStyle->getCurrentSelectedNumber() - 1;
        if (lastSelectedIndex != index) {
            lastSelectedIndex = index;
            emit pageIndexChanged(index);
        }
    });

    lastSelectedIndex = pagingStyle->getCurrentSelectedNumber() - 1;
}

void Pagination::setCurrentPage(int number) {
    pagingStyle->numberSelected(number);
}

void Pagination::pageDown() {
    pagingStyle->numberSelected(pagingStyle->getCurrentSelectedNumber() + 1);
}

void Pagination::pageUp() {
    pagingStyle->numberSelected(pagingStyle->getCurrentSelectedNumber() - 1);
}

void Pagination::pageFirst() {
    pagingStyle->numberSelected(1);
}

void Pagination::pageLast() {
    pagingStyle->numberSelected(pagingStyle->getPageSize());
}

int Pagination::getCurrentPageIndex() const {
    return lastSelectedIndex;
}


QSize Pagination::sizeHint() const {
    return minimumSizeHint();
}

QSize Pagination::minimumSizeHint() const {

    int minWidth = getPageUpBtnWidth();
    minWidth += boxSpacing;

    auto boxSize = getBoxSize();

    //calc numbers width
    auto numbers = pagingStyle->getCurCacheNumbers();
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            mousePos = dynamic_cast<QHoverEvent*>(e)->pos();
#else
            mousePos = dynamic_cast<QHoverEvent*>(e)->position().toPoint();
#endif
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

            auto numbers = pagingStyle->getCurCacheNumbers();

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

    auto numbers = pagingStyle->getCurCacheNumbers();

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
    opt.state.setFlag(QStyle::State_Selected, pagingStyle->isSelectedNumber(number));
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
    readContentMargins(l, t, r, b);

    int boxWidth = l + r + met.height();
    int boxHeight = t + b + met.height();

    return { boxWidth, boxHeight };
}

int Pagination::getPageUpBtnWidth() const {
    if (!isPageUpEnabled()) {
        return 0;
    }

    int l, t, r, b;
    readContentMargins(l, t, r, b);

    auto met = fontMetrics();
    return qMax(t + b + met.height(),
#if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
                met.width(pageUpText) + l + r
#else
                met.horizontalAdvance(pageUpText) + l + r
#endif
                );
}

int Pagination::getPageDownBtnWidth() const {
    if (!isPageDownEnabled()) {
        return 0;
    }

    int l, t, r, b;
    readContentMargins(l, t, r, b);

    auto met = fontMetrics();
    return qMax(t + b + met.height(),
#if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
                met.width(pageDownText) + l + r
#else
                met.horizontalAdvance(pageDownText) + l + r
#endif
                );
}

int Pagination::centerTop(int boxHeight) const {
    return (height() - boxHeight) / 2;
}

void Pagination::readContentMargins(int& l, int& t, int& r, int& b) const {
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    getContentsMargins(&l, &t, &r, &b);
#else
    auto margins = contentsMargins();
    l = margins.left();
    t = margins.top();
    r = margins.right();
    b = margins.bottom();
#endif
}
