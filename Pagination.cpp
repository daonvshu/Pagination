#include "Pagination.h"
#include <qstyleoption.h>
#include <qpainter.h>
#include <qboxlayout.h>
#include <QMouseEvent>
#include <qapplication.h>
#include <qregexp.h>
#include <qlineedit.h>

#define USE_GOTO_PAGE

Pagination::Pagination(QWidget* parent) : QWidget(parent) {
	setStyleSheet("Pagination{background:#b4b4b4;border:none;border-radius:5px;}"
		".QPushButton{background:transparent;border:none;font-size:12px;} .QPushButton:hover{background:#bfbfbf;}");
	setMinimumHeight(30);

	auto hbox = new QHBoxLayout(this);
	hbox->setContentsMargins(6, 0, 6, 0);
	hbox->setSpacing(0);

	auto preview = new QPushButton(u8"上一页", this);
	preview->setMinimumSize(45, 24);
	hbox->addWidget(preview);
	connect(preview, &QPushButton::clicked, [&](){
		if (curBtnIndex == 0)
			return;
		int offset = 1;
		if (curBtnIndex - offset == 2 || curBtnIndex - offset == 10) {
			offset++;
		}
		if (!btn[curBtnIndex - offset]->isVisible()) {
			offset++;
		}
		clicked(curBtnIndex - offset);
	});

	for (int i = 0; i < 13; i++) {
		btn[i] = new QPushButton(this);
		btn[i]->setMinimumSize(24, 24);
		hbox->addWidget(btn[i]);

		if (i != 2 && i != 10) {
			connect(btn[i], &QPushButton::clicked, [=]() {
				clicked(i);
			});
		} else {
			btn[i]->setStyleSheet(".QPushButton:hover{background:transparent;}");//去掉省略号hover状态
		}
	}

	auto next = new QPushButton(u8"下一页", this);
	next->setMinimumSize(45, 24);
	hbox->addWidget(next);
	connect(next, &QPushButton::clicked, [&]() {
		if (totalSize >= 10 && curBtnIndex + 1 > 12) {
			return;
		} else if (totalSize > 2 && totalSize < 10 && curBtnIndex + 1 > totalSize) {
			return;
		} else if (totalSize < 3 && curBtnIndex + 1 > totalSize - 1) {
			return;
		}
		
		int offset = 1;
		if (!btn[curBtnIndex + offset]->isVisible()) {
			offset++;
		}
		if (curBtnIndex + offset == 2 || curBtnIndex + offset == 10) {
			offset++;
		}
		clicked(curBtnIndex + offset);
	});

#ifdef USE_GOTO_PAGE
	auto pageEditor = new QLineEdit(this);
	pageEditor->setStyleSheet("border:1px solid #636363;background:transparent;margin-left:8px;padding-left:2px;padding-rigth:2px;");
	pageEditor->setMinimumWidth(38);
	pageEditor->setMaximumWidth(38);
	pageEditor->setValidator(new QRegExpValidator(QRegExp("\\d{3}"), this));
	hbox->addWidget(pageEditor);

	auto gotoBtn = new QPushButton("Go", this);
	gotoBtn->setMinimumSize(32, 24);
	hbox->addWidget(gotoBtn);
	connect(gotoBtn, &QPushButton::clicked, [=]() {
		auto page = pageEditor->text().toInt();
		gotoPage(qMin(page, totalSize));
	});
#endif // USE_GOTO_PAGE

	btnValue[0] = 1;
	btnValue[1] = 2;
	for (int i = 3; i <= 9; i++) {
		btnValue[i] = i;
	}

	btn[2]->setText("...");
	btn[10]->setText("...");
}


Pagination::~Pagination() {
}

void Pagination::setTotalSize(int size) {
	this->totalSize = size;
	btnValue[11] = totalSize - 1;
	btnValue[12] = totalSize;

	for (int i = 0; i < 13; i++) {
		btn[i]->setVisible(true);
	}

	clicked(0);
}

void Pagination::clicked(int btnIndex) {
	int curValue = btnValue[btnIndex];//选中值
	//强制取消hover状态
	QMouseEvent event(QEvent::Leave, QPointF(), Qt::LeftButton, 0, 0);
	QApplication::sendEvent(btn[btnIndex], &event);
	clickedValue(curValue);
}

void Pagination::clickedValue(int curValue) {
	int v6 = btnValue[6];//第7个控件当前值，中间值
	bool b2Visiable = btn[2]->isVisible();
	bool b10Visiable = btn[10]->isVisible();

	bool li = curValue > v6 && b10Visiable;//右省略显示，点击值大于中间值
	bool ri = curValue < v6 && b2Visiable;//左省略显示，点击值小于中间值
	if ((li || ri) && totalSize > 10) {
		btnValue[6] = ri ? qMax(curValue, 6) : qMin(curValue, totalSize - 5);
		for (int i = 3; i <= 9; i++) {//对中间7个控件值重新排列
			btnValue[i] = btnValue[6] + i - 6;
		}
	}

	if (totalSize >= 10) {
		v6 = btnValue[6];
		btn[2]->setVisible(curValue >= 7);
		btn[3]->setVisible(curValue < 7);
		btn[4]->setVisible(curValue <= v6);
		btn[5]->setVisible(curValue <= v6 + 1);
		btn[6]->setVisible(curValue >= v6 - 2 && curValue <= v6 + 2);
		btn[7]->setVisible(curValue >= v6 - 1);
		btn[8]->setVisible(curValue >= v6 || curValue + 5 >= totalSize);//totalSize=10时的处理
		btn[9]->setVisible(curValue + 6 > qMax(totalSize, 11) && totalSize > 10);
		btn[10]->setVisible(curValue + 6 <= totalSize);
	} else {//总数小于10直接全部显示
		for (int i = 0; i < 13; i++) {
			if (i < 2) {
				btn[i]->setVisible(i + 1 <= totalSize);
			} else if (i >= 3 && i <= 9) {
				btn[i]->setVisible(i <= totalSize);
			} else {
				btn[i]->setVisible(false);
				btnValue[i] = -1;//避免当前选择的按钮是隐藏的按钮
			}
		}
	}

	for (int i = 0; i < 13; i++) {
		if (i == 2 || i == 10)
			continue;
		btn[i]->setText(QString::number(btnValue[i]));
		if (btnValue[i] == curValue) {
			curBtnIndex = i;//保存当前点击的按钮index
			btn[i]->setStyleSheet("color:white;");
		} else {
			btn[i]->setStyleSheet("color:black;");
		}
	}

	if (lastValue != curValue) {//避免发送重复值
		lastValue = curValue;
		emit selectPage(curValue);
	}
}

void Pagination::gotoPage(int pageValue) {
	if (pageValue == 0)
		return;
	if (totalSize > 10) {
		btnValue[6] = qMax(6, qMin(pageValue, totalSize - 5));
		for (int i = 3; i <= 9; i++) {//对中间7个控件值重新排列
			btnValue[i] = btnValue[6] + i - 6;
		}
	}
	clickedValue(pageValue);
}

void Pagination::paintEvent(QPaintEvent *) {
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
