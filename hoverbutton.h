#ifndef HOVERBUTTON_H
#define HOVERBUTTON_H

#include <QPushButton>
#include <QEnterEvent>
#include <QEvent>
#include <QPropertyAnimation>

class HoverButton : public QPushButton {
    Q_OBJECT

public:
    explicit HoverButton(QWidget *parent = nullptr);

    void setShadowGlowAnimation(QPropertyAnimation *animation);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QPropertyAnimation *shadowGlow;
};

#endif // HOVERBUTTON_H

