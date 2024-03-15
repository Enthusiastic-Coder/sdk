#ifndef MENU_H
#define MENU_H

#include <QObject>
#include <QList>
#include "Action.h"

class Menu : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList actions MEMBER _varActions NOTIFY actionsChanged)
    Q_PROPERTY(Action* menuAction MEMBER _menuAction CONSTANT)
    Q_PROPERTY(QString title MEMBER _title CONSTANT)
    Q_PROPERTY(QVariantList subMenus MEMBER _varSubMenus CONSTANT)
    Q_PROPERTY(QVariant data READ data NOTIFY dataChanged)
public:
    explicit Menu(QObject *parent = nullptr);
    explicit Menu(const QString& title, QObject *parent = nullptr);

    void clear();
    void addMenu(Menu* m);
    Menu* addMenu(const QString& title);
    void addAction(Action* a);
    void removeAction(Action* a);

    void triggerNext();

    Action* addAction(const QString& text);
    void setTitle(const QString& );

    const QList<Action*>& actions() const;

    Action* menuAction();

    void setData(const QVariant& var);
    QVariant data() const;

signals:
    void dataChanged(QVariant);
    void actionsChanged();

public slots:

private:
    Action* _menuAction = new Action(this);
    QString _title;
    QVariantList _varActions;
    QList<Action*> _actions;
    QList<Menu*> _subMenus;
    QVariantList _varSubMenus;
    QVariant _data;
};

#endif // MENU_H
