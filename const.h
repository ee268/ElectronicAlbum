#ifndef CONST_H
#define CONST_H

enum TreeItemType {
    TreeItemPro = 1, //表示项目的条目
    TreeItemDir,     //表示项目的文件夹
    TreeItemPic,     //表示项目的图片
    TreeItemLine,    //表示项目分隔符
};

const int PROGRESS_WIDTH = 300;
const int PROGRESS_MAX = 300;

#endif // CONST_H
