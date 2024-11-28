// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtWidgets>

#include "mainwindow.h"

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

    provideButton = new QPushButton();
    provideButton->setText("Get geo data");

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

    {
        // TODO: Database handler playground
        dbHandler = std::make_unique<DbHandler>();
        dbHandler->insertGeoLocData({"test.pl", "1.2.3.4", "50", "18"});
        dbHandler->insertGeoLocData({"test.com", "2.3.4.5", "51", "19"});
        dbHandler->insertGeoLocData({"abc.info", "4.3.2.1", "52", "20"});
        dbHandler->insertGeoLocData({"xyz.net", "7.6.5.4", "53", "21"});

        auto item = dbHandler->fetchGeoLocData("7.6.5.4");
        qInfo() << "Fetched: URL: " << item.getUrl() << ", IP: " << item.getIp() << ", Long: " << item.getLongitude()
                << ", Lati: " << item.getLatitude();

        dbHandler->removeGeoLocData("4.3.2.1");
        dbHandler->removeGeoLocData("4.3.2.1");
        item = dbHandler->fetchGeoLocData("4.3.2.1");
        qInfo() << "Fetched: URL: " << item.getUrl() << ", IP: " << item.getIp() << ", Long: " << item.getLongitude()
                << ", Lati: " << item.getLatitude();
    }
}

void MainWindow::addButtonPress() {
    infoLabel->setText(urlInput->text());
}

