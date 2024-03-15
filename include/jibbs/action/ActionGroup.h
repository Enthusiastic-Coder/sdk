#ifndef ACTIONGROUP_H
#define ACTIONGROUP_H

#include <QObject>
#include "Action.h"

class ActionGroup : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList actions MEMBER _actions CONSTANT)
    Q_PROPERTY(QString title MEMBER _title CONSTANT)
    Q_PROPERTY(Action* checkedAction READ checkedAction CONSTANT)
public:
    explicit ActionGroup(QObject *parent = nullptr);

    void addAction(Action* a);
    Action* checkedAction() const;
    QVariantList actions();
    void setTitle(const QString& );
    void triggerNext();

signals:
    void triggered(Action* a);

public slots:
    void trigger(Action *a);

private:
    QVariantList _actions;
    Action* _checkedAction = nullptr;
    QString _title;
    };

#endif // ACTIONGROUP_H
