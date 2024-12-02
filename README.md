# Sofomo

Simple recruitment task.

---

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Build and Run](#build-and-run)

---

## Introduction
Application provides simple QT Widgets GUI to inquire ipstack.com to fetch geolocation data for domain/IP. 
SQL database supports the application to store already discovered entries to prevent extensive calls to ipstack API.

---

## Features
- QT Widgets
- SQL database support with sqlite3 library
- domain resolution using Linux netdb functions
- ipstack.com http request with curl library

---

## Prerequisites
Before building the project, ensure you have the following installed:
- cmake (3.22+)
- build-essential
- git
- libxkbcommon-x11-0
- libxcb-icccm4
- libxcb-image0
- libxcb-keysyms1
- libxcb-render-util0
- libxcb-shape0
- libgl1-mesa-dev
- libcurl4-gnutls-dev
- qt6.8.0-essentials (e.g. ./qt-online-installer-linux-x64-4.8.1.run --root /opt/qt --accept-licenses --default-answer --confirm-command install qt6.8.0-essentials)
---

## Build and Run

### Clone the Repository
```bash
git clone git@github.com:pietryna/sofomo_task.git
cd sofomo_task
```
### Configure
```bash
cmake -DQt6_DIR=<qt install dir>/6.8.0/gcc_64/lib/cmake/Qt6 -S ./ -B ./build
```
### Build app
```bash
cmake --build ./build/ --target Sofomo -j
```
### Build tests
```bash
cmake --build ./build/ --target Sofomo_test -j
```