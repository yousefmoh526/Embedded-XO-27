#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QKeyEvent>
#include <QTextEdit>
#include "ui_mainwindow.h"
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <hoverbutton.h>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QGraphicsVideoItem>
#include <QAudioOutput>
//IMAGE DISPLAYER
void loadAndDisplayImage(QGraphicsView *view, QString imagePath, double scaleFactor) {
    QGraphicsScene *scene = new QGraphicsScene(view);
    scene->setBackgroundBrush(Qt::NoBrush); // Transparent background for the scene
    view->setScene(scene);

    view->setFrameShape(QFrame::NoFrame); // Remove any frame
    view->setStyleSheet("background: transparent;"); // Transparent background for the view

    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        QGraphicsPixmapItem *item = scene->addPixmap(pixmap);

        QRectF boundingRect = item->boundingRect();
        item->setPos(-boundingRect.width() / 2, -boundingRect.height() / 2);

        item->setTransform(QTransform::fromScale(scaleFactor, scaleFactor));
        scene->setSceneRect(item->sceneBoundingRect());
        view->centerOn(scene->sceneRect().center());
    } else {
        qDebug() << "Failed to load image at:" << imagePath;
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->centralwidget->setGeometry(0, 0, this->width(), this->height());

QString appDir = QApplication::applicationDirPath();
    QString musicPath = appDir + "/sounds/back_ground.wav"; // Path to your music file

    // Create QMediaPlayer for background music
    QMediaPlayer *musicPlayer = new QMediaPlayer(this);

    // Create QAudioOutput for controlling audio
    QAudioOutput *audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(0.5); // Set initial volume to 50%

    // Attach the audio output to the media player
    musicPlayer->setAudioOutput(audioOutput);

    // Load and play the music
    if (QFile::exists(musicPath)) {
        musicPlayer->setSource(QUrl::fromLocalFile(musicPath));
        musicPlayer->play(); // Start playing music
    } else {
        qDebug() << "Music file not found at:" << musicPath;
    }

    // Set looping for the background music
    connect(musicPlayer, &QMediaPlayer::mediaStatusChanged, this, [musicPlayer](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            musicPlayer->play(); // Restart music when it ends
        }
    });

    // After ui->setupUi(this);
    QString videoPath = appDir + "/videos/background2.mp4"; // Path to your video file

    // Create a QGraphicsScene and QGraphicsView
    QGraphicsScene *scene = new QGraphicsScene(ui->centralwidget); // Scene tied to centralwidget
    QGraphicsView *graphicsView = new QGraphicsView(scene, ui->centralwidget); // View to display scene
    graphicsView->setGeometry(0, 0, ui->centralwidget->width(), ui->centralwidget->height());
    graphicsView->setFrameShape(QFrame::NoFrame); // Remove borders
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // No scroll bars
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->lower(); // Push the view to the background
    graphicsView->show();

    // Add a QGraphicsVideoItem to the scene
    QGraphicsVideoItem *videoItem = new QGraphicsVideoItem();
    scene->addItem(videoItem); // Add video item to the graphics scene
    videoItem->setSize(QSizeF(ui->centralwidget->width(), ui->centralwidget->height())); // Match parent dimensions

    // Set up the QMediaPlayer to play the video on the QGraphicsVideoItem
    QMediaPlayer *videoPlayer = new QMediaPlayer(this);
    videoPlayer->setVideoOutput(videoItem);

    // Load and play the video
    if (QFile::exists(videoPath)) {
        videoPlayer->setSource(QUrl::fromLocalFile(videoPath));
        videoPlayer->play(); // Start video playback
    } else {
        qDebug() << "Video file not found at:" << videoPath;
    }

    // Loop the video
    connect(videoPlayer, &QMediaPlayer::mediaStatusChanged, this, [videoPlayer](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            videoPlayer->play(); // Restart the video when it ends
        }
    });

    // Raise UI components above the graphics view
    ui->Menus->raise();       // Stacked widget containing multiple pages
    ui->loginButton->raise(); // Login button
    ui->PlayButton->raise();  // Play button
    ui->MainImage->raise();   // Main image


//CLICK SOUND

QString soundPath = appDir + "/sounds/minecraftclick.wav"; // Append the relative path to your sound file


QSoundEffect *clickSound = new QSoundEffect(this);

