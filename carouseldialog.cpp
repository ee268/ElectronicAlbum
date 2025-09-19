#include "carouseldialog.h"
#include "ui_carouseldialog.h"
#include "previewlistwidget.h"
#include <QDebug>
#include "protreewidget.h"

CarouselDialog::CarouselDialog(QTreeWidgetItem *first_item, QTreeWidgetItem *last_item, QList<QPair<QString, QTreeWidgetItem*>>* pixmap_list, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CarouselDialog)
    , _first_item(first_item)
    , _last_item(last_item)
    , _pixmap_list(pixmap_list)

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

    connect(ui->slidenextBtn, &QPushButton::clicked, this, &CarouselDialog::SlotSlideNext);
    connect(ui->slidepreBtn, &QPushButton::clicked, this, &CarouselDialog::SlotSlidePre);

    auto* prelistWid = dynamic_cast<previewListWidget*>(ui->preListWidget);
    connect(ui->picAnimation, &PicAnimationWidget::SigUpPreList, prelistWid, &previewListWidget::SlotUpPreList);

    connect(ui->picAnimation, &PicAnimationWidget::SigSelectItem, prelistWid, &previewListWidget::SlotUpSelect);

    connect(prelistWid, &previewListWidget::SigUpSelectShow, ui->picAnimation, &PicAnimationWidget::SlotUpSelectShow);

    connect(ui->playBtn, &PicStateBtn::clicked, ui->picAnimation, &PicAnimationWidget::SlotStartOrStop);

    connect(ui->picAnimation, &PicAnimationWidget::SigStart, ui->playBtn, &PicStateBtn::SlotStart);
    connect(ui->picAnimation, &PicAnimationWidget::SigStop, ui->playBtn, &PicStateBtn::SlotStop);

    auto* _protree_widget = dynamic_cast<ProTreeWidget*>(parent);
    connect(ui->picAnimation, &PicAnimationWidget::SigStartMusic, _protree_widget, &ProTreeWidget::SlotStartMusic);
    connect(ui->picAnimation, &PicAnimationWidget::SigStopMusic, _protree_widget, &ProTreeWidget::SlotStopMusic);

    ui->picAnimation->setPixmapList(_pixmap_list);
    ui->picAnimation->SetPixmap(_pixmap_list->at(0).second);
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

void CarouselDialog::SlotSlideNext()
{
    ui->picAnimation->SlideNext();
}

void CarouselDialog::SlotSlidePre()
{
    ui->picAnimation->SlidePre();
}

