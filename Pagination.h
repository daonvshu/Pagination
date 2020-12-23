#pragma once

#include <qwidget.h>
#include <qpushbutton.h>

class Pagination : public QWidget {
	Q_OBJECT

public:
	Pagination(QWidget* parent = nullptr);
	~Pagination();

	void setTotalSize(int size);
	void setTotalSize(int dataSize, int perSize);
	int currentPage();
    void setCurrent(int btnIndex);

signals:
	void selectPage(int value);

private:
	QPushButton* btn[13];
	int curBtnIndex = 0;
	int btnValue[13];
	int totalSize;
	int lastValue = -1;

private:
	void clicked(int btnIndex);
	void clickedValue(int curValue);
	void gotoPage(int pageValue);

protected:
	void paintEvent(QPaintEvent*) override;
};