if (QFile::exists(soundPath)) {
    clickSound->setSource(QUrl::fromLocalFile(soundPath));
} else {
    qDebug() << "Sound file not found at:" << soundPath;
}

;
clickSound->setVolume(1); // Set volume (0.0 to 1.0)
//Connect the button's clicked signal to the sound effect
connect(ui->loginButton, &QPushButton::clicked, clickSound, &QSoundEffect::play);



// Raise your UI components
ui->MainImage->raise();


    //Login and register screen

    loadAndDisplayImage(ui->MainImage, appDir + "/images/MainImage.png", 0.25);

    loadAndDisplayImage(ui->UserName, appDir + "/images/UserName.png", 0.25);

    loadAndDisplayImage(ui->Password, appDir + "/images/Password.png", 0.25);

    //login register button
    QIcon iconLogin("images/LoginRegister.png"); // Adjust the path to match your folder and file name
    ui->loginButton->setIcon(iconLogin);
    ui->loginButton->setIconSize(QSize(300, 300));
    ui->loginButton->setStyleSheet(
        "QPushButton {"
        "    border: none;"

        "    border-radius: 10px;" /* Smooth edges */
        "    color: white;" /* Ensure text/icon contrasts well */
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "        stop:0 rgba(60, 60, 60, 1), stop:1 rgba(30, 30, 30, 0.9));" /* Subtle hover gradient */
        "    border: 1px solid rgba(255, 255, 255, 0.2);" /* Simulated glow */
        "}"
        "QPushButton:pressed {"
        "    background-color: rgba(20, 20, 20, 0.9);" /* Slightly darker when pressed */
        "    border: 1px solid rgba(255, 255, 255, 0.4);" /* Stronger pressed border glow */
        "}"
        );


    // Create a property animation for the button geometry
    QPropertyAnimation *animation = new QPropertyAnimation(ui->loginButton, "geometry", this);
    animation->setDuration(100); // Duration in milliseconds
    animation->setStartValue(ui->loginButton->geometry());
    animation->setEndValue(ui->loginButton->geometry().adjusted(5, 5, -5, -5)); // Shrink slightly
    animation->setEasingCurve(QEasingCurve::InOutQuad); // Smooth animation

    QPropertyAnimation *scaleAnimation = new QPropertyAnimation(ui->loginButton, "geometry", this);
    scaleAnimation->setDuration(200); // Slow it down slightly
    scaleAnimation->setStartValue(ui->loginButton->geometry());
    scaleAnimation->setEndValue(ui->loginButton->geometry().adjusted(-2, -2, 2, 2)); // Slight enlarge effect
    scaleAnimation->setEasingCurve(QEasingCurve::OutBounce); // Bounce effect for tactile feel



    // Apply a drop shadow effect to the button
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(20); // Amount of blur
    shadowEffect->setColor(QColor(255, 255, 255, 128)); // White glow
    shadowEffect->setOffset(3, 3); // No offset for centered shadow
    ui->loginButton->setGraphicsEffect(shadowEffect);
    QPropertyAnimation *shadowGlow = new QPropertyAnimation(shadowEffect, "blurRadius", this);
    shadowGlow->setDuration(200);
    shadowGlow->setStartValue(10); // Initial blur
    shadowGlow->setEndValue(20); // Glow increase

    // Connect animations to button clicks
    connect(ui->loginButton, &QPushButton::clicked, this, [scaleAnimation]() {
        scaleAnimation->start();
    });
    connect(ui->loginButton, &QPushButton::clicked, this, [animation]() {
        animation->start();
    });


    ui->Menus->raise(); // Raise the stacked widget containing your pages

    // Raise individual UI components if necessary
    ui->loginButton->raise();
    ui->PlayButton->raise();
    ui->MainImage->raise();

    connect(musicPlayer, &QMediaPlayer::mediaStatusChanged, this, [](QMediaPlayer::MediaStatus status) {
        qDebug() << "Media Status:" << status;

    });

    connect(musicPlayer, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error) {
        qDebug() << "Media Player Error:" << error;
    });


    //Play Button

    // Set an icon on the QPushButton
    QIcon iconPlay("images/Play.png"); // Adjust the path to match your folder and file name
    ui->PlayButton->setIcon(iconPlay);
    ui->PlayButton->setIconSize(QSize(100, 100));




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_PlayButton_clicked()
{
    ui -> Menus ->setCurrentIndex(1);
}


