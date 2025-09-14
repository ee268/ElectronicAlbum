#include "protreewidget.h"
#include <QDir>
#include "protreeitem.h"
#include "const.h"
#include <QHeaderView>
#include <QGuiApplication>
// #include <QScreen>
// #include <QRect>
#include <QDebug>
#include <QMenu>
#include <QFileDialog>
#include "removeprodialog.h"

ProTreeWidget::ProTreeWidget(QWidget *parent)
    : _right_btn_item(nullptr)
    , _active_item(nullptr)
    , _selected_item(nullptr)
    , _dialog_progress(nullptr)
    , _open_progress_dlg(nullptr)
    , _thread_create_pro(nullptr)
    , _thread_open_pro(nullptr)
{
    // QRect screen_size = QGuiApplication::primaryScreen()->geometry();
    // this->setMinimumSize(screen_size.width(),screen_size.height());
    this->header()->hide();

    connect(this, &ProTreeWidget::itemPressed, this, &ProTreeWidget::SlotitemPressed);
    _action_import = new QAction(QIcon(""), tr("Import folder"), this);
    _action_setstart = new QAction(QIcon(""), tr("Set active project"), this);
    _action_closepro = new QAction(QIcon(""), tr("关闭项目"), this);
    _action_slideshow = new QAction(QIcon(""), tr("Carousel graph playback"), this);

    connect(_action_import, &QAction::triggered, this, &ProTreeWidget::SlotImport);
    connect(_action_setstart, &QAction::triggered, this, &ProTreeWidget::slotSetActive);
    connect(_action_closepro, &QAction::triggered, this, &ProTreeWidget::SlotClosePro);
    connect(this, &ProTreeWidget::itemDoubleClicked, this, &ProTreeWidget::SlotDoubleClickItem);
}

void ProTreeWidget::AddProToTree()
{
    // qDebug() << "call AddProToTree";
    auto* item = new ProTreeItem(this, "", "", TreeItemLine);
    item->setData(0, Qt::DisplayRole, "---------------");
}

void ProTreeWidget::AddProToTree(const QString &name, const QString &path)
{
    QDir dir(path);
    QString file_path = dir.absoluteFilePath(name);
    if (_set_path.find(file_path) != _set_path.end()) {
        return;
    }

    QDir pro_dir(file_path);
    if (!pro_dir.exists()) {
        bool enable = pro_dir.mkpath(file_path);

        if (!enable) {
            return;
        }
    }

    _set_path.insert(file_path);

    auto* item = new ProTreeItem(this, name, file_path, TreeItemPro);
    item->setData(0, Qt::DisplayRole, name);
    item->setData(0, Qt::DecorationRole, QIcon(":/icon/folder.png"));
    item->setData(0, Qt::ToolTipRole, file_path);

    this->addTopLevelItem(item);
}

void ProTreeWidget::SlotitemPressed(QTreeWidgetItem *pressedItem, int column)
{
    if (QGuiApplication::mouseButtons() == Qt::RightButton) {
        QMenu menu(this);
        int itemType = pressedItem->type();
        if (itemType == TreeItemPro) {
            _right_btn_item = pressedItem;
            menu.addAction(_action_import);
            menu.addAction(_action_setstart);
            menu.addAction(_action_closepro);
            menu.addAction(_action_slideshow);
            menu.exec(QCursor::pos()); //弹出菜单
        }
        else if (itemType == TreeItemLine) {
            return;
        }
    }
}

void ProTreeWidget::SlotDoubleClickItem(QTreeWidgetItem *item, int column)
{
    // qDebug() << "doubleClicked item";
    if (QGuiApplication::mouseButtons() == Qt::LeftButton) {
        auto* pro_tree_item = dynamic_cast<ProTreeItem*>(item);

        if (!pro_tree_item) {
            return;
        }

        int itemType = pro_tree_item->type();
        qDebug() << "doubleClick item type is " << itemType;

        if (itemType == TreeItemPic) {
            qDebug() << "doubleClicked item is TreeItemPic";
            emit SigUpdateSelected(pro_tree_item->GetPath());
            _selected_item = pro_tree_item;
        }
    }
}

