#ifndef CAROUSELDIALOG_H
#define CAROUSELDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class CarouselDialog;
}

class CarouselDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CarouselDialog(QTreeWidgetItem* first_item, QTreeWidgetItem* last_item, QWidget *parent = nullptr);
    ~CarouselDialog();

private:
    Ui::CarouselDialog *ui;
    QTreeWidgetItem* _first_item;
    QTreeWidgetItem* _last_item;
};

#endif // CAROUSELDIALOG_H
