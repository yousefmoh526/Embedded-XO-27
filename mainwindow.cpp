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
#include "minimax.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QCryptographicHash>
//meow time to github
//hasher
QString hashPassword(const QString& password) {
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

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
    initializeDatabase();
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
connect(ui->VsAI_2, &QPushButton::clicked, clickSound, &QSoundEffect::play);
connect(ui->VsAI_3, &QPushButton::clicked, clickSound, &QSoundEffect::play);
connect(ui->Back, &QPushButton::clicked, clickSound, &QSoundEffect::play);
ui->backFromReplay->hide(); // Initially hidden in the UI constructor
connect(ui->backFromReplay, &QPushButton::clicked, clickSound, &QSoundEffect::play);
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
//replays
connect(ui->replayList, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
    int gameId = item->data(Qt::UserRole).toInt();

    // 🚀 Transition to gameplay view first (e.g., index 4)

    transitionWithBlur(ui->Menus, ui->Menus->currentIndex(), 4);

    // Delay replay slightly to let UI settle
    QTimer::singleShot(400, this, [this, gameId]() {
        loadReplay(gameId);
    });
});
//back from replay
QIcon iconBR(appDir +"/images/Back.png"); // Adjust the path to match your guest button image file
ui->backFromReplay->setIcon(iconBR);
ui->backFromReplay->setIconSize(QSize(200, 200));

