#ifndef PREVIEWLISTWIDGET_H
#define PREVIEWLISTWIDGET_H

#include <QListWidget>
#include <QTreeWidgetItem>

class previewListWidget : public QListWidget
{
    Q_OBJECT
public:
    previewListWidget(QWidget* parent = nullptr);

    virtual ~previewListWidget();

private:
    void AddListItem(const QString& path);

    QMap<QString, QListWidgetItem*> _set_items;
    int _global;
    QPoint _pos_origin;
    int _last_index;

public slots:
    void SlotUpPreList(QTreeWidgetItem* tree_item);
    void SlotUpSelect(QTreeWidgetItem* tree_item);
    void SlotItemPressed(QListWidgetItem *item);

signals:
    void SigUpSelectShow(QString path);
};

#endif // PREVIEWLISTWIDGET_H
