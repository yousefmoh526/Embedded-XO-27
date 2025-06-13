#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QStackedWidget>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMediaDevices>
#include <QPushButton>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_PlayButton_clicked();


private:
    Ui::MainWindow *ui;
    bool isXturn;  // ✅ This is your flag for turns
    QString appDir;
 void transitionWithBlur(QStackedWidget* stackedWidget, int currentIndex, int nextIndex);
    void slideTransition(QStackedWidget* stackedWidget, int currentIndex, int nextIndex);
 void applyBlurEffect(QWidget* widget, qreal blurRadius);
    void updatePlayerLabel(const QString& username);
 void moveGraphicsView();
    void initializeBoard();
 void setImageOnButton(QPushButton* button);
};
#endif // MAINWINDOW_H
