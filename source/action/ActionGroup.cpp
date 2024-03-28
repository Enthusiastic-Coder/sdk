#include <jibbs/action/ActionGroup.h>

ActionGroup::ActionGroup(QObject *parent) : QObject(parent)
{
}

void ActionGroup::addAction(Action *a)
{
    _actions.append(QVariant::fromValue(a));
    a->setGroup(this);
    emit actionsChanged();
    emit lengthChanged();
}

Action *ActionGroup::checkedAction() const
{
    return _checkedAction;
}

QVariantList ActionGroup::actions()
{
    return _actions;
}

void ActionGroup::setTitle(const QString &title)
{
    _title = title;
}

void ActionGroup::triggerNext()
{
    if( _actions.size() < 2)
        return;

    int index = _actions.indexOf(QVariant::fromValue(_checkedAction)) + 1;
    if( index == _actions.size())
        index = 0;

    trigger(_actions[index].value<Action*>());
}

int ActionGroup::getLength() const
{
    return _actions.length();
}

void ActionGroup::trigger(Action *a)
{
    _checkedAction = a;
    a->setChecked(true);
    emit triggered(a);
}
