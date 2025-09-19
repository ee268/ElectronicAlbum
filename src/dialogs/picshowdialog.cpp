#include "include/picshowdialog.h"
#include "ui_picshowdialog.h"
#include <QDebug>

PicShowDialog::PicShowDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PicShowDialog)
{
    ui->setupUi(this);

    ui->previousButton->SetIcons(
        ":/icon/previous.png",
        ":/icon/previous_hover.png",
        ":/icon/previous_press.png"
    );

    ui->nextButton->SetIcons(
        ":/icon/next.png",
        ":/icon/next_hover.png",
        ":/icon/next_press.png"
    );

    QGraphicsOpacityEffect* opacity_pre = new QGraphicsOpacityEffect(this);
    opacity_pre->setOpacity(0);
    ui->previousButton->setGraphicsEffect(opacity_pre);

    QGraphicsOpacityEffect* opacity_next = new QGraphicsOpacityEffect(this);
    opacity_next->setOpacity(0);
    ui->nextButton->setGraphicsEffect(opacity_next);

    _animation_show_pre = new QPropertyAnimation(opacity_pre, "opacity", this);
    _animation_show_pre->setEasingCurve(QEasingCurve::Linear);
    _animation_show_pre->setDuration(500);

    _animation_show_next = new QPropertyAnimation(opacity_next, "opacity", this);
    _animation_show_next->setEasingCurve(QEasingCurve::Linear);
    _animation_show_next->setDuration(500);

    connect(ui->nextButton, &QPushButton::clicked, this, &PicShowDialog::SigNextClicked);
    connect(ui->previousButton, &QPushButton::clicked, this, &PicShowDialog::SigPreClicked);
}

PicShowDialog::~PicShowDialog()
{
    delete ui;
}

void PicShowDialog::ReloadPic()
{
    if (_selected_path != "") {
        _pix_map.load(_selected_path);
        double srcWidth = _pix_map.width(), srcHeight = _pix_map.height();
        double dstWidth = this->width() - (ui->widget->width() + ui->widget_2->width());
        double dstHeight = this->height();

        if (dstWidth < srcWidth || dstHeight < srcHeight) {
            double scale_ratio = std::min(dstWidth / srcWidth, dstHeight / srcHeight) + 0.1;
            dstWidth *= scale_ratio;
            dstHeight *= scale_ratio;
            qDebug() << dstWidth << dstHeight;
            // qDebug() << "ReloadPic: " << scale_ratio;
        }

        _pix_map = _pix_map.scaled(dstWidth, dstHeight, Qt::KeepAspectRatio);
        ui->label->setPixmap(_pix_map);
    }
}

bool PicShowDialog::event(QEvent *event)
{
    switch(event->type()) {
        case QEvent::Enter:
            ShowPreNextBtns(true);
            break;
        case QEvent::Leave:
            ShowPreNextBtns(false);
            break;
        default:
            break;
    }

    return QDialog::event(event);
}

void PicShowDialog::ShowPreNextBtns(bool b_show)
{
    if (!b_show && _b_btn_visible) {
        _animation_show_pre->stop();
        //从显示到隐藏
        _animation_show_pre->setStartValue(1);
        _animation_show_pre->setEndValue(0);
        _animation_show_pre->start();

        _animation_show_next->stop();
        //从显示到隐藏
        _animation_show_next->setStartValue(1);
        _animation_show_next->setEndValue(0);
        _animation_show_next->start();

        _b_btn_visible = false;
        return;
    }

    if (b_show && !_b_btn_visible) {
        _animation_show_pre->stop();
        //从隐藏到显示
        _animation_show_pre->setStartValue(0);
        _animation_show_pre->setEndValue(1);
        _animation_show_pre->start();

        _animation_show_next->stop();
        //从隐藏到显示
        _animation_show_next->setStartValue(0);
        _animation_show_next->setEndValue(1);
        _animation_show_next->start();

        _b_btn_visible = true;
        return;
    }
}

void PicShowDialog::SlotSelectItem(const QString &path)
{
    _selected_path = path;
    _pix_map.load(path);
    // qDebug() << "picture origin size: width " << _pix_map.width() << " height " << _pix_map.height();

    auto width = this->width() - (ui->widget->width() + ui->widget_2->width());
    auto height = this->height();
    // qDebug() << "SlotSelectItem picture size: width " << width << " height " << height;

    if (width < _pix_map.width() || height < _pix_map.height()) {
        double width_ratio = (double)width / (double)_pix_map.width();
        double height_ratio = (double)height / (double)_pix_map.height();
        double scale_ratio = std::min(width_ratio, height_ratio) - 0.06;

        width = _pix_map.width() * scale_ratio;
        height = _pix_map.height() * scale_ratio;
        // qDebug() << "SlotSelectItem: " << scale_ratio;
    }

    // width = this->width() - (ui->widget->width() + ui->widget_2->width());
    // height = this->height();

    //拉伸图片，KeepAspectRatio保证图片比例不受拉伸影响
    _pix_map = _pix_map.scaled(width, height, Qt::KeepAspectRatio);
    ui->label->setPixmap(_pix_map);
}

void PicShowDialog::SlotDeleteItem()
{
    _selected_path = "";
}
