#ifndef QMLCLIPBOARDADAPTER_H
#define QMLCLIPBOARDADAPTER_H

#include <QClipboard>
#include <QObject>

class QmlClipboardAdapter : public QObject
{
    Q_OBJECT
public:
    explicit QmlClipboardAdapter(QObject *parent = nullptr);

    Q_INVOKABLE void setText(QString text);
    Q_INVOKABLE QString getText() const;

private:
    QClipboard *clipboard;

};

#endif // QMLCLIPBOARDADAPTER_H
