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

    void SetPixmap(QTreeWidgetItem* item);

    void Start();

    void Stop();

protected:
    virtual void paintEvent(QPaintEvent* event);

private:
    float _factor;
    QTimer* _timer;
    QPixmap _pixmap1;
    QPixmap _pixmap2;
    QTreeWidgetItem* _cur_item;
    QMap<QString, QTreeWidgetItem*> _map_items;
    bool _b_start;

private slots:
    void TimeOut();

signals:
};

#endif // PICANIMATIONWIDGET_H
