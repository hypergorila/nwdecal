#include "nwdecal.h"
#include "ui_nwdecal.h"

NWDECal::NWDECal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NWDECal)
{
    ui->setupUi(this);
    mapper = new QSignalMapper(this);
    lbl_effectivness_73 = new QLabel();
    lbl_effectivness_under_73 = new QLabel();

    load_xml();
    set_gui();
    update_effectivness();
}


void NWDECal::update_effectivness()
{
    lbl_effectivness_73->setText("lvl = 73 effectiveness: " + QString::number(calculate_effectivness(.75), 'f', 10));
    lbl_effectivness_under_73->setText("lvl < 73 effectiveness: " + QString::number(calculate_effectivness(1), 'f', 10));
}


NWDECal::~NWDECal()
{
    delete ui;
}

void NWDECal::handle_state_change(int i)
{
    change_debuff_state(i);
    update_effectivness();
}

void NWDECal::handle_ri_change(QString ri)
{
    bool ok;
    this->ri = ri.toDouble(&ok);
    if (ok)
    {
        le_ri->setStyleSheet("color:black");
    }
    else
    {
        le_ri->setStyleSheet("color:red");
    }
    update_effectivness();
}

void NWDECal::handle_dr_change(QString dr)
{
    bool ok;
    this->dr = dr.toDouble(&ok);
    if (ok)
    {
        le_dr->setStyleSheet("color:black");
    }
    else
    {
        le_dr->setStyleSheet("color:red");
    }
    update_effectivness();
}

void NWDECal::change_debuff_state(int i)
{
    switch (debuffs[i].get_category()) {
    case dr1:
        if (dr1l.contains(i)) dr1l.removeOne(i);
        else dr1l.append(i);
    break;
    case dr2:
        if (dr2l.contains(i)) dr2l.removeOne(i);
        else dr2l.append(i);
    break;
    case def1:
        if (def1l.contains(i)) def1l.removeOne(i);
        else def1l.append(i);
    break;
    case def2:
        if (def2l.contains(i)) def2l.removeOne(i);
        else def2l.append(i);
    break;
    case dread:
        if (dreadl.contains(i)) dreadl.removeOne(i);
        else dreadl.append(i);
    break;
    case multiplier:
        if (multiplierl.contains(i)) multiplierl.removeOne(i);
        else multiplierl.append(i);
    break;
    }
}



void NWDECal::load_xml()
{
    QFile file("debuffs.xml");
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        return;
    }

    QDomNodeList debuffs_node_list = doc.elementsByTagName("debuff");
    for (int i = 0; i < debuffs_node_list.size(); i++)
    {
        QDomNode debuff_node = debuffs_node_list.item(i);
        QDomElement type = debuff_node.firstChildElement("type");
        QDomElement holder = debuff_node.firstChildElement("holder");
        QDomElement name = debuff_node.firstChildElement("name");
        QDomElement value = debuff_node.firstChildElement("value");
        QDomElement category = debuff_node.firstChildElement("category");
        Debuff deb(type.text(), holder.text(), name.text(), value.text().toDouble(), Debuff::get_category_from_string(category.text()));
        debuffs.append(deb);
    }
}

void NWDECal::set_gui()
{
    QMap<QString, QPoint*> holder_coordinates;
    QMap<QString, QVBoxLayout*> holder_layout;

    int n_holder = 0;

    for (int i = 0; i < debuffs.size(); i++)
    {
        QPoint *coord;
        Debuff deb = debuffs.at(i);
        QString holder = deb.get_holder();
        QVBoxLayout *vlayout = NULL;
        if (holder_coordinates.contains(holder))
        {
            coord = holder_coordinates.value(holder);
            vlayout = holder_layout.value(holder);
        }
        else
        {
            coord = new QPoint(n_holder/8, n_holder%8);
            vlayout = new QVBoxLayout();
            ui->gl_debuffs->addLayout(vlayout, coord->x(), coord->y(), Qt::AlignTop);
            QLabel *label = new QLabel();
            label->setText(holder);
            vlayout->addWidget(label);
            holder_coordinates.insert(holder, coord);
            holder_layout.insert(holder, vlayout);
            n_holder += 1;
        }

        QCheckBox *cb = new QCheckBox(deb.get_name());
        vlayout->addWidget(cb);
        connect(cb, SIGNAL(stateChanged(int)), mapper, SLOT(map()));
        mapper->setMapping(cb, i);
    }

    connect(mapper, SIGNAL(mapped(int)), this, SLOT(handle_state_change(int)));

    QVBoxLayout *drrilayout = new QVBoxLayout();

    QLabel *lbl_dr = new QLabel();
    lbl_dr->setText("DR:");
    drrilayout->addWidget(lbl_dr);

    le_dr = new QLineEdit();
    le_dr->setFixedWidth(100);
    le_dr->setText("0.6");
    drrilayout->addWidget(le_dr);

    connect(le_dr, SIGNAL(textChanged(QString)), this, SLOT(handle_dr_change(QString)));
    handle_dr_change(le_dr->text());

    QLabel *lbl_ri = new QLabel();
    lbl_ri->setText("RI:");
    drrilayout->addWidget(lbl_ri);

    le_ri = new QLineEdit();
    le_ri->setFixedWidth(100);
    le_ri->setText("0.6");
    drrilayout->addWidget(le_ri);

    connect(le_ri, SIGNAL(textChanged(QString)), this, SLOT(handle_ri_change(QString)));
    handle_ri_change(le_ri->text());

    ui->gl_debuffs->addLayout(drrilayout, n_holder/8, n_holder%8, Qt::AlignTop);
    n_holder += 1;

    QVBoxLayout *effectivness_layout = new QVBoxLayout();

    effectivness_layout->addWidget(lbl_effectivness_73);
    effectivness_layout->addWidget(lbl_effectivness_under_73);

    ui->gl_debuffs->addLayout(effectivness_layout, n_holder/8, n_holder%8, Qt::AlignTop);
}

double NWDECal::calculate_effectivness(double factor)
{
    double sumdr1 = 0.0;
    double sumdr2 = 0.0;
    double sumdef1 = 0.0;
    double sumdef2 = 0.0;
    double sumdread = 0.0;
    double prodmultiplier = 0.0;

    if (ri > dr)
    {
        ri = dr;
    }

    foreach (int i, dr1l) {
        sumdr1 += debuffs[i].get_value();
    }
    foreach (int i, dr2l) {
        sumdr2 += debuffs[i].get_value();
    }
    foreach (int i, def1l) {
        sumdef1 += debuffs[i].get_value();
    }
    foreach (int i, def2l) {
        sumdef2 += debuffs[i].get_value();
    }
    foreach (int i, dreadl) {
        sumdread += debuffs[i].get_value();
    }
    foreach (int i, multiplierl) {
        prodmultiplier *= debuffs[i].get_value();
    }

    double eff = (1 - dr + sumdread *  factor + ri +
                  sumdr1 + sumdr2 * (1 - dr + sumdread)) * factor *
                  (1 + sumdef1 + sumdef2 * (1 - dr + sumdread) * factor) *
                  (1 + prodmultiplier);
    return eff;
}
