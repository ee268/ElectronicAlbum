#ifndef CONST_H
#define CONST_H

enum TreeItemType {
    TreeItemPro = 1, //表示项目的条目
    TreeItemDir,     //表示项目的文件夹
    TreeItemPic,     //表示项目的图片
    TreeItemLine,    //表示项目分隔符
};

enum PicBtnState{
    PicBtnStateNormal = 1,
    PicBtnStateHover,
    PicBtnStatePress,

    PicBtnState2Normal = 4,
    PicBtnState2Hover,
    PicBtnState2Press,
};

const int PROGRESS_WIDTH = 300;
const int PROGRESS_MAX = 300;
const int PREITEM_SIZE = 100;
const int PREICON_SIZE = 90;

#endif // CONST_H
