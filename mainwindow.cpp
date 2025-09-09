#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QAction>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //创建菜单
    QMenu* menu_file = ui->menubar->addMenu(tr("文件(&F)"));

    //创建子菜单
    QAction* act_create_pro = new QAction(QIcon(""), tr("创建项目"), this);
    act_create_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    menu_file->addAction(act_create_pro);

    QAction* act_open_pro = new QAction(QIcon(""), tr("打开项目"), this);
    act_open_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    menu_file->addAction(act_open_pro);

    //创建设置菜单
    QMenu* menu_set = ui->menubar->addMenu(tr("设置(&S)"));

    //设置子菜单
    QAction* act_music = new QAction(QIcon(""), tr("背景音乐"), this);
    act_music->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
    menu_set->addAction(act_music);

    //连接信号和槽
    connect(act_create_pro, &QAction::triggered, this, &MainWindow::SlotCreatePro);
    // connect(act_open_pro, &QAction::triggered, this, )
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotCreatePro()
{
    qDebug() << "act_create_pro triggered SlotCreatePro";
}
