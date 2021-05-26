#include <QApplication>

#include "qcalculateur.h"

int main(int argc, char *argv[])
{
    QApplication app = QApplication{argc, argv};
    QCalculateur cal = QCalculateur();
    app.setWindowIcon(QIcon("../projet-comput-master/res/icon/calculateur_icon.png"));
    cal.show();
    return app.exec();
}
