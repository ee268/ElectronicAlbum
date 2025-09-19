#ifndef OPENPROTHREAD_H
#define OPENPROTHREAD_H

#include <QThread>
#include <QTreeWidget>

class OpenProThread : public QThread
{
    Q_OBJECT
public:
    explicit OpenProThread(const QString& src_path, int file_cnt, QTreeWidget* self, QObject *parent = nullptr);

    void OpenProTree(const QString& src_path, int& file_cnt, QTreeWidget* self);

protected:
    virtual void run();

private:
    void RecursiveProTree(const QString& src_path, int & file_cnt, QTreeWidget* self, QTreeWidgetItem* root, QTreeWidgetItem* parent, QTreeWidgetItem* preItem);

    QString _src_path;
    int _file_cnt;
    QTreeWidget* _self;
    bool _bstop;
    QTreeWidgetItem* _root;

signals:
    void SigFinishProgress(int);
    void SigUpdateProgress(int);

public slots:
    void SlotCancelOpenProgress();
};

#endif // OPENPROTHREAD_H
