#include <QtWidgets>

#include <sstream>

#include "mainwindow.h"
#include "InputHelper.hpp"
#include "CurlWrapper.hpp"

MainWindow::MainWindow() {
    auto *centerWidget = new QWidget;
    this->setCentralWidget(centerWidget);

    infoLabel = new QLabel(
        tr("Application provides geolocation data based on URL/IP address provided. Checks it's database first and "
           "if it's not there it fetches it from https://ipstack.com/ and stores."));
    infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setWordWrap(true);

    addButton = new QPushButton();
    addButton->setText("Add geo data");
    QPushButton::connect(addButton, &QAbstractButton::pressed, this, &MainWindow::addButtonPress);

    deleteButton = new QPushButton();
    deleteButton->setText("Remove geo data");
    QPushButton::connect(deleteButton, &QAbstractButton::pressed, this, &MainWindow::deleteButtonPress);

    provideButton = new QPushButton();
    provideButton->setText("Get geo data");
    QPushButton::connect(provideButton, &QAbstractButton::pressed, this, &MainWindow::provideButtonPress);

    urlInput = new QLineEdit;

    geolocOut = new QLabel;
    geolocOut->setText("Text");
    geolocOut->setAlignment(Qt::AlignCenter);
    geolocOut->setFrameStyle(1u);

    auto *mainLayout = new QVBoxLayout;
    auto *controlWidget = new QWidget;
    centerWidget->setLayout(mainLayout);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(controlWidget);

    auto *bottomLayout = new QHBoxLayout;
    controlWidget->setLayout(bottomLayout);

    auto *leftWidget = new QWidget;
    auto *rightWidget = new QWidget;
    bottomLayout->addWidget(leftWidget);
    bottomLayout->addWidget(rightWidget);

    auto *vLeftLayout = new QVBoxLayout;
    vLeftLayout->setContentsMargins(5, 5, 5, 5);
    vLeftLayout->addWidget(addButton);
    vLeftLayout->addWidget(deleteButton);
    vLeftLayout->addWidget(provideButton);
    leftWidget->setLayout(vLeftLayout);

    auto *vRightLayout = new QVBoxLayout;
    vRightLayout->setContentsMargins(5, 5, 5, 5);
    vRightLayout->addWidget(urlInput);
    vRightLayout->addWidget(geolocOut);
    rightWidget->setLayout(vRightLayout);

    QString message = tr("Maybe needed later");
    this->statusBar()->showMessage(message);

    this->setWindowTitle(tr(MAIN_WINDOW));
    this->resize(640, 480);

    // TODO: make it smarter
    dbHandler = std::make_unique<DbHandler>();
    ipStackWrapper = std::make_unique<IpStackWrapper>(std::make_unique<CurlWrapper>());

}

void MainWindow::addButtonPress() {
    auto input = urlInput->text().toStdString();
    auto [valid, ipv4] = InputHelper::resolveUrl(input);
    std::stringstream infoMsg{}, outMsg{};
    if (valid) {
        infoMsg << "Domain is valid and IPv4 is: " << ipv4 << "\n";
        auto [obtained, longitude, latitude] = ipStackWrapper->getGeoData(ipv4);
        if(obtained)
        {
            if (!dbHandler->insertGeoLocData({input, ipv4, longitude, latitude})) {
                infoMsg << "Failed to insert data into database";
            }
            outMsg << "Domain: " << InputHelper::extractDomain(input) << "\nIPv4: " << ipv4 << "\nLongitude: " <<
                   longitude << "\nLatitude: " << latitude;
        }
        else
        {
            infoMsg << "Failed to fetch geolocation data from ipstack.com";
        }
    } else {
        infoMsg << "Domain is invalid or not reachable\n";
    }
    infoLabel->setText(QString(infoMsg.str().c_str()));
    geolocOut->setText(QString(outMsg.str().c_str()));
}

void MainWindow::deleteButtonPress() {
    auto input = urlInput->text().toStdString();
    auto [valid, ipv4] = InputHelper::resolveUrl(input);
    std::stringstream infoMsg{};

    if (valid) {
        if (dbHandler->isExistingGeoLocData(ipv4)) {
            if (dbHandler->removeGeoLocData(ipv4)) {
                infoMsg << "Removed database entry for IP: " << ipv4;
            } else {
                infoMsg << "Failed to remove database entry for IP: " << ipv4;
            }
        } else {
            infoMsg << "Domain not present in the database";
        }
    } else {
        infoMsg << "Domain invalid";
    }

    infoLabel->setText(QString(infoMsg.str().c_str()));
}

void MainWindow::provideButtonPress() {
    auto input = urlInput->text().toStdString();
    auto [valid, ipv4] = InputHelper::resolveUrl(input);
    std::stringstream infoMsg{}, outMsg{};

    if (valid) {
        if (dbHandler->isExistingGeoLocData(ipv4)) {
            auto [found, item] = dbHandler->fetchGeoLocData(ipv4);
            if (found) {
                infoMsg << "Found database entry for IP: " << ipv4;
                outMsg << "Domain: " << item.getUrl() << "\nIPv4: " << item.getIp() << "\nLongitude: " <<
                       item.getLongitude() << "\nLatitude: " << item.getLatitude();
            }
        } else {
            infoMsg << "No entry for IP: " << ipv4 << "\nFetching from outside\n";
            auto [obtained, longitude, latitude] = ipStackWrapper->getGeoData(ipv4);
            if(obtained)
            {
                if (!dbHandler->insertGeoLocData({input, ipv4, longitude, latitude})) {
                    infoMsg << "Failed to insert data into database";
                }
                outMsg << "Domain: " << InputHelper::extractDomain(input) << "\nIPv4: " << ipv4 << "\nLongitude: " <<
                       longitude << "\nLatitude: " << latitude;
            }
            else
            {
                infoMsg << "Failed to fetch geolocation data from ipstack.com";
            }
            outMsg << "Domain: " << InputHelper::extractDomain(input) << "\nIPv4: " << ipv4 << "\nLongitude: " <<
                   longitude << "\nLatitude: " << latitude;
        }

    } else {
        infoMsg << "Domain invalid";
    }

    infoLabel->setText(QString(infoMsg.str().c_str()));
    geolocOut->setText(QString(outMsg.str().c_str()));
}

