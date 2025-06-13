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
#include <QVideoWidget>
#include <QGraphicsVideoItem>
#include <QAudioOutput>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QStackedWidget>
#include <QGraphicsBlurEffect>
#include <QEasingCurve>
#include <QTimer>
#include "ButtonAnimation.h"
#include <QMessageBox>


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
    isXturn = true;
    ui->PlayerLabel->hide();
    ui->welcometext->hide();
    // Set the window title
    setWindowTitle("X-O - THE GAME");

    ui->centralwidget->setGeometry(0, 0, this->width(), this->height());

 appDir = QApplication::applicationDirPath(); // APP DIR
    // Use AppDir to dynamically set paths
    QString icoPath = appDir + "/Images/X-O.ico";

    setWindowIcon(QIcon(icoPath));
    QString musicPath = appDir + "/sounds/Tarkovback_ground.wav"; // Path to your music file

    // Create QMediaPlayer for background music
    QMediaPlayer *musicPlayer = new QMediaPlayer(this);

    // Create QAudioOutput for controlling audio
    QAudioOutput *audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(0.6); // Set initial volume to 50%

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

      // Connect the player to the audio output



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
clickSound->setVolume(0.8); // Set volume (0.0 to 1.0)
//Connect the button's clicked signal to the sound effect
connect(ui->loginButton, &QPushButton::clicked, clickSound, &QSoundEffect::play);
connect(ui->GuestButton, &QPushButton::clicked, clickSound, &QSoundEffect::play);
connect(ui->PlayButton, &QPushButton::clicked, clickSound, &QSoundEffect::play);
connect(ui->StatsButton, &QPushButton::clicked, clickSound, &QSoundEffect::play);
connect(ui->ExitButton, &QPushButton::clicked, clickSound, &QSoundEffect::play);
connect(ui->VsPlayer, &QPushButton::clicked, clickSound, &QSoundEffect::play);
connect(ui->VsAI, &QPushButton::clicked, clickSound, &QSoundEffect::play);


//connect game buttons
connect(ui->index0, &QPushButton::clicked, this, [=]() { setImageOnButton(ui->index0); });
connect(ui->index1, &QPushButton::clicked, this, [=]() { setImageOnButton(ui->index1); });
connect(ui->index2, &QPushButton::clicked, this, [=]() { setImageOnButton(ui->index2); });
connect(ui->index3, &QPushButton::clicked, this, [=]() { setImageOnButton(ui->index3); });
connect(ui->index4, &QPushButton::clicked, this, [=]() { setImageOnButton(ui->index4); });
connect(ui->index5, &QPushButton::clicked, this, [=]() { setImageOnButton(ui->index5); });
connect(ui->index6, &QPushButton::clicked, this, [=]() { setImageOnButton(ui->index6); });
connect(ui->index7, &QPushButton::clicked, this, [=]() { setImageOnButton(ui->index7); });
connect(ui->index8, &QPushButton::clicked, this, [=]() { setImageOnButton(ui->index8); });

