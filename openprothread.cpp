#include "openprothread.h"
#include <QDir>
#include "protreeitem.h"
#include "const.h"

OpenProThread::OpenProThread(const QString &src_path, int file_cnt, QTreeWidget *self, QObject *parent)
    : QThread(parent)
    , _src_path(src_path)
    , _file_cnt(file_cnt)
    , _self(self)
    , _bstop(false)
    , _root(nullptr)
{

}

void OpenProThread::OpenProTree(const QString &src_path, int &file_cnt, QTreeWidget *self)
{
    QDir src_dir(src_path);
    auto name = src_dir.dirName();
    auto* item = new ProTreeItem(self, name, src_path, TreeItemPro);
    item->setData(0, Qt::DisplayRole, name);
    item->setData(0, Qt::DecorationRole, QIcon(":/icon/folder.png"));
    item->setData(0, Qt::ToolTipRole, src_path);
    _root = item;

    RecursiveProTree(src_path, file_cnt, self, _root, item, nullptr);
}

void OpenProThread::run()
{
    OpenProTree(_src_path, _file_cnt, _self);

    if (_bstop) {
        auto path = dynamic_cast<ProTreeItem*>(_root)->GetPath();
        auto index = _self->indexOfTopLevelItem(_root);
        delete _self->takeTopLevelItem(index);

        return;
    }

    emit SigFinishProgress(_file_cnt);
}

void OpenProThread::RecursiveProTree(const QString &src_path, int &file_cnt, QTreeWidget *self, QTreeWidgetItem *root, QTreeWidgetItem *parent, QTreeWidgetItem *preItem)
{
    QDir src_dir(src_path);
    src_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    src_dir.setSorting(QDir::Name);

    QFileInfoList list = src_dir.entryInfoList();

    for (int i = 0; i < list.size(); i++) {
        if (_bstop) {
            return;
        }

        QFileInfo fileInfo = list.at(i);
        bool bIsDir = fileInfo.isDir();

        if (bIsDir) {
            if (_bstop) {
                return;
            }

            file_cnt++;
            emit SigUpdateProgress(file_cnt);

            auto* item = new ProTreeItem(parent, fileInfo.fileName(), fileInfo.absoluteFilePath(), root, TreeItemDir);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/folder.png"));
            item->setData(0, Qt::ToolTipRole, fileInfo.absoluteFilePath());

            RecursiveProTree(fileInfo.absoluteFilePath(), file_cnt, self, root, item, preItem);
        }
        else {
            if (_bstop) {
                return;
            }

            const QString& suffix = fileInfo.completeSuffix();
            if (suffix != "png" && suffix != "jpeg" && suffix != "jpg") {
                continue;
            }

            file_cnt++;
            emit SigUpdateProgress(file_cnt);

            auto* item = new ProTreeItem(parent, fileInfo.fileName(), fileInfo.absoluteFilePath(), root, TreeItemPic);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/picture.png"));
            item->setData(0, Qt::ToolTipRole, fileInfo.absoluteFilePath());

            //如果前一个节点不为空，则让其指向当前item
            if (preItem) {
                auto* pre_proItem = dynamic_cast<ProTreeItem*>(preItem);
                pre_proItem->SetNextItem(item);
            }

            //如果为空，字自己就是前一个节点
            item->SetPreItem(preItem);
            preItem = item;
        }
    }
}

void OpenProThread::SlotCancelOpenProgress()
{
    this->_bstop = true;
}
