#ifndef PICSHOWDIALOG_H
#define PICSHOWDIALOG_H

#include <QDialog>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QEvent>

namespace Ui {
class PicShowDialog;
}

class PicShowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PicShowDialog(QWidget *parent = nullptr);
    ~PicShowDialog();

    void ReloadPic();

protected:
    bool event(QEvent* event);

private:
    void ShowPreNextBtns(bool b_show);

    Ui::PicShowDialog *ui;
    QPropertyAnimation* _animation_show_pre;
    QPropertyAnimation* _animation_show_next;
    bool _b_btn_visible;

    QString _selected_path;
    QPixmap _pix_map;

public slots:
    void SlotSelectItem(const QString& path);

    void SlotDeleteItem();

signals:
    void SigPreClicked();
    void SigNextClicked();
};

#endif // PICSHOWDIALOG_H
