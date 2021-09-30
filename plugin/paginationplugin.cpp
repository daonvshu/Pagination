#include "paginationplugin.h"

#include "pagination.h"

#include <QtCore/QtPlugin>

PaginationPlugin::PaginationPlugin(QObject* parent)
    : QObject(parent) {
    initialized = false;
}

void PaginationPlugin::initialize(QDesignerFormEditorInterface* /*core*/) {
    if (initialized)
        return;

    initialized = true;
}

bool PaginationPlugin::isInitialized() const {
    return initialized;
}

QWidget* PaginationPlugin::createWidget(QWidget* parent) {
    return new Pagination(parent);
}

QString PaginationPlugin::name() const {
    return "Pagination";
}

QString PaginationPlugin::group() const {
    return "Display Widgets";
}

QIcon PaginationPlugin::icon() const {
    return QIcon(":/res/pagination.png");
}

QString PaginationPlugin::toolTip() const {
    return QString();
}

QString PaginationPlugin::whatsThis() const {
    return QString();
}

bool PaginationPlugin::isContainer() const {
    return false;
}

QString PaginationPlugin::domXml() const {
    return "<widget class=\"Pagination\" name=\"pagination\">\n"
        " <property name=\"geometry\">\n"
        "  <rect>\n"
        "   <x>0</x>\n"
        "   <y>0</y>\n"
        "   <width>20</width>\n"
        "   <height>20</height>\n"
        "  </rect>\n"
        " </property>\n"
        "</widget>\n";
}

QString PaginationPlugin::includeFile() const {
    return "Pagination.h";
}