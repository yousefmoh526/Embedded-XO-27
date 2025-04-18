#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>
#include <QApplication>
#include <QDir>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Create a scene for the MainImage view
    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->MainImage->setScene(scene);

    // Disable rubber band selection to avoid the error
    ui->MainImage->setDragMode(QGraphicsView::NoDrag);

    // Get path to the image
QString appDir = QApplication::applicationDirPath();
    QString imagePath = appDir + "/images/MainImage.png";

    // Load the image
    QPixmap pixmap(imagePath);

    if (!pixmap.isNull()) {
        // Add pixmap to the scene
        QGraphicsPixmapItem *item = scene->addPixmap(pixmap);

        // Scale the pixmap using a transformation
        item->setTransform(QTransform::fromScale(5.0, 5.0)); // Scale to 200%

        // Update the scene rectangle to match the scaled item
        scene->setSceneRect(item->boundingRect());

        // Align and fit the view to the scene
        ui->MainImage->setAlignment(Qt::AlignCenter);
        ui->MainImage->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    } else {
        qDebug() << "Failed to load image at:" << imagePath;
    }
    //Login and register screen

    // Set the desired page (e.g., index 1) as the default
    ui->Menus->setCurrentIndex(0);

    //Play Button

    // Set an icon on the QPushButton
    QIcon icon("images/Play.png"); // Adjust the path to match your folder and file name
    ui->PlayButton->setIcon(icon);
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

