#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include <QTreeWidget>
#include <QAction>
#include <QProgressDialog>
#include "protreethread.h"
#include "OpenProThread.h"
class CarouselDialog;

class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProTreeWidget(QWidget *parent = nullptr);
    void AddProToTree();
    void AddProToTree(const QString& name, const QString& path);

private:
    QSet<QString> _set_path;

    QTreeWidgetItem *_right_btn_item;
    QTreeWidgetItem *_active_item;
    QTreeWidgetItem *_selected_item;

    QAction* _action_import;
    QAction* _action_setstart;
    QAction* _action_closepro;
    QAction* _action_slideshow;

    QProgressDialog* _dialog_progress;
    QProgressDialog* _open_progress_dlg;

    std::shared_ptr<ProTreeThread> _thread_create_pro;
    std::shared_ptr<OpenProThread> _thread_open_pro;

    std::shared_ptr<CarouselDialog> _slide_show_dlg;

public slots:
    void SlotitemPressed(QTreeWidgetItem *item, int column);
    void SlotDoubleClickItem(QTreeWidgetItem *item, int column);
    void SlotImport();
    void slotSetActive();
    void SlotClosePro();
    void SlotSlideShow();

    void SlotUpdateProgress(int file_cnt);
    void SlotFinishProgress(int file_cnt);
    void SlotCancelProgress();

    void SlotUpdateOpenProgress(int file_cnt);
    void SlotFinishOpenProgress(int file_cnt);
    void SlotCancelOpenProgress();

    void SlotAddPicToTree(QTreeWidgetItem* parent, const QString &name, const QString &path, QTreeWidgetItem* root, int type, QTreeWidgetItem* preItem);
    // void SlotAddDirToTree(QTreeWidgetItem* parent, const QString &name, const QString &path, QTreeWidgetItem* root, int type);
    void SlotAddDirToTree(QTreeWidgetItem* item, const QString &name, const QString &path);

    void SlotAddEmptyItem();

    void SlotOpenPro(const QString& path);

    void SlotNextShow();
    void SlotPreShow();

signals:
    void SigCancelProgress();

    void SigCancelOpenProgress();

    void SigUpdateSelected(const QString&);

    void SigClearSelected();
};

#endif // PROTREEWIDGET_H
