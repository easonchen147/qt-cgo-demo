//
// Created by Eason on 2022/5/15.
//

#ifndef QT_CGO_DEMO_DIALOG_H
#define QT_CGO_DEMO_DIALOG_H

#include <QWidget>

class Dialog : public QWidget {
    Q_OBJECT
    class Impl;

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void stClickedBtbAdd();
    void stClickedBtnConcat();
    void stClickedBtnCreate();

private:
    //使用智能指针处理界面的内容
    QScopedPointer <Impl> m_Impl;
};



#endif //QT_CGO_DEMO_DIALOG_H
