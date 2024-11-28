
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

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
};


#endif
