#include "include/protreeitem.h"
#include "include/const.h"
#include <QDebug>

ProTreeItem::ProTreeItem(QTreeWidget *view, const QString &name, const QString &path, int type)
    : QTreeWidgetItem(view, type)
    , _path(path)
    , _name(name)
    , _root(this)
    , _pre_item(nullptr)
    , _next_item(nullptr)
{

}

ProTreeItem::ProTreeItem(QTreeWidgetItem *parent, const QString &name, const QString &path, QTreeWidgetItem *root, int type)
    : QTreeWidgetItem(parent, type)
    , _path(path)
    , _name(name)
    , _root(root)
    , _pre_item(nullptr)
    , _next_item(nullptr)
{

}

const QString &ProTreeItem::GetPath()
{
    return _path;
}

QTreeWidgetItem *ProTreeItem::GetRoot()
{
    return _root;
}

void ProTreeItem::SetPreItem(QTreeWidgetItem *item)
{
    _pre_item = item;
}

void ProTreeItem::SetNextItem(QTreeWidgetItem *item)
{
    _next_item = item;
}

// ProTreeItem *ProTreeItem::GetPreItem()
// {
//     return dynamic_cast<ProTreeItem*>(_pre_item);
// }

ProTreeItem *ProTreeItem::GetPreItem() {

    //如果_pre_item存在且是图片，直接返回
    if (_pre_item) {

        ProTreeItem* pre = dynamic_cast<ProTreeItem*>(_pre_item);

        if (pre && pre->type() == TreeItemPic) {

            return pre;
        }
    }

    QTreeWidgetItem* parent = this->parent();

    if (!parent)
        return nullptr;

    int index = parent->indexOfChild(this); //查找当前item父节点下该item的index

    qDebug() << "parent node subnode count: " << parent->childCount();

    for (int i = index - 1; i >= 0; --i) {

        ProTreeItem* sibling = dynamic_cast<ProTreeItem*>(parent->child(i));

        if (sibling->type() == TreeItemPic) {

            return sibling;

        } else if (sibling->type() == TreeItemDir) {

            ProTreeItem* last_pic = sibling->GetLastPicChild();

            // qDebug() << "last_pic path is " << last_pic->GetPath();

            if (last_pic) return last_pic;

        }

    } //当前是父级的第一个子项，前驱为父级

    if (parent->type() == TreeItemPic) {
        return dynamic_cast<ProTreeItem*>(parent);
    }

    return nullptr;
}

// ProTreeItem *ProTreeItem::GetNextItem()
// {
//     return dynamic_cast<ProTreeItem*>(_next_item);
// }

ProTreeItem* ProTreeItem::GetNextItem()
{
    if (_next_item) {
        ProTreeItem* next = dynamic_cast<ProTreeItem*>(_next_item);

        if (next) {
            if (next->type() == TreeItemPic) {
                qDebug() << "<TreeItemPic> _next_item is not empty, path is " << next->GetPath();
                return next;
            }

            //下一个节点是文件夹
            if (next->type() == TreeItemDir) {
                qDebug() << "<TreeItemDir> directory files: " << next->childCount();

                for (int i = 0; i < next->childCount(); ++i) {
                    ProTreeItem* child = dynamic_cast<ProTreeItem*>(next->child(i));

                    if (child->type() == TreeItemPic) {
                        return child;
                    }
                    else if (child->type() == TreeItemDir) {
                        ProTreeItem* pic_child = child->GetNextItem();
                        if (pic_child)
                            return pic_child;
                    }
                }
            }
        }
    }

    // //下一个节点是图片

    // QTreeWidgetItem* current = this;

    // while (current) {
    //     QTreeWidgetItem* parent = current->parent();
    //     int index = parent ? parent->indexOfChild(current) : -1;//查找同级下一个项
    //     // ProTreeItem* sibling_ = dynamic_cast<ProTreeItem*>(parent->child(index));
    //     // if (sibling_) {
    //     //     qDebug() << "index: " << index << "parent childCount: " << parent->childCount();
    //     //     qDebug() << sibling_->GetPath();
    //     // }

    //     if (parent && index < parent->childCount() - 1) {

    //         for (int i = index + 1; i < parent->childCount(); ++i) {
    //             ProTreeItem* sibling = dynamic_cast<ProTreeItem*>(parent->child(i));

    //             if (sibling->type() == TreeItemPic) {
    //                 qDebug() << "sibling type is TreeItemPic";
    //                 return sibling;
    //             }
    //             else if (sibling->type() == TreeItemDir) {
    //                 ProTreeItem* pic_child = sibling->GetNextItem();

    //                 if (pic_child)
    //                     qDebug() << "sibling type is TreeItemDir";
    //                     return pic_child;
    //             }
    //         }
    //     }

    //     current = parent;
    // }

    return nullptr;
}

ProTreeItem *ProTreeItem::GetLastPicChild()
{
    if (this->type() == TreeItemPic) {
        return nullptr;
    }

    auto child_count = this->childCount();
    if (child_count == 0) {
        return nullptr;
    }

    for (int i = child_count - 1; i >= 0; i--) {
        auto* last_child = this->child(i);
        auto* last_tree_item = dynamic_cast<ProTreeItem*>(last_child);

        int item_type = last_tree_item->type();
        if (item_type == TreeItemPic) {
            return last_tree_item;
        }

        last_child = last_tree_item->GetLastPicChild();
        if (!last_child) {
            continue;
        }

        last_tree_item = dynamic_cast<ProTreeItem*>(last_child);
        return last_tree_item;
    }

    return nullptr;
}

ProTreeItem *ProTreeItem::GetFirstPicChild()
{
    if (this->type() == TreeItemPic) {
        return nullptr;
    }

    auto child_count = this->childCount();
    if (child_count == 0) {
        return nullptr;
    }

    for (int i = 0; i < child_count; i++) {
        auto* first_child = this->child(i);
        auto first_tree_item = dynamic_cast<ProTreeItem*>(first_child);
        auto item_type = first_tree_item->type();

        if (item_type == TreeItemPic) {
            return first_tree_item;
        }

        first_child = first_tree_item->GetFirstPicChild();
        if (!first_child) {
            continue;
        }

        first_tree_item = dynamic_cast<ProTreeItem*>(first_child);
        return first_tree_item;
    }

    return nullptr;
}
