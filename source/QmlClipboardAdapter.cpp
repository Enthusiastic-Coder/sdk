#include "QmlClipboardAdapter.h"
#include <QGuiApplication>

QmlClipboardAdapter::QmlClipboardAdapter(QObject *parent) : QObject(parent)
{
    clipboard = QGuiApplication::clipboard();
}

void QmlClipboardAdapter::setText(QString text)
{
    clipboard->setText(text, QClipboard::Clipboard);
    clipboard->setText(text, QClipboard::Selection);
}

QString QmlClipboardAdapter::getText() const
{
    return clipboard->text();
}
