#include "ButtonAnimation.h"
#include <QGraphicsDropShadowEffect>

QPropertyAnimation* createShrinkAnimation(QPushButton* button, QObject* parent) {
    auto* animation = new QPropertyAnimation(button, "geometry", parent);
    animation->setDuration(100);  // Duration in milliseconds
    animation->setStartValue(button->geometry());
    animation->setEndValue(button->geometry().adjusted(5, 5, -5, -5));  // Shrink slightly
    animation->setEasingCurve(QEasingCurve::InOutQuad);  // Smooth animation
    return animation;
}

QPropertyAnimation* createEnlargeAnimation(QPushButton* button, QObject* parent) {
    auto* animation = new QPropertyAnimation(button, "geometry", parent);
    animation->setDuration(200);  // Duration in milliseconds
    animation->setStartValue(button->geometry());
    animation->setEndValue(button->geometry().adjusted(-2, -2, 2, 2));  // Enlarge slightly
    animation->setEasingCurve(QEasingCurve::OutBounce);  // Bounce effect
    return animation;
}

QPropertyAnimation* createUnifiedAnimation(QPushButton* button, QObject* parent) {
    auto* shrinkAnimation = createShrinkAnimation(button, parent);
    auto* enlargeAnimation = createEnlargeAnimation(button, parent);

    auto* unifiedAnimation = new QPropertyAnimation(button, "geometry", parent);
    unifiedAnimation->setDuration(shrinkAnimation->duration() + enlargeAnimation->duration());  // Combine durations
    unifiedAnimation->setStartValue(button->geometry());
    unifiedAnimation->setKeyValueAt(0.5, button->geometry().adjusted(5, 5, -5, -5));  // Midpoint (shrink effect)
    unifiedAnimation->setEndValue(button->geometry().adjusted(-2, -2, 2, 2));  // Endpoint (enlarge effect)
    unifiedAnimation->setEasingCurve(QEasingCurve::OutBounce);  // Bounce effect
    return unifiedAnimation;
}

QGraphicsEffect* createDropShadowEffect(QObject* parent) {
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(parent);
    effect->setBlurRadius(20);                  // The intensity of the shadow blur
    effect->setColor(QColor(255, 255, 255, 128)); // The shadow color (white with transparency)
    effect->setOffset(3, 3);                   // The offset of the shadow
    return effect;
}
