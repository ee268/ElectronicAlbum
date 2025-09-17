#ifndef PREVIEWLISTITEM_H
#define PREVIEWLISTITEM_H

#include <QListWidgetItem>
#include <QObject>

class PreviewListItem : public QListWidgetItem
{
public:
    PreviewListItem(const QIcon& icon, const QString& text, const int& index, QListWidget *listview = nullptr, int type = Type);

    int GetIndex();
    QString GetPath();

private:
    QString _path;
    int _index;

};

#endif // PREVIEWLISTITEM_H
