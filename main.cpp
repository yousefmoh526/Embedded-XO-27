#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaDevices>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Set the application icon (affects the taskbar icon)
    QString appDir = QApplication::applicationDirPath(); // Path to application directory
    QString iconPath = appDir + "/Images/X-O.ico"; // Path to your .ico file

    if (QFile::exists(iconPath)) {
        QIcon appIcon(iconPath);
        a.setWindowIcon(appIcon); // Set application-wide icon
    } else {
        qDebug() << "Icon file not found at:" << iconPath;
    }
    MainWindow w;
    w.show();
    return a.exec();
}
