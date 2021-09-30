#pragma once

#include <qwidget.h>

#include "ui_paginationtest.h"

class PaginationTest : public QWidget {
public:
    explicit PaginationTest(QWidget* parent = nullptr);

private:
    Ui::PaginationTest ui;
};