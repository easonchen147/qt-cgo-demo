//
// Created by Eason on 2022/5/15.
//
#include "dialog.h"
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QString>
#include <QTextCodec>
#include "../dll/goutil.h"
#include <Windows.h>
#include <QTextCodec>
#include <QtDebug>

using namespace std;

// cgo function typedef
typedef GoInt (*funcPtrAdd)(GoInt, GoInt);

typedef char *(*funcPtrConcat)(const char *a, const char *b, const char *out);
// end

QString GBKToUTF8(QString s) {
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    return codec->toUnicode(s.toUtf8());
}

/**
	 * 构造GoString结构体对象
	 * @param p
	 * @param n
	 * @return
	 */
GoString buildGoString(const char *p, size_t n) {
    //typedef struct { const char *p; ptrdiff_t n; } _GoString_;
    //typedef _GoString_ GoString;
    return {p, static_cast<ptrdiff_t>(n)};
}

//定义界面类的键和函数
class Dialog::Impl {
public:
    QLabel *add;
    QLineEdit *add_a;
    QLineEdit *add_b;
    QPushButton *button_add;

    QLabel *concat;
    QLineEdit *concat_a;
    QLineEdit *concat_b;
    QPushButton *button_concat;

    QGridLayout *grayReview;

    // cgo dll 库方法
    HINSTANCE cgoInstance;
    funcPtrAdd pFunc_Add;
    funcPtrConcat pFunc_Concat;

    void setupUi(QWidget *parent);

    void reTranslate(QWidget *parent);

    void signalSlot(QWidget *parent);
};


void Dialog::Impl::setupUi(QWidget *parent) {
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    add = new QLabel;
    add_a = new QLineEdit;
    add_b = new QLineEdit;
    button_add = new QPushButton;

    concat = new QLabel;
    concat_a = new QLineEdit;
    concat_b = new QLineEdit;
    button_concat = new QPushButton;

    grayReview = new QGridLayout(parent);
    grayReview->setContentsMargins(10, 10, 10, 10);

    grayReview->addWidget(add, 0, 0, 1, 1);
    grayReview->addWidget(add_a, 1, 0, 1, 1);
    grayReview->addWidget(add_b, 1, 1, 1, 1);
    grayReview->addWidget(button_add, 1, 2, 1, 1);

    grayReview->addWidget(concat, 3, 0, 1, 1);
    grayReview->addWidget(concat_a, 4, 0, 1, 1);
    grayReview->addWidget(concat_b, 4, 1, 1, 1);
    grayReview->addWidget(button_concat, 4, 2, 1, 1);

    // 加载cgo dll 库
    cgoInstance = LoadLibrary("../dll/goutil.dll");
    pFunc_Add = (funcPtrAdd) GetProcAddress(cgoInstance, "Add");
    pFunc_Concat = (funcPtrConcat) GetProcAddress(cgoInstance, "Concat");

    reTranslate(parent);
    signalSlot(parent);
}

//初始化名称size等
void Dialog::Impl::reTranslate(QWidget *parent) {
    add->setText("求两数和");
    button_add->setText("求和");

    concat->setText("拼接字符");
    button_concat->setText("拼接");

    add->setMinimumWidth(100);
    add_a->setMinimumWidth(100);
    add_b->setMinimumWidth(100);
    button_add->setMinimumWidth(50);

    concat->setMinimumWidth(100);
    concat_a->setMinimumWidth(100);
    concat_b->setMinimumWidth(100);
    button_concat->setMinimumWidth(50);

    parent->setFixedSize(320, 120);
}

//初始化信号槽
void Dialog::Impl::signalSlot(QWidget *parent) {
    connect(button_add, SIGNAL(clicked()), parent, SLOT(stClickedBtbAdd()));
    connect(button_concat, SIGNAL(clicked()), parent, SLOT(stClickedBtnConcat()));
}

void Dialog::stClickedBtbAdd() {
    QString a = m_Impl->add_a->text();
    int aInt = a.toInt();

    QString b = m_Impl->add_b->text();
    int bInt = b.toInt();

    int result = m_Impl->pFunc_Add(aInt, bInt);

    QMessageBox::about(NULL, "结果", QString("%1").arg(result));
}

void Dialog::stClickedBtnConcat() {
    QString a = m_Impl->concat_a->text();
    std::string stra = a.toStdString();

    QString b = m_Impl->concat_b->text();
    std::string strb = b.toStdString();

    // 这种形式声明的不需要释放内存空间，存在栈上的
    // char ch[100]={0};

    char * ch = new char[100];
    memset(ch, 0, 100);
    m_Impl->pFunc_Concat(stra.c_str(), strb.c_str(), ch);
    QString result = QString::fromUtf8(ch);

    QMessageBox::about(NULL, "结果", result);

    // cgo返回后需要手动释放内存空间
    delete []ch;
}

Dialog::Dialog(QWidget *parent) :
        QWidget(parent), m_Impl(new Impl) {
    //设置UI
    m_Impl->setupUi(this);
}

Dialog::~Dialog() = default;


