#ifndef DEBUFF_H
#define DEBUFF_H

#include <QObject>

enum Category {
    dr1, dr2, def1, def2, dread, multiplier
};

class Debuff
{
public:
    Debuff();
    Debuff(QString, QString, QString name, double value, Category category);

    QString get_type();
    void set_type(QString &value);

    QString get_holder();
    void set_holder(QString &value);

    QString get_name();
    void set_name(QString &value);

    double get_value();
    void set_value(double value);

    Category get_category();
    void set_category(Category &value);

    static Category get_category_from_string(QString category);

    bool is_active();
    void set_active(bool value);

private:
    QString type;
    QString holder;
    QString name;
    double value;
    Category category;
    bool active;
};

#endif // DEBUFF_H
