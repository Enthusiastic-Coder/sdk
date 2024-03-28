#include <jibbs/action/Menu.h>

Menu::Menu(QObject *parent) : QObject(parent)
{

}

Menu::Menu(const QString& title, QObject *parent): QObject(parent)
{
    setTitle(title);
}

void Menu::clear()
{
    _varActions.clear();
    _subMenus.clear();
    _actions.clear();
    _varSubMenus.clear();

    emit actionsChanged();
}

void Menu::addMenu(Menu *m)
{
    _subMenus << m;
    _varSubMenus << QVariant::fromValue(m);
}

Menu* Menu::addMenu(const QString &title)
{
    Menu* m = new Menu(title, this);
    _subMenus << m;
    _varSubMenus << QVariant::fromValue(m);
    return m;
}

void Menu::addAction(Action *a)
{
    _varActions << QVariant::fromValue(a);
    _actions << a;
    emit lengthChanged();
    emit actionsChanged();
}

void Menu::removeAction(Action *a)
{
    _actions.removeOne(a);
    _varActions.removeOne(QVariant::fromValue(a));
    emit lengthChanged();
    emit actionsChanged();
}

void Menu::triggerNext()
{
    int index=-1;
    for(int i=0; i < _actions.size(); ++i)
        if( _actions[i]->isChecked() )
        {
            index = i;
            break;
        }

    index ++;
    if( index == _actions.size())
        index = 0;

    _actions[index]->trigger();
}

Action *Menu::addAction(const QString &text)
{
    Action* a = new Action(text, this);
    _varActions << QVariant::fromValue(a);
    _actions << a;
    emit actionsChanged();
    return a;
}

void Menu::setTitle(const QString &title)
{
    _title = title;
}

const QList<Action *>& Menu::actions() const
{
    return _actions;
}

Action *Menu::menuAction()
{
    return _menuAction;
}

void Menu::setData(const QVariant &var)
{
    _data = var;
}

QVariant Menu::data() const
{
    return _data;
}

int Menu::getLength() const
{
    return _varActions.length();
}