void ProTreeWidget::SlotImport()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle(tr("选择导入的文件夹"));
    QString path = "";

    if (!_right_btn_item) {
        qDebug() << "_right_btn_item is empty";
        return;
    }

    path = dynamic_cast<ProTreeItem*>(_right_btn_item)->GetPath();
    file_dialog.setDirectory(path);
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (file_dialog.exec()) {
        //缓存选中的文件
        fileNames = file_dialog.selectedFiles();
    }

    if (fileNames.length() <= 0) {
        return;
    }

    QString import_path = fileNames.at(0);
    int file_cnt = 0;
    _dialog_progress = new QProgressDialog(this);

    qRegisterMetaType<QVector<int>>("QVector<int>");
    _thread_create_pro = std::make_shared<ProTreeThread>(
            std::ref(import_path),
            std::ref(path),
            _right_btn_item,
            file_cnt,
            this,
            _right_btn_item,
            nullptr
        );

    connect(_thread_create_pro.get(), &ProTreeThread::SigCreatePicToTree, this, &ProTreeWidget::SlotAddPicToTree);
    connect(_thread_create_pro.get(), &ProTreeThread::SigCreateDirToTree, this, &ProTreeWidget::SlotAddDirToTree);

    connect(_thread_create_pro.get(), &ProTreeThread::SigUpdateProgress, this, &ProTreeWidget::SlotUpdateProgress);
    connect(_thread_create_pro.get(), &ProTreeThread::SigFinishProgress, this, &ProTreeWidget::SlotFinishProgress);

    connect(_dialog_progress, &QProgressDialog::canceled, this, &ProTreeWidget::SlotCancelProgress);

    connect(this, &ProTreeWidget::SigCancelProgress, _thread_create_pro.get(), &ProTreeThread::SlotCancelProgress);

    _thread_create_pro->start();

    connect(_thread_create_pro.get(), &ProTreeThread::SigCreateEmptyToTree, this, &ProTreeWidget::SlotAddEmptyItem);

    _dialog_progress->setWindowTitle("Please wait....");
    _dialog_progress->setFixedWidth(PROGRESS_WIDTH);
    _dialog_progress->setRange(0, PROGRESS_WIDTH);
    _dialog_progress->exec();
}

void ProTreeWidget::slotSetActive()
{
    if (!_right_btn_item) {
        return;
    }

    QFont nullFont;
    nullFont.setBold(false);

    //判断之前的item是否为激活状态
    //是就取消
    if (_active_item) {
        _active_item->setFont(0, nullFont);
    }

    //设置当前选中的项目为激活状态
    _active_item = _right_btn_item;
    nullFont.setBold(true);
    _active_item->setFont(0, nullFont);
}

void ProTreeWidget::SlotClosePro()
{
    RemoveProDialog remove_pro_dialog;
    auto res = remove_pro_dialog.exec();

    if (res != QDialog::Accepted) {
        return;
    }

    bool b_remove = remove_pro_dialog.IsRemoved();
    auto index_right_btn = this->indexOfTopLevelItem(_right_btn_item);

    auto* protreeitem = dynamic_cast<ProTreeItem*>(_right_btn_item);
    auto* selecteditem = dynamic_cast<ProTreeItem*>(_selected_item);

    auto delete_path = protreeitem->GetPath();
    _set_path.remove(delete_path);

    //递归删除项目文件夹
    if (b_remove) {
        QDir delete_dir(delete_path);
        delete_dir.removeRecursively();
    }

    if (selecteditem && protreeitem == selecteditem->GetRoot()) {
        selecteditem = nullptr;
        _selected_item = nullptr;
        emit SigClearSelected();
    }

    //当前右击的项目和激活项目是同一个就将激活置空
    //防止_active_item指向已被delete的内存
    if (protreeitem == _active_item) {
        _active_item = nullptr;
    }

    delete this->takeTopLevelItem(index_right_btn);
}

void ProTreeWidget::SlotUpdateProgress(int file_cnt)
{
    if (!_dialog_progress) {
        return;
    }

    if (file_cnt >= PROGRESS_MAX) {
        _dialog_progress->setValue(file_cnt % PROGRESS_MAX);
    }
    else {
        _dialog_progress->setValue(file_cnt);
    }
}

void ProTreeWidget::SlotFinishProgress(int file_cnt)
{
    _dialog_progress->setValue(PROGRESS_MAX);
    _dialog_progress->deleteLater();
}

void ProTreeWidget::SlotCancelProgress()
{
    emit SigCancelProgress();
    delete _dialog_progress;
    _dialog_progress = nullptr;
}

void ProTreeWidget::SlotUpdateOpenProgress(int file_cnt)
{
    if (!_open_progress_dlg) {
        return;
    }

    if (file_cnt >= PROGRESS_MAX) {
        _open_progress_dlg->setValue(file_cnt % PROGRESS_MAX);
    }
    else {
        _open_progress_dlg->setValue(file_cnt);
    }
}

