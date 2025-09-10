#ifndef PROTREEITEM_H
#define PROTREEITEM_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

class ProTreeItem : public QTreeWidgetItem
{
public:
    ProTreeItem(QTreeWidget *view, const QString &name, const QString &path, int type = Type);
    ProTreeItem(QTreeWidgetItem* parent, const QString &name, const QString &path, QTreeWidgetItem* root, int type = Type);
};

#endif // PROTREEITEM_H
