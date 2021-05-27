#include "digitalImage.h"
#include <QtWidgets/QApplication>
#include "CareRayAPIDemo.h"
#include "opencv2/core.hpp"

int main(int argc, char *argv[])
{
	CCareRayAPIDemo* pCareRayAPIDemo = new CCareRayAPIDemo();
    QApplication a(argc, argv);
    digitalImage w;
    w.show();
    return a.exec();
}
