
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include <memory>

#include "DbHandler.hpp"

class QLabel;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:


private slots:
    void addButtonPress();



private:
    QLabel *infoLabel;
    QPushButton* addButton;
    QPushButton* deleteButton;
    QPushButton* provideButton;
    QLineEdit* urlInput;
    QLabel* geolocOut;

    std::unique_ptr<DbHandler> dbHandler;
};


#endif
