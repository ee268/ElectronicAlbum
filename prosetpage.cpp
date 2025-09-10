#include "prosetpage.h"
#include "ui_prosetpage.h"
#include <QLineEdit>
#include <QDir>
#include <QFileDialog>

ProSetPage::ProSetPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::ProSetPage)
{
    ui->setupUi(this);
    registerField("proPath", ui->path_lineEdit);
    registerField("proName*", ui->name_lineEdit);
    connect(ui->name_lineEdit, &QLineEdit::textEdited, this, &ProSetPage::completeChanged);
    connect(ui->path_lineEdit, &QLineEdit::textEdited, this, &ProSetPage::completeChanged);
    QString curPath = QDir::currentPath();
    ui->path_lineEdit->setText(curPath);
    ui->path_lineEdit->setCursorPosition(ui->path_lineEdit->text().size());
    ui->path_lineEdit->setClearButtonEnabled(true);
    ui->name_lineEdit->setClearButtonEnabled(true);
}

ProSetPage::~ProSetPage()
{
    delete ui;
}

void ProSetPage::GetProSettings(QString &name, QString &path)
{
    name = ui->name_lineEdit->text();
    path = ui->path_lineEdit->text();
}

bool ProSetPage::isComplete() const
{
    if (ui->name_lineEdit->text() == "" || ui->path_lineEdit->text() == "") {
        return false;
    }

    QDir dir(ui->path_lineEdit->text());
    if (!dir.exists()) {
        ui->tips->setText("project path is not exists");
        return false;
    }

    QString absPath = dir.absoluteFilePath(ui->path_lineEdit->text());
    QDir dist_dir(absPath);
    if (!dir.exists()) {
        ui->tips->setText("project path is not exists");
        return false;
    }

    ui->tips->setText("");
    return QWizardPage::isComplete();
}

void ProSetPage::on_browse_btn_clicked()
{
    QFileDialog file_dialog;
    //打开目录
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle("选择导入的文件夹");

    auto path = QDir::currentPath();
    file_dialog.setDirectory(path);
    //详细模式打开
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (file_dialog.exec()) {
        fileNames = file_dialog.selectedFiles();
    }

    if (fileNames.size() <= 0) {
        return;
    }

    QString import_path = fileNames.at(0);
    ui->path_lineEdit->setText(import_path);
}

