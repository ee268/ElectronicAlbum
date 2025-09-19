#include "previewlistwidget.h"
#include <QDebug>
#include <QPainter>
#include "const.h"
#include "protreeitem.h"
#include "previewlistitem.h"
#include <QGuiApplication>

previewListWidget::previewListWidget(QWidget *parent)
    : QListWidget(parent)
    , _global(0)
    , _last_index(17)
{
    this->setViewMode(QListWidget::IconMode);
    this->setIconSize(QSize(PREICON_SIZE, PREICON_SIZE));
    this->setSpacing(5);

    connect(this, &previewListWidget::itemPressed, this, &previewListWidget::SlotItemPressed);
}

previewListWidget::~previewListWidget()
{

}

void previewListWidget::AddListItem(const QString &path)
{
    QPixmap src_pixmap(path);
    src_pixmap = src_pixmap.scaled(PREICON_SIZE, PREICON_SIZE, Qt::KeepAspectRatio);

    QPixmap dst_pixmap(QSize(PREICON_SIZE, PREICON_SIZE));

    dst_pixmap.fill(QColor(220, 220, 220, 50));
    QPainter painter(&dst_pixmap);
    auto src_width = src_pixmap.width();
    auto src_height = src_pixmap.height();

    auto dst_width = dst_pixmap.width();
    auto dst_height = dst_pixmap.height();

    auto x = (dst_width - src_width) / 2;
    auto y = (dst_height - src_height) / 2;

    painter.drawPixmap(x, y, src_pixmap);
    _global++;

    PreviewListItem* preItem = new PreviewListItem(QIcon(dst_pixmap), path, _global, this);
    preItem->setSizeHint(QSize(PREITEM_SIZE, PREITEM_SIZE));
    this->addItem(preItem);
    _set_items[path] = preItem;

    if (_global == 1) {
        _pos_origin = this->pos();
    }
}

void previewListWidget::SlotUpPreList(QTreeWidgetItem *tree_item)
{
    // assert(tree_item)
    if (!tree_item) {
        qDebug() << "tree_item is empty";
        return;
    }

    auto* pro_item = dynamic_cast<ProTreeItem*>(tree_item);
    auto path = pro_item->GetPath();
    auto iter = _set_items.find(path);

    if (iter != _set_items.end()) {
        qDebug() << "path" << path << " exists";
        return;
    }

    AddListItem(path);
}

void previewListWidget::SlotUpSelect(QTreeWidgetItem *tree_item)
{
    if (!tree_item) {
        return;
    }

    auto* pro_item = dynamic_cast<ProTreeItem*>(tree_item);
    auto path = pro_item->GetPath();
    auto iter = _set_items.find(path);
    if (iter == _set_items.end()) {
        return;
    }

    auto* list_item = dynamic_cast<PreviewListItem*>(iter.value());
    auto index = list_item->GetIndex();

    if (index > 17) {
        auto pos_cur = this->pos();
        this->move(pos_cur.x() - (index - _last_index) * 100, pos_cur.y());
    }
    else {
        this->move(_pos_origin);
        _last_index = 17;
    }

    this->setCurrentItem(iter.value());
}

void previewListWidget::SlotItemPressed(QListWidgetItem *item)
{
    if (QGuiApplication::mouseButtons() != Qt::LeftButton) {
        return;
    }

    auto* list_item = dynamic_cast<PreviewListItem*>(item);
    auto cur_index = list_item->GetIndex();
    auto path = list_item->GetPath();

    this->setCurrentItem(item);
    emit SigUpSelectShow(path);
}
