
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include <memory>

#include "DbHandler.hpp"
#include "IpStackWrapper.hpp"

class QLabel;

class MainWindow : public QMainWindow {
 Q_OBJECT

 public:
    MainWindow();

 protected:

 private slots:
    void addButtonPress();
    void deleteButtonPress();
    void provideButtonPress();
    void submitApiKey(const QString &);
    void clearApiKey();

 private:
    QLabel *infoLabel;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *provideButton;
    QLineEdit *urlInput;
    QLabel *geolocOut;
    QInputDialog* apiKey;

    std::unique_ptr<DbHandler> dbHandler;
    std::unique_ptr<IpStackWrapper> ipStackWrapper;
};

#endif
