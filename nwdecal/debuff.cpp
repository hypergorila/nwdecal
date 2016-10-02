#include "debuff.h"

Debuff::Debuff()
{

}

Debuff::Debuff(QString type, QString holder, QString name, double value, Category category)
{
    this->type = type;
    this->holder = holder;
    this->name = name;
    this->value = value;
    this->category = category;
    this->active = false;
}

QString Debuff::get_type()
{
    return type;
}

void Debuff::set_type(QString &value)
{
    type = value;
}

QString Debuff::get_holder()
{
    return holder;
}

void Debuff::set_holder(QString &value)
{
    holder = value;
}

QString Debuff::get_name()
{
    return name;
}

void Debuff::set_name(QString &value)
{
    name = value;
}

double Debuff::get_value()
{
    return value/100 - 1;
}

void Debuff::set_value(double value)
{
    value = value;
}

Category Debuff::get_category()
{
    return category;
}

void Debuff::set_category( Category &value)
{
    category = value;
}

Category Debuff::get_category_from_string(QString category)
{
    if (category == "dr1")
    {
        return dr1;
    }
    else if (category == "dr2")
    {
        return dr2;
    }
    else if (category == "def1")
    {
        return def1;
    }
    else if (category == "def2")
    {
        return def2;
    }
    else if (category == "dread")
    {
        return dread;
    }
    else
    {
        return multiplier;
    }
}

bool Debuff::is_active()
{
    return active;
}

void Debuff::set_active(bool value)
{
    active = value;
}
