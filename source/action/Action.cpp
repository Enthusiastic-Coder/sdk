#include <jibbs/action/Action.h>
#include <jibbs/action/ActionGroup.h>

Action::Action(QObject *parent) : QObject(parent)
{
}

Action::Action(QString text, QObject *parent) : QObject(parent)
{
    setText(text);
}

void Action::setText(const QString text)
{
    if( _text == text)
        return;

    _text = text;
    emit textChanged(_text);
}

void Action::setData(QVariant var)
{
    _data = var;
    emit dataChanged(_data);
}

void Action::setDisabled(bool bDisabled)
{
    if( _isDisabled == bDisabled )
        return;

    _isDisabled = bDisabled;
    emit disabledChanged(_isDisabled);
}

void Action::setEnabled(bool bEnabled)
{
    if( _isDisabled == !bEnabled)
        return;

    _isDisabled = !bEnabled;
    emit disabledChanged(_isDisabled);

}

void Action::setGroup(ActionGroup *group)
{
    _group = group;
}

void Action::setVisible(bool visible)
{
    if( _isVisible == visible)
        return;

    _isVisible = visible;
    emit visibilityChanged(_isVisible);
}

bool Action::isChecked() const
{
    return _isChecked;
}

bool Action::isVisible() const
{
    return _isVisible;
}

bool Action::isDisabled() const
{
    return _isDisabled;
}

void Action::toggle()
{
    setChecked(!isChecked());
}

QString Action::text() const
{
    return _text;
}

QVariant Action::data() const
{
    return _data;
}

void Action::trigger()
{
    if( _group == nullptr)
        emit triggered();
    else
        _group->trigger(this);
}

void Action::setChecked(bool v)
{
    if( _isChecked == v)
        return;

    _isChecked = v;

    if( _group == nullptr)
    {
        emit toggled(v);
    }
    else
    {
        auto actions = _group->actions();

        for(QVariant& var : actions)
        {
            Action* a = var.value<Action*>();

            a->_isChecked = a != this?false:v;
            emit a->toggled(a->_isChecked);
        }
    }
}
