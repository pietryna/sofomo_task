// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtWidgets>

#include "mainwindow.h"

MainWindow::MainWindow()
{
    auto* centerWidget = new QWidget;
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

    geolocOut  = new QLabel;
    geolocOut->setText("Text");
    geolocOut->setAlignment(Qt::AlignCenter);
    geolocOut->setFrameStyle(1u);

    auto* mainLayout = new QVBoxLayout;
    auto* controlWidget = new QWidget;
    centerWidget->setLayout(mainLayout);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(controlWidget);

    auto* bottomLayout = new QHBoxLayout;
    controlWidget->setLayout(bottomLayout);

    auto* leftWidget = new QWidget;
    auto* rightWidget = new QWidget;
    bottomLayout->addWidget(leftWidget);
    bottomLayout->addWidget(rightWidget);

    auto* vLeftLayout = new QVBoxLayout;
    vLeftLayout->setContentsMargins(5, 5, 5, 5);
    vLeftLayout->addWidget(addButton);
    vLeftLayout->addWidget(deleteButton);
    vLeftLayout->addWidget(provideButton);
    leftWidget->setLayout(vLeftLayout);

    auto* vRightLayout = new QVBoxLayout;
    vRightLayout->setContentsMargins(5, 5, 5, 5);
    vRightLayout->addWidget(urlInput);
    vRightLayout->addWidget(geolocOut);
    rightWidget->setLayout(vRightLayout);

    QString message = tr("Maybe needed later");
    this->statusBar()->showMessage(message);

    this->setWindowTitle(tr(MAIN_WINDOW));
    this->resize(640, 480);
}

void MainWindow::addButtonPress()
{
    infoLabel->setText(urlInput->text());
}

