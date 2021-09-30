#include <qapplication.h>

#include "paginationtest.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    PaginationTest w;
    w.show();

    return a.exec();
}