void ProTreeWidget::SlotFinishOpenProgress(int file_cnt)
{
    if (!_open_progress_dlg) {
        return;
    }

    _open_progress_dlg->setValue(PROGRESS_MAX);
    _open_progress_dlg->deleteLater();
}

void ProTreeWidget::SlotCancelOpenProgress()
{
    emit SigCancelOpenProgress();
    delete _open_progress_dlg;
    _open_progress_dlg = nullptr;
}

void ProTreeWidget::SlotAddPicToTree(QTreeWidgetItem *parent, const QString &name, const QString &path, QTreeWidgetItem *root, int type, QTreeWidgetItem *preItem)
{
    auto* item = new ProTreeItem(parent, name, path, root, type);
    item->setData(0, Qt::DisplayRole, name);
    item->setData(0, Qt::DecorationRole, QIcon(":/icon/picture.png"));
    item->setData(0, Qt::ToolTipRole, path);

    if (preItem) {
        auto* pre_proitem = dynamic_cast<ProTreeItem*>(preItem);
        pre_proitem->SetNextItem(item);
    }

    item->SetPreItem(preItem);
    preItem = item;
}

void ProTreeWidget::SlotAddDirToTree(QTreeWidgetItem *item, const QString &name, const QString &path)
{
    auto* sub_dir = dynamic_cast<ProTreeItem*>(item);
    sub_dir->setData(0, Qt::DisplayRole, name);
    sub_dir->setData(0, Qt::DecorationRole, QIcon(":/icon/folder.png"));
    sub_dir->setData(0, Qt::ToolTipRole, path);
}

void ProTreeWidget::SlotAddEmptyItem()
{
    this->AddProToTree();
}

void ProTreeWidget::SlotOpenPro(const QString &path)
{
    //如果打开过就返回
    // qDebug() << "调用SlotOpenPro";
    if (_set_path.find(path) != _set_path.end()) {
        return;
    }
    // qDebug() << "调用SlotOpenPro";

    _set_path.insert(path);

    int file_cnt = 0;
    QDir pro_dir(path);
    QString proname = pro_dir.dirName();

    qRegisterMetaType<QVector<int>>("QVector<int>");
    _thread_open_pro = std::make_shared<OpenProThread>(path, file_cnt, this, nullptr);

    _open_progress_dlg = new QProgressDialog(this);

    connect(_thread_open_pro.get(), &OpenProThread::SigUpdateProgress, this, &ProTreeWidget::SlotUpdateOpenProgress);
    connect(_thread_open_pro.get(), &OpenProThread::SigFinishProgress, this, &ProTreeWidget::SlotFinishOpenProgress);

    connect(_open_progress_dlg, &QProgressDialog::canceled, this, &ProTreeWidget::SlotCancelOpenProgress);

    connect(this, &ProTreeWidget::SigCancelOpenProgress, _thread_open_pro.get(), &OpenProThread::SlotCancelOpenProgress);

    _thread_open_pro->start();

    _open_progress_dlg->setWindowTitle("Please wait....");
    _open_progress_dlg->setFixedWidth(PROGRESS_WIDTH);
    _open_progress_dlg->setRange(0, PROGRESS_WIDTH);
    _open_progress_dlg->exec();
}

void ProTreeWidget::SlotNextShow()
{
    if (!_selected_item) {
        return;
    }

    auto *curItem = dynamic_cast<ProTreeItem*>(_selected_item)->GetNextItem();
    if (!curItem) {
        return;
    }

    emit SigUpdateSelected(curItem->GetPath());
    _selected_item = curItem;
    this->setCurrentItem(curItem); //设置当前激活的item
}

void ProTreeWidget::SlotPreShow()
{
    if (!_selected_item) {
        return;
    }

    auto *curItem = dynamic_cast<ProTreeItem*>(_selected_item)->GetPreItem();
    if (!curItem) {
        return;
    }

    emit SigUpdateSelected(curItem->GetPath());
    _selected_item = curItem;
    this->setCurrentItem(curItem); //设置当前激活的item
}

// void ProTreeWidget::SlotAddDirToTree(QTreeWidgetItem *parent, const QString &name, const QString &path, QTreeWidgetItem *root, int type)
// {
//     auto* item = new ProTreeItem(parent, name, path, root, TreeItemDir);
//     item->setData(0, Qt::DisplayRole, name);
//     item->setData(0, Qt::DecorationRole, QIcon(":/icon/folder.png"));
//     item->setData(0, Qt::ToolTipRole, path);
// }
