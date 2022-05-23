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
#include <QtDebug>
#include <cstdio>

using namespace std;

// cgo function typedef
typedef GoInt (*funcPtrAdd)(GoInt, GoInt);

typedef char *(*funcPtrConcat)(const char *a, const char *b, const char *out);

typedef void (*funcToNewGBKCStr)(const char *str, char **c);

typedef void (*funcHttpGet)(const char *path, char **c);

typedef void (*funcSetPerson)(struct Person *p);

typedef void (*funcJsonToPerson)(const char *d, struct Person *p);
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

    QLabel *createP;
    QLabel *lblName;
    QLabel *lblAge;
    QLineEdit *leName;
    QLineEdit *leAge;
    QPushButton *button_create;

    QGridLayout *grayReview;

    // cgo dll 库方法
    HINSTANCE cgoInstance;
    funcPtrAdd pFunc_Add;
    funcPtrConcat pFunc_Concat;
    funcToNewGBKCStr pFunc_ToNewGBKCStr;
    funcHttpGet pFunc_HttpGet;
    funcSetPerson pFunc_SetPerson;
    funcJsonToPerson pFunc_JsonToPerson;

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

    createP = new QLabel;
    lblName = new QLabel;
    lblAge = new QLabel;
    leName = new QLineEdit;
    leAge = new QLineEdit;
    button_create = new QPushButton;

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

    grayReview->addWidget(createP, 6, 0, 1, 1);
    grayReview->addWidget(lblName, 7, 0, 1, 1);
    grayReview->addWidget(leName, 7, 1, 1, 1);
    grayReview->addWidget(lblAge, 8, 0, 1, 1);
    grayReview->addWidget(leAge, 8, 1, 1, 1);
    grayReview->addWidget(button_create, 9, 1, 1, 1);

    // 加载cgo dll 库
    cgoInstance = LoadLibrary("../dll/goutil.dll");
    pFunc_Add = (funcPtrAdd) GetProcAddress(cgoInstance, "Add");
    pFunc_Concat = (funcPtrConcat) GetProcAddress(cgoInstance, "Concat");

    pFunc_ToNewGBKCStr = (funcToNewGBKCStr) GetProcAddress(cgoInstance, "ToNewGBKCStr");
    pFunc_HttpGet = (funcHttpGet) GetProcAddress(cgoInstance, "HttpGet");
    pFunc_SetPerson = (funcSetPerson) GetProcAddress(cgoInstance, "SetPerson");
    pFunc_JsonToPerson = (funcJsonToPerson) GetProcAddress(cgoInstance, "JsonToPerson");

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

    createP->setText("创建用户");
    lblName->setText("用户姓名");
    lblAge->setText("用户年龄");
    button_create->setText("创建用户");

    leName->setMinimumWidth(100);
    leAge->setMinimumWidth(100);
    button_create->setMinimumWidth(50);

    parent->setFixedSize(320, 240);
}

//初始化信号槽
void Dialog::Impl::signalSlot(QWidget *parent) {
    connect(button_add, SIGNAL(clicked()), parent, SLOT(stClickedBtbAdd()));
    connect(button_concat, SIGNAL(clicked()), parent, SLOT(stClickedBtnConcat()));
    connect(button_create, SIGNAL(clicked()), parent, SLOT(stClickedBtnCreate()));
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

    char* ch = new char[100]{};
    m_Impl->pFunc_Concat(stra.c_str(), strb.c_str(), ch);
    QString result = QString::fromUtf8(ch);

    Person p{0, ch};
    m_Impl->pFunc_SetPerson(&p);
    char *pName = new char[100]{};
    m_Impl->pFunc_ToNewGBKCStr(p.Name, &pName);

    QMessageBox::about(NULL, "结果", pName);

    // cgo返回后需要手动释放内存空间
    delete []ch;
    delete []pName;
}

void Dialog::stClickedBtnCreate() {
    QString age = m_Impl->leAge->text();
    QString name = m_Impl->leName->text();
    string stdName = name.toStdString();
    qDebug("name: %s", stdName.c_str());

    string fmt(R"({"Name":"%s", "Age":%d})");
    char buffer[120] = {};
    sprintf_s(buffer, sizeof(buffer), fmt.c_str(), stdName.c_str(), age.toInt());
    string json(buffer);
    qDebug("json: %s len: %d", json.c_str(), json.length());

    char* ppp = new char[40]{};
    auto* p = new Person{0, ppp};
    m_Impl->pFunc_JsonToPerson(json.c_str(), p);

    string pName(p->Name);
    delete []ppp;

    p->Name = pName.c_str();

    qDebug("back of person name: %s, %d", p->Name, p->Age);

    delete p;
}

Dialog::Dialog(QWidget *parent) :
        QWidget(parent), m_Impl(new Impl) {
    //设置UI
    m_Impl->setupUi(this);
}

Dialog::~Dialog() = default;


