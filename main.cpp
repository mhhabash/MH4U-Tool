#include "mainwindow.hpp"
#include <QApplication>
#include <iostream>
#include <QtSql>

QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

int main(int argc, char *argv[])
{
    db.setHostName("localhost");
    db.setDatabaseName("MH4U.db");

    if (!db.open())
    {
        std::cout << "Could not open database" << std::endl;
        exit(1);
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
