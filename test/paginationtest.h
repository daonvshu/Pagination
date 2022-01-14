#pragma once

#include <qwidget.h>
#include <qstandarditemmodel.h>

#include "ui_paginationtest.h"

class PaginationTest : public QWidget {
    Q_OBJECT

public:
    explicit PaginationTest(QWidget* parent = nullptr);

private:
    Ui::PaginationTest ui;

    QStandardItemModel* model;

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    void on_pushButton_clicked();

    void on_pagination_pageIndexChanged(int index);
    void on_pagination_2_pageIndexChanged(int index);
    void on_pagination_3_pageIndexChanged(int index);

private:
    void reloadListView();
};