#include "hoverbutton.h"
#include <QDebug>

HoverButton::HoverButton(QWidget *parent)
    : QPushButton(parent), shadowGlow(nullptr) {
}

void HoverButton::setShadowGlowAnimation(QPropertyAnimation *animation) {
    shadowGlow = animation;
}

void HoverButton::enterEvent(QEnterEvent *event) {
    if (shadowGlow) {
        shadowGlow->setStartValue(10);
        shadowGlow->setEndValue(60); // Increase blur for glow effect
        shadowGlow->start();
    }
    setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 rgba(255, 255, 255, 0.2), stop:1 rgba(255, 255, 255, 0.1));"
        "border: 1px solid rgba(255, 255, 255, 0.5);"
        "border-radius: 10px;"
        );
    QPushButton::enterEvent(event);
}

void HoverButton::leaveEvent(QEvent *event) {
    if (shadowGlow) {
        shadowGlow->setStartValue(60);
        shadowGlow->setEndValue(10); // Reduce blur for glow effect
        shadowGlow->start();
    }
    setStyleSheet(
        "background-color: transparent;"
        "border: none;"
        );
    QPushButton::leaveEvent(event);
}
