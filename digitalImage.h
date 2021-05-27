#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_digitalImage.h"

class digitalImage : public QMainWindow
{
    Q_OBJECT

public:
    digitalImage(QWidget *parent = Q_NULLPTR);

private:
    Ui::digitalImageClass ui;
};