// Style for the Back Button (same as loginButton)
ui->backFromReplay->setStyleSheet(
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


// Apply drop shadow effect to back Button (reuse from Login Button)
ui->backFromReplay->setGraphicsEffect(createDropShadowEffect(this));
QPropertyAnimation* BRUnifiedAnimation = createUnifiedAnimation(ui->backFromReplay, this);

// Connect UnifiedAnimation to the button click
connect(ui->backFromReplay, &QPushButton::clicked, this, [BRUnifiedAnimation]() {
    BRUnifiedAnimation->start();

});

connect(ui->backFromReplay, &QPushButton::clicked, this, [this]() {
    ui->backFromReplay->hide(); // Hide again for next time
    transitionWithBlur(ui->Menus, ui->Menus->currentIndex(), 3); // or wherever you want to return
});



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
        QString username = ui->UsernameText->toPlainText().trimmed();

        QString password = ui->PasswordText->getActualText(); // ✅ this gets the actual typed password

        handleLoginOrRegister(username, password);
updatePlayerLabel(ui->UsernameText->toPlainText());
populateStats();
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
        moveGraphicsView();
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
selectedDifficulty = 3;
        // Add a tiny delay (e.g., 300 ms) before the transition starts
        QTimer::singleShot(200, [this]() {
 transitionWithBlur(ui->Menus, 2, 4);
            moveGraphicsView();
 initializeBoard();
        });
    });

    //VS AI EASY


    QIcon VsAI(appDir +"/images/Ai-Easy.png"); // Adjust the path to match your folder and file name
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
selectedDifficulty = 0;
        // Add a tiny delay (e.g., 300 ms) before the transition starts
        QTimer::singleShot(200, [this]() {
 transitionWithBlur(ui->Menus, 2, 4);
            moveGraphicsView();
 initializeBoard();
        });
    });

    //VS AI MEDIUM


    QIcon VsAI_2(appDir +"/images/Ai-Med.png"); // Adjust the path to match your folder and file name
    ui->VsAI_2->setIcon(VsAI_2);
    ui->VsAI_2->setIconSize(QSize(160, 160));
    ui->VsAI_2->setStyleSheet(
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

    ui->VsAI_2->setGraphicsEffect(createDropShadowEffect(this));

    // Connect animations to button clicks

    QPropertyAnimation* VsAI2UnifiedAnimation = createUnifiedAnimation(ui->VsAI_2, this);

    // Connect UnifiedAnimation to the button click
    connect(ui->VsAI_2, &QPushButton::clicked, this, [VsAI2UnifiedAnimation]() {
        VsAI2UnifiedAnimation->start();
    });
    connect(ui->VsAI_2, &QPushButton::clicked, [this]() {
selectedDifficulty = 1;
        // Add a tiny delay (e.g., 300 ms) before the transition starts
        QTimer::singleShot(200, [this]() {
            transitionWithBlur(ui->Menus, 2, 4);
            moveGraphicsView();
            initializeBoard();
        });
    });

    //VS AI HARD


    QIcon VsAI_3(appDir +"/images/Ai-Hard.png"); // Adjust the path to match your folder and file name
    ui->VsAI_3->setIcon(VsAI_3);
    ui->VsAI_3->setIconSize(QSize(160, 160));
    ui->VsAI_3->setStyleSheet(
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

    ui->VsAI_3->setGraphicsEffect(createDropShadowEffect(this));

    // Connect animations to button clicks

    QPropertyAnimation* VsAI3UnifiedAnimation = createUnifiedAnimation(ui->VsAI_3, this);

    // Connect UnifiedAnimation to the button click
    connect(ui->VsAI_3, &QPushButton::clicked, this, [VsAI3UnifiedAnimation]() {
        VsAI3UnifiedAnimation->start();
    });
    connect(ui->VsAI_3, &QPushButton::clicked, [this]() {
selectedDifficulty = 2;
        // Add a tiny delay (e.g., 300 ms) before the transition starts
        QTimer::singleShot(200, [this]() {

            transitionWithBlur(ui->Menus, 2, 4);
            moveGraphicsView();
            initializeBoard();
        });
    });

    //Ai TITLE

    loadAndDisplayImage(ui->VsAITitle, appDir + "/images/AITitle.png", 1);


//stats screen

loadAndDisplayImage(ui->gamesPlayed, appDir + "/images/Games-Played.png", 0.45);
loadAndDisplayImage(ui->wins, appDir + "/images/Wins.png", 0.5);
loadAndDisplayImage(ui->losses, appDir + "/images/Losses.png", 0.5);
loadAndDisplayImage(ui->draws, appDir + "/images/Draws.png", 0.5);
loadAndDisplayImage(ui->wR, appDir + "/images/WR.png", 0.5);
ui->gamesPlayedLabel->setStyleSheet(R"(
    QLabel {
        color: white;
        font-weight: bold;
        font-size: 18px;
        letter-spacing: 1px;
    }
)");

ui->winsLabel->setStyleSheet(R"(
    QLabel {
        color: white;
        font-weight: bold;
        font-size: 18px;
        letter-spacing: 1px;
    }
)");

ui->lossesLabel->setStyleSheet(R"(
    QLabel {
        color: white;
        font-weight: bold;
        font-size: 18px;
        letter-spacing: 1px;
    }
)");

ui->wRLabel->setStyleSheet(R"(
    QLabel {
        color: white;
        font-weight: bold;
        font-size: 18px;
        letter-spacing: 1px;
    }
)");

ui->drawsLabel->setStyleSheet(R"(
    QLabel {
        color: white;
        font-weight: bold;
        font-size: 18px;
        letter-spacing: 1px;
    }
)");
auto shadow = new QGraphicsDropShadowEffect(this);
shadow->setBlurRadius(20);
shadow->setOffset(2, 2);
shadow->setColor(Qt::black);
ui->gamesPlayedLabel->setGraphicsEffect(shadow);
ui->winsLabel->setGraphicsEffect(shadow);
ui->lossesLabel->setGraphicsEffect(shadow);
ui->drawsLabel->setGraphicsEffect(shadow);
ui->wRLabel->setGraphicsEffect(shadow);
//Back Button

// Set an icon on the QPushButton
QIcon iconBack(appDir +"/images/Back.png"); // Adjust the path to match your folder and file name
ui->Back->setIcon(iconBack);
ui->Back->setIconSize(QSize(150, 150));
ui->Back->setStyleSheet(
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
ui->Back->setGraphicsEffect(createDropShadowEffect(this));
QPropertyAnimation* BackUnifiedAnimation = createUnifiedAnimation(ui->Back, this);

// Connect UnifiedAnimation to the button click
connect(ui->Back, &QPushButton::clicked, this, [BackUnifiedAnimation]() {
    BackUnifiedAnimation->start();

});
connect(ui->Back, &QPushButton::clicked, [this]() {
    restoreGraphicsViewPosition();
    // Add a tiny delay (e.g., 300 ms) before the transition starts
    QTimer::singleShot(200, [this]() {

        transitionWithBlur(ui->Menus, 3, 1);
        // Delay stats population slightly
        QTimer::singleShot(500, [this]() {
            populateStats();
        });
    });
});

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

    // 🚀 **Force correct geometry for new page before animation**
    stackedWidget->setCurrentIndex(nextIndex);
    stackedWidget->repaint();
    nextPage->setGeometry(QRect(stackedWidget->width(), currentPage->y(),
                                nextPage->width(), nextPage->height()));
    nextPage->setVisible(true);
    nextPage->update();
    QCoreApplication::processEvents(); // ✅ Forces immediate UI updates

    // 🚀 **Outgoing animation (fast exit)**
    auto* animationOut = new QPropertyAnimation(currentPage, "geometry");
    animationOut->setDuration(300); // **Quick transition**
    animationOut->setStartValue(currentPage->geometry());
    animationOut->setEndValue(QRect(-currentPage->width(), currentPage->y(),
                                    currentPage->width(), currentPage->height()));
    animationOut->setEasingCurve(QEasingCurve::InExpo);

    QObject::connect(animationOut, &QPropertyAnimation::finished, [=]() {
        currentPage->hide();
        stackedWidget->repaint();  // ✅ Ensure entire widget refreshes
    });

    // 🚀 **Incoming animation (smooth)**
    auto* animationIn = new QPropertyAnimation(nextPage, "geometry");
    animationIn->setDuration(600);
    animationIn->setStartValue(QRect(stackedWidget->width(), nextPage->y(),
                                     nextPage->width(), nextPage->height()));
    animationIn->setEndValue(QRect(0, nextPage->y(),
                                   nextPage->width(), nextPage->height()));
    animationIn->setEasingCurve(QEasingCurve::InOutExpo);

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
//board intialization
void MainWindow::initializeBoard() {
    QList<QPushButton*> buttons = {
        ui->index0, ui->index1, ui->index2,
        ui->index3, ui->index4, ui->index5,
        ui->index6, ui->index7, ui->index8
    };
    isXturn = true;
    moveHistory.clear();

    // **Reset board array to -1 (empty cells)**
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = -1;
    QIcon placeholder(appDir + ("/Images/Empty.png")); // a fully transparent PNG
    for (QPushButton* btn : buttons) {
        btn->setIcon(placeholder);
        btn->setIconSize(btn->size());
        btn->setEnabled(true);
    }


}
//button image on table
void MainWindow::setImageOnButton(QPushButton* button) {
    if (!button) return;
    // Extract the number from the button name (e.g., "index3" → 3)
    QString buttonName = button->objectName();  // Gets "indexX"
    int index = buttonName.mid(5).toInt();  // Extract number after "index"
qDebug() << "Setting icon for button:" << buttonName << "Index:" << index;

    // Update board state
    board[index / 3][index % 3] = isXturn ? 1 : 0;
    // Pick the correct icon
    QIcon icon(isXturn ? appDir+ ("/Images/x.png") : appDir+("/Images/o.png"));
    button->setIcon(icon);
    button->setIconSize(button->size());
    button->setEnabled(false); // Prevent re-clicking
    QString moveString = QString("%1:%2").arg(isXturn ? "X" : "O").arg(index);
    moveHistory.append(moveString);
    isXturn = !isXturn;        // Toggle turn
    checkGameStatus();
}
//table check
void MainWindow::checkGameStatus() {
    int resultX = check_for_win(board, 1, 0); // Check if X won
    int resultO = check_for_win(board, 0, 0); // Check if O won
qDebug() << "Result X:" << resultX << "Result O:" << resultO;
    if (resultX == -10) {
        showGameOverDialog("X Wins!");
        populateStats();
        return;
    }

    if (resultO == 10) {
        showGameOverDialog("O Wins!");
        populateStats();
        return;
    }

    // Check if board is full (draw)
    bool isDraw = true;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == -1) isDraw = false;

    if (isDraw) {
        showGameOverDialog("It's a Draw!");
        populateStats();
        return;
    }

    // ✅ **Only trigger AI move if difficulty is NOT 3 (PvP mode)**
    if (!isXturn && selectedDifficulty != 3)
        QTimer::singleShot(800, this, [this]() {
            makeAIMove();
        });
}
//ai handle
void MainWindow::makeAIMove() {
    int aiMove = -1;

    // Select AI difficulty
    if (selectedDifficulty == 0) { // Easy AI
        aiMove = easy_move(board);
    } else if (selectedDifficulty == 1) { // Medium AI
        aiMove = medium_move(board);
    } else { // Hard AI (Minimax)
        aiMove = hard_move(board);
    }
    // Debug: Check if AI move is valid
    qDebug() << "AI chooses index:" << aiMove;
    // If AI found a valid move, play it
    if (aiMove != -1) {

        QPushButton* aiButton = getButtonAtIndex(aiMove);

        if (aiButton) {
            qDebug() << "Updating button:" << aiButton->objectName();
            setImageOnButton(aiButton);
        } else {
            qDebug() << "ERROR: Button at index" << aiMove << "not found!";
        }
        // Debug: Print board state
        for (int i = 0; i < 3; i++)
            qDebug() << board[i][0] << board[i][1] << board[i][2];

        isXturn = true; // Switch back to player turn
    }


}
QPushButton* MainWindow::getButtonAtIndex(int index) {
    QList<QPushButton*> buttons = {
        ui->index0, ui->index1, ui->index2,
        ui->index3, ui->index4, ui->index5,
        ui->index6, ui->index7, ui->index8
    };
qDebug() << "AI selecting button at index:" << index;
    if (index >= 0 && index < buttons.size()) {
        return buttons[index]; // Return button at given index
    }

    return nullptr; // Return null if index is out of range
}

// game over
void MainWindow::showGameOverDialog(const QString& message) {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Game Over");
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Information);
    populateStats();
    QPushButton* playAgainButton = msgBox.addButton("Play Again", QMessageBox::AcceptRole);
    QPushButton* returnMenuButton = msgBox.addButton("Return to Menu", QMessageBox::RejectRole);
    qDebug() << "Current user ID when saving stats:" << currentUserId;

    if (currentUserId != -1) {
        QString result;
        if (message.contains("X Wins")) result = "Win";
        else if (message.contains("O Wins")) result = "Loss";
        else result = "Draw";

        QSqlQuery insert;
        insert.prepare("INSERT INTO GameHistory (user_id, result, moves, played_at) "
                       "VALUES (:user_id, :result, :moves, datetime('now'))");

        insert.bindValue(":user_id", currentUserId);
        insert.bindValue(":result", result);
        insert.bindValue(":moves", moveHistory.join(","));
        insert.exec();
    }

    msgBox.exec();

    if (msgBox.clickedButton() == returnMenuButton) {
        restoreGraphicsViewPosition();
        transitionWithBlur(ui->Menus, ui->Menus->currentIndex(), 1); // Assuming index 1 is menu
    } else {
        initializeBoard(); // Restart game
    }
}
// return title to center
void MainWindow::restoreGraphicsViewPosition() {
    QPropertyAnimation* animation = new QPropertyAnimation(ui->MainImage, "geometry");
    animation->setDuration(500);
    animation->setStartValue(ui->MainImage->geometry());
    animation->setEndValue(ui->MainImage->geometry().translated(-300, 0));  // Move back to original position
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}
// DATA BASE
void MainWindow::initializeDatabase() {
    QString dbPath = QCoreApplication::applicationDirPath() + "/game_data.db";
    QDir().mkpath(QFileInfo(dbPath).absolutePath()); // Make sure /data exists

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "❌ Failed to open database:" << db.lastError().text();
        return;
    }

    QSqlQuery query;


    // Create Users table
    query.exec("CREATE TABLE IF NOT EXISTS Users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "username TEXT UNIQUE NOT NULL,"
               "password_hash TEXT NOT NULL,"
               "display_name TEXT,"
               "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)");

    // Create GameHistory table
    query.exec("CREATE TABLE IF NOT EXISTS GameHistory ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "user_id INTEGER,"
               "opponent_type TEXT,"
               "difficulty INTEGER,"
               "result TEXT,"
               "moves TEXT,"
               "played_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
               "FOREIGN KEY(user_id) REFERENCES Users(id))");

    qDebug() << "✅ Database initialized at:" << dbPath;
}
//login and register handler
void MainWindow::handleLoginOrRegister(const QString& username, const QString& password) {
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Username and password cannot be empty.");
        return;
    }

    if (password.length() < 5) {
        QMessageBox::warning(this, "Password Too Short", "Password must be at least 5 characters long.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT id, password_hash FROM Users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        QString storedHash = query.value("password_hash").toString();
        int userId = query.value("id").toInt();

        if (storedHash == hashPassword(password)) {
            currentUserId = userId;
            updatePlayerLabel(username);

            QTimer::singleShot(200, [this]() {
                transitionWithBlur(ui->Menus, 0, 1);
            });
        } else {
            QMessageBox::critical(this, "Login Failed", "Incorrect password.");
        }

    } else {
        QSqlQuery insert;
        insert.prepare("INSERT INTO Users (username, password_hash, display_name) "
                       "VALUES (:username, :password_hash, :display_name)");
        insert.bindValue(":username", username);
        insert.bindValue(":password_hash", hashPassword(password));
        insert.bindValue(":display_name", username);

        if (insert.exec()) {
            currentUserId = insert.lastInsertId().toInt();
            updatePlayerLabel(username);

            QMessageBox::information(this, "Registration Successful", "Welcome, " + username + "!");

            QTimer::singleShot(200, [this]() {
                transitionWithBlur(ui->Menus, 0, 1);
            });
        } else {
            QMessageBox::critical(this, "Registration Failed", insert.lastError().text());
        }
    }
}
//stats
void MainWindow::populateStats() {
    int gamesPlayed = 0, wins = 0, losses = 0, draws = 0;

    QSqlQuery query;
    query.prepare("SELECT result FROM GameHistory WHERE user_id = :id");
    query.bindValue(":id", currentUserId);
    if (query.exec()) {
        while (query.next()) {
            gamesPlayed++;
            QString result = query.value(0).toString();
            if (result == "Win") wins++;
            else if (result == "Loss") losses++;
            else if (result == "Draw") draws++;
        }
    } else {
        qDebug() << "Failed to fetch game stats:" << query.lastError().text();
    }

    double winRate = (gamesPlayed > 0) ? (static_cast<double>(wins) / gamesPlayed) * 100.0 : 0.0;

    // Update labels
    ui->gamesPlayedLabel->setText(QString::number(gamesPlayed));
    ui->winsLabel->setText(QString::number(wins));
    ui->lossesLabel->setText(QString::number(losses));
    ui->drawsLabel->setText(QString::number(draws));
    ui->wRLabel->setText(QString::number(winRate, 'f', 1) + "%");
    ui->replayList->clear();

    QSqlQuery q;
    q.prepare("SELECT id, result, played_at FROM GameHistory WHERE user_id = :userId ORDER BY played_at DESC");
    q.bindValue(":userId", currentUserId);
    q.exec();

    while (q.next()) {
        int gameId = q.value("id").toInt();
        QString result = q.value("result").toString();
        QString date = q.value("played_at").toString();

        auto* item = new QListWidgetItem(QString("Game #%1 - %2 on %3").arg(gameId).arg(result, date));
        item->setData(Qt::UserRole, gameId);
        ui->replayList->addItem(item);
    }

}
//game replay system
void MainWindow::loadReplay(int gameId) {
    // Reset board
    initializeBoard();
    ui->backFromReplay->show(); // 🌟 Only shown during replay

    QSqlQuery q;
    q.prepare("SELECT moves FROM GameHistory WHERE id = :id");
    q.bindValue(":id", gameId);
    if (!q.exec() || !q.next()) {
        qDebug() << "❌ Failed to load replay for game ID" << gameId;
        return;
    }

    QString moves = q.value("moves").toString();
    QStringList moveList = moves.split(",", Qt::SkipEmptyParts);

    // Play moves one by one
    int delay = 0;
    for (const QString& move : moveList) {
        QStringList parts = move.split(":");
        if (parts.size() != 2) continue;

        QString symbol = parts[0];
        int index = parts[1].toInt();

        QTimer::singleShot(delay, this, [=]() {
            QPushButton* btn = getButtonAtIndex(index);
            if (btn) {
                QIcon icon(symbol == "X" ? appDir + "/Images/x.png"
                                         : appDir + "/Images/o.png");
                btn->setIcon(icon);
                btn->setIconSize(btn->size());
                btn->setEnabled(false);
            }
        });

        delay += 800; // Delay between each move (in ms)
    }

}
