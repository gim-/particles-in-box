#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QObject>
#include <QMouseEvent>
#include <QLabel>

class ClickableLabel : public QLabel {
    Q_OBJECT
public:
    ClickableLabel(QWidget* parent);
    ClickableLabel(const QString& text, QWidget* parent);
    ~ClickableLabel();
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif // CLICKABLELABEL_H
