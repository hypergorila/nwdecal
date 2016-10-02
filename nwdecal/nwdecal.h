#ifndef NWDECAL_H
#define NWDECAL_H

#include <QMainWindow>
#include <QtXml>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QSignalMapper>

#include "debuff.h"

namespace Ui {
class NWDECal;
}

class NWDECal : public QMainWindow
{
    Q_OBJECT

public:
    explicit NWDECal(QWidget *parent = 0);
    ~NWDECal();

signals:
    state_changed(int i, bool);

private slots:
    void handle_state_change(int i);
    void handle_ri_change(QString ri);
    void handle_dr_change(QString dr);

private:
    Ui::NWDECal *ui;
    QList<Debuff> debuffs;
    QList<int> dr1l, dr2l, def1l, def2l, dreadl, multiplierl;
    QSignalMapper *mapper;
    QLabel *lbl_effectivness_73;
    QLabel *lbl_effectivness_under_73;
    QLineEdit *le_dr, *le_ri;
    double dr, ri;

    void load_xml();
    void set_gui();
    void change_debuff_state(int i);
    double calculate_effectivness(double factor);
    void update_effectivness();
};

#endif // NWDECAL_H
