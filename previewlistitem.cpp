#include "previewlistitem.h"

PreviewListItem::PreviewListItem(const QIcon &icon, const QString &text, const int &index, QListWidget *listview, int type)
    : QListWidgetItem(icon, "", listview, type)
    , _path(text)
    , _index(index)
{

}

int PreviewListItem::GetIndex()
{
    return _index;
}

QString PreviewListItem::GetPath()
{
    return _path;
}
