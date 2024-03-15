#ifndef ACTIONVALUE_H
#define ACTIONVALUE_H

#include <QObject>
#include <QVariant>

class ActionValue : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant value MEMBER _value NOTIFY onValueChanged)

public:
    explicit ActionValue(QObject *parent=nullptr) : QObject(parent)
    {
    }

    void setValue(const QVariant &value)
    {
        if( _value == value)
            return;

        _value = value;
        emit onValueChanged(_value);
    }

    QVariant value() const
    {
        return _value;
    }

signals:
    void onValueChanged(QVariant value);

private:
    QVariant _value;
};


#endif // ACTIONVALUE_H
