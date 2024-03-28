#ifndef ACTIONGROUP_H
#define ACTIONGROUP_H

#include <QObject>
#include "Action.h"

class ActionGroup : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList actions MEMBER _actions NOTIFY actionsChanged)
    Q_PROPERTY(QString title MEMBER _title CONSTANT)
    Q_PROPERTY(Action* checkedAction READ checkedAction CONSTANT)
    Q_PROPERTY(int length READ getLength NOTIFY lengthChanged)
public:
    explicit ActionGroup(QObject *parent = nullptr);

    void addAction(Action* a);
    Action* checkedAction() const;
    QVariantList actions();
    void setTitle(const QString& );
    void triggerNext();

    int getLength() const;

signals:
    void triggered(Action* a);
    void actionsChanged();
    void lengthChanged();

public slots:
    void trigger(Action *a);

private:
    QVariantList _actions;
    Action* _checkedAction = nullptr;
    QString _title;
    };

#endif // ACTIONGROUP_H
