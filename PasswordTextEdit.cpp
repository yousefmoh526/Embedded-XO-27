#include "PasswordTextEdit.h"

PasswordTextEdit::PasswordTextEdit(QWidget *parent) : QTextEdit(parent) {}

void PasswordTextEdit::keyPressEvent(QKeyEvent *event) {
    QString typedChar = event->text();

    // Check if Backspace key is pressed
    if (event->key() == Qt::Key_Backspace) {
        if (!actualText.isEmpty()) {
            actualText.chop(1); // Remove the last character from actualText
        }
        QTextEdit::keyPressEvent(event); // Call the default behavior for backspace
        return; // Exit early, as we've handled backspace
    }

    // Replace printable characters (except spaces) with '*'
    if (!typedChar.isEmpty() && !typedChar.contains(" ")) {
        QTextCursor cursor = this->textCursor();
        cursor.insertText("*"); // Display '*' instead of the actual character
        actualText += typedChar; // Save the actual character
    } else {
        // Call the default behavior for other keys (like Enter, arrows)
        QTextEdit::keyPressEvent(event);
    }
}
QString PasswordTextEdit::getActualText() const { return actualText; }
