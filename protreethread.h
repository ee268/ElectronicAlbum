#ifndef PROTREETHREAD_H
#define PROTREETHREAD_H

#include <QTreeWidget>
#include <QThread>
#include <QObject>

class ProTreeThread : public QThread
{
    Q_OBJECT
public:
    ProTreeThread(const QString &src_path, const QString& dist_path, QTreeWidgetItem* parent_item,
                  int file_cnt, QTreeWidget* self, QTreeWidgetItem* root, QObject* parent = nullptr);
    ~ProTreeThread();

protected:
    virtual void run();

private:
    void CreateProTree(const QString &src_path, const QString& dist_path, QTreeWidgetItem* parent_item,
                       int& file_cnt, QTreeWidget* self, QTreeWidgetItem* root, QTreeWidgetItem* preItem = nullptr);

    QString _src_path;
    QString _dist_path;
    int _file_cnt;
    QTreeWidgetItem* _parent_item;
    QTreeWidget* _self;
    QTreeWidgetItem* _root;
    bool _bstop;

signals:
    void SigUpdateProgress(int);
    void SigFinishProgress(int);

    void SigCreatePicToTree(QTreeWidgetItem* parent, const QString &name, const QString &path, QTreeWidgetItem* root, int type, QTreeWidgetItem* preItem);
    // void SigCreateDirToTree(QTreeWidgetItem* parent, const QString &name, const QString &path, QTreeWidgetItem* root, int type);
    void SigCreateDirToTree(QTreeWidgetItem* item, const QString &name, const QString &path);

    void SigCreateEmptyToTree();

public slots:
    void SlotCancelProgress();
};

#endif // PROTREETHREAD_H
