#include "paginationtest.h"

PaginationTest::PaginationTest(QWidget* parent): QWidget(parent) {
    ui.setupUi(this);

    connect(ui.pushButton, &QPushButton::clicked, [&] {
        ui.pagination->setCurrentPage(ui.lineEdit->text().toInt());
    });
}
