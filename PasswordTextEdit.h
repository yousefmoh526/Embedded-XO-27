#ifndef PASSWORDTEXTEDIT_H
#define PASSWORDTEXTEDIT_H


#include <QTextEdit>
#include <QKeyEvent>
#include <QTextCursor>

class PasswordTextEdit : public QTextEdit {
    Q_OBJECT

public:
    explicit PasswordTextEdit(QWidget *parent = nullptr);

    QString getActualText() const;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QString actualText; // Stores the actual user input
};



#endif // PASSWORDTEXTEDIT_H
