#ifndef ACTION_H
#define ACTION_H

#include <QObject>
#include <QVariant>

class ActionGroup;

class Action : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(bool isChecked READ isChecked WRITE setChecked NOTIFY toggled)
    Q_PROPERTY(bool isVisible READ isVisible WRITE setVisible NOTIFY visibilityChanged)
    Q_PROPERTY(bool isDisabled READ isDisabled WRITE setDisabled NOTIFY disabledChanged)
    Q_PROPERTY(QVariant data MEMBER _data NOTIFY dataChanged)
public:
    explicit Action(QObject *parent = nullptr);
    explicit Action(QString text, QObject *parent = nullptr);

    void setData(QVariant var);
    void setGroup(ActionGroup* group);
    void setCheckable(bool) {/*DUMMY*/}

    bool isChecked() const;
    bool isVisible() const;
    bool isDisabled() const;

    void pulse();
    void toggle();
    QString text() const;
    QVariant data() const;

signals:
    void triggered();
    void toggled(bool);
    void visibilityChanged(bool);
    void disabledChanged(bool);
    void textChanged(QString);
    void dataChanged(QVariant);

public slots:
    void trigger();
    void setChecked(bool);
    void setText(const QString text);
    void setVisible(bool visible);
    void setDisabled(bool bDisabled);
    void setEnabled(bool bEnabled);

private:
    ActionGroup* _group = nullptr;
    bool _isVisible = true;
    bool _isChecked = false;
    bool _isDisabled = false;
    QString _text;
    QVariant _data;
};

#endif // ACTION_H
