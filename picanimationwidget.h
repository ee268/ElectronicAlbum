#ifndef PICANIMATIONWIDGET_H
#define PICANIMATIONWIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>

class PicAnimationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PicAnimationWidget(QWidget *parent = nullptr);

    ~PicAnimationWidget();

    void setPixmapList(QList<QPair<QString, QTreeWidgetItem*>>* pixmap_list);

    void SetPixmap(QTreeWidgetItem* item);

    void Start();

    void Stop();

    void SlideNext();

    void SlidePre();

protected:
    virtual void paintEvent(QPaintEvent* event);

private:
    void UpSelectPixmap(QTreeWidgetItem* item);

    float _factor;
    QTimer* _timer;
    QPixmap _pixmap1;
    QPixmap _pixmap2;
    QTreeWidgetItem* _cur_item;
    QMap<QString, QTreeWidgetItem*> _map_items;
    bool _b_start;
    int _cur_pixmap_index;
    QList<QPair<QString, QTreeWidgetItem*>>* _pixmap_list;

private slots:
    void TimeOut();

public slots:
    void SlotUpSelectShow(QString path);

    void SlotStartOrStop();

signals:
    void SigUpPreList(QTreeWidgetItem*);
    void SigSelectItem(QTreeWidgetItem*);
    void SigStart();
    void SigStop();
    void SigStartMusic();
    void SigStopMusic();
};
#endif // PICANIMATIONWIDGET_H
