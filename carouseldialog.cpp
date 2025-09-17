#include "carouseldialog.h"
#include "ui_carouseldialog.h"
#include "previewlistwidget.h"
#include <QDebug>

CarouselDialog::CarouselDialog(QTreeWidgetItem *first_item, QTreeWidgetItem *last_item, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CarouselDialog)
    , _first_item(first_item)
    , _last_item(last_item)

{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    ui->slidepreBtn->SetIcons(
        ":/icon/previous.png",
        ":/icon/previous_hover.png",
        ":/icon/previous_press.png"
    );
    ui->slidenextBtn->SetIcons(
        ":/icon/next.png",
        ":/icon/next_hover.png",
        ":/icon/next_press.png"
    );

    ui->closeBtn->SetIcons(
        ":/icon/closeshow.png",
        ":/icon/closeshow_hover.png",
        ":/icon/closeshow_press.png"
    );

    ui->playBtn->SetIcons(
        ":/icon/play.png",
        ":/icon/play_hover.png",
        ":/icon/play_press.png",
        ":/icon/pause.png",
        ":/icon/pause_hover.png",
        ":/icon/pause_press.png"
    );

    auto* prelistWid = dynamic_cast<previewListWidget*>(ui->preListWidget);
    connect(ui->picAnimation, &PicAnimationWidget::SigUpPreList, prelistWid, &previewListWidget::SlotUpPreList);

    connect(ui->picAnimation, &PicAnimationWidget::SigSelectItem, prelistWid, &previewListWidget::SlotUpSelect);

    ui->picAnimation->SetPixmap(_first_item);
    ui->picAnimation->Start();
}

CarouselDialog::~CarouselDialog()
{
    delete ui;
}

void CarouselDialog::on_closeBtn_clicked()
{
    this->close();
    ui->picAnimation->Stop();
    disconnect(ui->picAnimation);
}

