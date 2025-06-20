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
    // Forward declaration

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_PlayButton_clicked();


private:
    int currentUserId = -1;
    QStringList moveHistory;

    QPushButton* getButtonAtIndex(int index);
    void checkGameStatus();
    void makeAIMove();
    Ui::MainWindow *ui;
    int selectedDifficulty = 2;
    bool isXturn;  // ✅ This is your flag for turns
    QString appDir;
    short board[3][3]; // Represents Tic-Tac-Toe board (-1 for empty, 0 for 'O', 1 for 'X')
 void transitionWithBlur(QStackedWidget* stackedWidget, int currentIndex, int nextIndex);
    void slideTransition(QStackedWidget* stackedWidget, int currentIndex, int nextIndex);
 void applyBlurEffect(QWidget* widget, qreal blurRadius);
    void updatePlayerLabel(const QString& username);
 void moveGraphicsView();
    void initializeBoard();
 void setImageOnButton(QPushButton* button);
  void  showGameOverDialog(const QString& message);
 void restoreGraphicsViewPosition();
  void initializeDatabase();
 void handleLoginOrRegister(const QString& username, const QString& password);
  void populateStats();
 void loadReplay(int gameId);
};
#endif // MAINWINDOW_H
