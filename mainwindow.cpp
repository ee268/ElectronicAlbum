#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QAction>
#include <QScreen>
#include <QRect>
#include <QDebug>
#include "wizard.h"
#include "protree.h"
#include <QFileDialog>
#include "protreewidget.h"
#include "picshowdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QRect screenSize = QGuiApplication::primaryScreen()->geometry();
    this->setMinimumSize(screenSize.width() / 2, screenSize.height() / 2);

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
    connect(act_open_pro, &QAction::triggered, this, &MainWindow::SlotOpenPro);

    _protree = new ProTree(this);
    ui->proLayout->addWidget(_protree);

    QTreeWidget* tree_widget = dynamic_cast<ProTree*>(_protree)->GetTreeWidget();
    auto* pro_tree_widget = dynamic_cast<ProTreeWidget*>(tree_widget);

    connect(this, &MainWindow::SigOpenPro, pro_tree_widget, &ProTreeWidget::SlotOpenPro);

    _picshow = new PicShowDialog();
    ui->picLayout->addWidget(_picshow);

    auto* pro_pic_show = dynamic_cast<PicShowDialog*>(_picshow);
    connect(pro_tree_widget, &ProTreeWidget::SigUpdateSelected, pro_pic_show, &PicShowDialog::SlotSelectItem);

    connect(pro_pic_show, &PicShowDialog::SigNextClicked, pro_tree_widget, &ProTreeWidget::SlotNextShow);
    connect(pro_pic_show, &PicShowDialog::SigPreClicked, pro_tree_widget, &ProTreeWidget::SlotPreShow);

    connect(pro_tree_widget, &ProTreeWidget::SigUpdateSelected, pro_pic_show, &PicShowDialog::SlotSelectItem);

    connect(pro_tree_widget, &ProTreeWidget::SigClearSelected, pro_pic_show, &PicShowDialog::SlotDeleteItem);

    connect(act_music, &QAction::triggered, pro_tree_widget, &ProTreeWidget::SlotSetMusic);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    auto* pro_pic_show = dynamic_cast<PicShowDialog*>(_picshow);
    pro_pic_show->ReloadPic();
    QMainWindow::resizeEvent(event);
}

void MainWindow::SlotCreatePro()
{
    qDebug() << "act_create_pro triggered SlotCreatePro";
    Wizard wizard(this);
    wizard.setWindowTitle(tr("创建项目"));

    auto* page = wizard.page(0);
    page->setTitle(tr("设置项目配置"));
    connect(&wizard, &Wizard::SigProSettings, dynamic_cast<ProTree*>(_protree), &ProTree::AddProTree);

    wizard.show();
    wizard.exec();
    disconnect(&wizard);
}

void MainWindow::SlotOpenPro()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle(tr("选择导入的文件夹"));
    file_dialog.setDirectory(QDir::currentPath());
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (file_dialog.exec()) {
        fileNames = file_dialog.selectedFiles();
    }

    if (fileNames.length() <= 0) {
        return;
    }

    QString import_path = fileNames.at(0);

    // qDebug() << "发送信号SigOpenPro";
    emit SigOpenPro(import_path);
}
