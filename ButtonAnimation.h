#ifndef BUTTONANIMATION_H
#define BUTTONANIMATION_H

#include <QPushButton>
#include <QPropertyAnimation>
#include <QGraphicsEffect>
#include <QEasingCurve>

// Declare reusable functions for button animations
QPropertyAnimation* createShrinkAnimation(QPushButton* button, QObject* parent);
QPropertyAnimation* createEnlargeAnimation(QPushButton* button, QObject* parent);
QPropertyAnimation* createUnifiedAnimation(QPushButton* button, QObject* parent);
QGraphicsEffect* createDropShadowEffect(QObject* parent);



#endif // BUTTONANIMATION_H
