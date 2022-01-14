#include "paginationtest.h"

#include <qlabel.h>

#include "pagingstyle2.h"

PaginationTest::PaginationTest(QWidget* parent): QWidget(parent) {
    ui.setupUi(this);

    for (int i=0; i<23; i++) {
        auto widget = new QWidget(this);
        new QLabel(QString("this is page %1").arg(i + 1), widget);
        ui.page_container->addWidget(widget);
    }

    ui.pagination_4->setPagingStyle(new PagingStyle2);

    ui.listView->setModel(model = new QStandardItemModel(this));
}

void PaginationTest::showEvent(QShowEvent* event) {
    reloadListView();
}

void PaginationTest::on_pushButton_clicked() {
    ui.pagination->setCurrentPage(ui.lineEdit->text().toInt());
}

void PaginationTest::on_pagination_pageIndexChanged(int index) {
    ui.pagination_2->setCurrentPage(index + 1);
    ui.pagination_3->setCurrentPage(index + 1);
}

void PaginationTest::on_pagination_2_pageIndexChanged(int index) {
    ui.pagination->setCurrentPage(index + 1);
    ui.pagination_3->setCurrentPage(index + 1);
}

void PaginationTest::on_pagination_3_pageIndexChanged(int index) {
    ui.pagination_2->setCurrentPage(index + 1);
    ui.pagination->setCurrentPage(index + 1);

    ui.page_container->setCurrentIndex(index);

    ui.pagination->setTotalSize(46);
    ui.pagination_2->setTotalSize(46);
    ui.pagination_3->setTotalSize(46);

    reloadListView();
}

void PaginationTest::reloadListView() {
    model->clear();

    int currentPageIndex = ui.pagination_3->getCurrentPageIndex();
    int pageOffset = currentPageIndex * ui.pagination_3->getSizeofPerPage();

    for (int i=0; i<ui.pagination_3->getSizeofPerPage(); i++) {
        model->appendRow(new QStandardItem(QString("item %1").arg(pageOffset + i + 1)));
    }
}