// Raise your UI components
ui->MainImage->raise();



    //Login and register screen

    loadAndDisplayImage(ui->MainImage, appDir + "/images/MainImage.png", 0.25);

    loadAndDisplayImage(ui->UserName, appDir + "/images/UserName.png", 0.25);

    loadAndDisplayImage(ui->Password, appDir + "/images/Password.png", 0.25);

    //login register button
    QIcon iconLogin(appDir +"/images/LoginRegister.png"); // Adjust the path to match your folder and file name
    ui->loginButton->setIcon(iconLogin);
    ui->loginButton->setIconSize(QSize(350, 350));
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




    ui->loginButton->setGraphicsEffect(createDropShadowEffect(this));






    // Connect animations to button clicks

    QPropertyAnimation* UnifiedAnimation = createUnifiedAnimation(ui->loginButton, this);

    // Connect UnifiedAnimation to the button click
    connect(ui->loginButton, &QPushButton::clicked, this, [UnifiedAnimation]() {
        UnifiedAnimation->start();
    });
    connect(ui->loginButton, &QPushButton::clicked, [this]() {

        // Add a tiny delay (e.g., 300 ms) before the transition starts
        QTimer::singleShot(200, [this]() {
            transitionWithBlur(ui->Menus, 0, 1);
        });
    });

    // Create Guest Button

    QIcon iconGuest(appDir +"/images/GUEST.png"); // Adjust the path to match your guest button image file
    ui->GuestButton->setIcon(iconGuest);
    ui->GuestButton->setIconSize(QSize(200, 200));

    // Style for the Guest Button (same as loginButton)
   ui->GuestButton->setStyleSheet(
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


    // Apply drop shadow effect to Guest Button (reuse from Login Button)
   ui->GuestButton->setGraphicsEffect(createDropShadowEffect(this));
   QPropertyAnimation* guestUnifiedAnimation = createUnifiedAnimation(ui->GuestButton, this);

   // Connect UnifiedAnimation to the button click
   connect(ui->GuestButton, &QPushButton::clicked, this, [guestUnifiedAnimation]() {
       guestUnifiedAnimation->start();

   });
   connect(ui->GuestButton, &QPushButton::clicked, [this]() {
       updatePlayerLabel(""); // Default text
       // Add a tiny delay (e.g., 300 ms) before the transition starts
       QTimer::singleShot(200, [this]() {
           transitionWithBlur(ui->Menus, 0, 1);
       });
   });
    ui->Menus->raise(); // Raise the stacked widget containing your pages

    // Raise individual UI components if necessary
    ui->loginButton->raise();
    ui->PlayButton->raise();
    ui->MainImage->raise();

// Main Menu

    //Play Button

    // Set an icon on the QPushButton
    QIcon iconPlay(appDir +"/images/Play.png"); // Adjust the path to match your folder and file name
    ui->PlayButton->setIcon(iconPlay);
    ui->PlayButton->setIconSize(QSize(150, 150));
    ui->PlayButton->setStyleSheet(
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



    // Apply drop shadow effect to Play Button (reuse from Login Button)
    ui->PlayButton->setGraphicsEffect(createDropShadowEffect(this));
    QPropertyAnimation* PlayUnifiedAnimation = createUnifiedAnimation(ui->PlayButton, this);

    // Connect UnifiedAnimation to the button click
    connect(ui->PlayButton, &QPushButton::clicked, this, [PlayUnifiedAnimation]() {
        PlayUnifiedAnimation->start();
    });
    connect(ui->PlayButton, &QPushButton::clicked, [this]() {
        // Add a tiny delay (e.g., 300 ms) before the transition starts
        QTimer::singleShot(200, [this]() {
            transitionWithBlur(ui->Menus, 1, 2);
        });
    });


    // STATS  BUTTON

    QIcon iconStats(appDir +"/images/STATS.png"); // Adjust the path to match your folder and file name
    ui->StatsButton->setIcon(iconStats);
    ui->StatsButton->setIconSize(QSize(160, 160));
    ui->StatsButton->setStyleSheet(
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

    ui->StatsButton->setGraphicsEffect(createDropShadowEffect(this));

    // Connect animations to button clicks

    QPropertyAnimation* StatsUnifiedAnimation = createUnifiedAnimation(ui->StatsButton, this);

    // Connect UnifiedAnimation to the button click
    connect(ui->StatsButton, &QPushButton::clicked, this, [StatsUnifiedAnimation]() {
        StatsUnifiedAnimation->start();
    });
    connect(ui->StatsButton, &QPushButton::clicked, [this]() {

        // Add a tiny delay (e.g., 300 ms) before the transition starts
        QTimer::singleShot(200, [this]() {
            transitionWithBlur(ui->Menus, 1, 3);
        });
    });

    // EXIT
    QIcon iconExit(appDir +"/images/Exit.png"); // Adjust the path to match your folder and file name
    ui->ExitButton->setIcon(iconExit);
    ui->ExitButton->setIconSize(QSize(150, 150));
    ui->ExitButton->setStyleSheet(
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

    ui->ExitButton->setGraphicsEffect(createDropShadowEffect(this));

    // Connect animations to button clicks

    QPropertyAnimation* ExitUnifiedAnimation = createUnifiedAnimation(ui->ExitButton, this);

    // Connect UnifiedAnimation to the button click
    connect(ui->ExitButton, &QPushButton::clicked, this, [ExitUnifiedAnimation]() {
        ExitUnifiedAnimation->start();
    });
    connect(ui->ExitButton, &QPushButton::clicked, [this]() {

        // Add a tiny delay before the transition starts
        QTimer::singleShot(200, [this]() {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Exit Application", "Are you sure you want to exit?",
                                          QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                QCoreApplication::quit();  // Exit the application
            }
        });
    });

    //Player Came Back
    // Set stylesheet programmatically
    ui->welcometext->setStyleSheet(
        "QLabel {"

        "    color: white;" /* White text for contrast */
 "    box-shadow: 5px 5px 10px rgba(0, 0, 0, 0.5);" /* Drop shadow emulation */
        "    padding: 5px;" /* Space between text and border */
        "    font-size: 14px;" /* Adjust font size */
        "    font-weight: bold;" /* Bold text */
        "    text-align: center;" /* Center-align text */
        "}"
        );
    ui->PlayerLabel->setStyleSheet(
        "QLabel {"

        "    color: white;" /* White text for contrast */
        "    box-shadow: 5px 5px 10px rgba(0, 0, 0, 0.5);" /* Drop shadow emulation */
        "    padding: 5px;" /* Space between text and border */
        "    font-size: 14px;" /* Adjust font size */
        "    font-weight: bold;" /* Bold text */
        "    text-align: center;" /* Center-align text */
        "}"
        );
    connect(ui->Menus, &QStackedWidget::currentChanged, this, [this](int index) {
        if (index == 1 || index == 2) { // Pages where QLabel should be visible
            ui->PlayerLabel->show();
            ui->welcometext->show();
        } else {
            ui->PlayerLabel->hide();
            ui->welcometext->hide();
        }
    });


    //Intermediate Play Window

    //VS Player
    QIcon VsPlayer(appDir +"/images/VS-PLAYER.png"); // Adjust the path to match your folder and file name
    ui->VsPlayer->setIcon(VsPlayer);
    ui->VsPlayer->setIconSize(QSize(300, 300));
    ui->VsPlayer->setStyleSheet(
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

    ui->VsPlayer->setGraphicsEffect(createDropShadowEffect(this));

    // Connect animations to button clicks

    QPropertyAnimation* VsPlayerUnifiedAnimation = createUnifiedAnimation(ui->VsPlayer, this);

    // Connect UnifiedAnimation to the button click
    connect(ui->VsPlayer, &QPushButton::clicked, this, [VsPlayerUnifiedAnimation]() {
        VsPlayerUnifiedAnimation->start();
    });
    connect(ui->VsPlayer, &QPushButton::clicked, [this]() {

        // Add a tiny delay (e.g., 300 ms) before the transition starts
        QTimer::singleShot(200, [this]() {
 transitionWithBlur(ui->Menus, 2, 4);
            moveGraphicsView();
 initializeBoard();
        });
    });

    //VS AI


    QIcon VsAI(appDir +"/images/VS-AI.png"); // Adjust the path to match your folder and file name
    ui->VsAI->setIcon(VsAI);
    ui->VsAI->setIconSize(QSize(160, 160));
    ui->VsAI->setStyleSheet(
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

    ui->VsAI->setGraphicsEffect(createDropShadowEffect(this));

    // Connect animations to button clicks

    QPropertyAnimation* VsAIUnifiedAnimation = createUnifiedAnimation(ui->VsAI, this);

    // Connect UnifiedAnimation to the button click
    connect(ui->VsAI, &QPushButton::clicked, this, [VsAIUnifiedAnimation]() {
        VsAIUnifiedAnimation->start();
    });
    connect(ui->VsAI, &QPushButton::clicked, [this]() {

        // Add a tiny delay (e.g., 300 ms) before the transition starts
        QTimer::singleShot(200, [this]() {
 transitionWithBlur(ui->Menus, 2, 4);
            moveGraphicsView();
 initializeBoard();
        });
    });

//Arena



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_PlayButton_clicked()
{

}

//transition effect
void MainWindow::applyBlurEffect(QWidget* widget, qreal blurRadius) {
    auto* blurEffect = new QGraphicsBlurEffect(widget);
    blurEffect->setBlurRadius(blurRadius);  // Set the blur intensity
    widget->setGraphicsEffect(blurEffect);
}

void MainWindow::slideTransition(QStackedWidget* stackedWidget, int currentIndex, int nextIndex) {
    QWidget* currentPage = stackedWidget->widget(currentIndex);
    QWidget* nextPage = stackedWidget->widget(nextIndex);

    // Ensure the incoming page is fully prepared
    nextPage->setGeometry(QRect(stackedWidget->width(), currentPage->y(),
                                nextPage->width(), nextPage->height()));
    nextPage->show();

    // Slide-out animation for the current (outgoing) page
    auto* animationOut = new QPropertyAnimation(currentPage, "geometry");
    animationOut->setDuration(500);
    animationOut->setStartValue(currentPage->geometry());
    animationOut->setEndValue(QRect(-currentPage->width(), currentPage->y(),
                                    currentPage->width(), currentPage->height()));
    animationOut->setEasingCurve(QEasingCurve::InOutQuad);

    // Slide-in animation for the next (incoming) page
    auto* animationIn = new QPropertyAnimation(nextPage, "geometry");
    animationIn->setDuration(500);
    animationIn->setStartValue(QRect(stackedWidget->width(), nextPage->y(),
                                     nextPage->width(), nextPage->height()));
    animationIn->setEndValue(QRect(0, nextPage->y(),
                                   nextPage->width(), nextPage->height()));
    animationIn->setEasingCurve(QEasingCurve::InOutQuad);

    // Connect animations to manage visibility and stacking
    QObject::connect(animationOut, &QPropertyAnimation::finished, [=]() {
        currentPage->hide(); // Hide the outgoing page
        stackedWidget->setCurrentIndex(nextIndex); // Update stacked widget index
    });

    // Start both animations
    animationOut->start();
    animationIn->start();
}

void MainWindow::transitionWithBlur(QStackedWidget* stackedWidget, int currentIndex, int nextIndex) {
    QWidget* currentPage = stackedWidget->widget(currentIndex);

    // Apply blur effect to the outgoing page
    applyBlurEffect(currentPage, 10.0);

    // Start sliding transition
    slideTransition(stackedWidget, currentIndex, nextIndex);

    // Reduce blur effect dynamically during transition
    auto* blurAnimation = new QPropertyAnimation(currentPage->graphicsEffect(), "blurRadius");
    blurAnimation->setDuration(500);
    blurAnimation->setStartValue(10.0);
    blurAnimation->setEndValue(0.0); // Remove blur at the end of the transition
    QObject::connect(blurAnimation, &QPropertyAnimation::finished, [currentPage]() {
        currentPage->setGraphicsEffect(nullptr); // Remove the blur effect completely
    });

    blurAnimation->start();
}

//welcome back
void MainWindow::updatePlayerLabel(const QString& username) {
    if (username.isEmpty()) {
        ui->PlayerLabel->setText("Player"); // Default text if no username
    } else {
        ui->PlayerLabel->setText(username); // Display the username
    }
}

//IMAGE MOVE
void MainWindow::moveGraphicsView() {
    QPropertyAnimation* animation = new QPropertyAnimation(ui->MainImage, "geometry");
    animation->setDuration(500);  // Duration in milliseconds
    animation->setStartValue(ui->MainImage->geometry());
    animation->setEndValue(ui->MainImage->geometry().translated(300, 0));  // Shift 300 pixels to the right
    animation->setEasingCurve(QEasingCurve::OutQuad);  // Smooth easing effect
    animation->start(QAbstractAnimation::DeleteWhenStopped);  // Automatically clean up after animation
}
void MainWindow::initializeBoard() {
    QList<QPushButton*> buttons = {
        ui->index0, ui->index1, ui->index2,
        ui->index3, ui->index4, ui->index5,
        ui->index6, ui->index7, ui->index8
    };
    isXturn = true;
    QIcon placeholder(appDir + ("/Images/Empty.png")); // a fully transparent PNG
    for (QPushButton* btn : buttons) {
        btn->setIcon(placeholder);
        btn->setIconSize(btn->size());
    }


}
void MainWindow::setImageOnButton(QPushButton* button) {
    if (!button) return;

    // Pick the correct icon
    QIcon icon(isXturn ? appDir+ ("/Images/x.png") : appDir+("/Images/o.png"));
    button->setIcon(icon);
    button->setIconSize(button->size());
    button->setEnabled(false); // Prevent re-clicking
    isXturn = !isXturn;        // Toggle turn
}
