#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QClipboard>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QFileDialog>
#include <QFile>
#include <QErrorMessage>
#include <QMessageBox>
#include <QStandardPaths>
#include <QSettings>
#include <QPointer>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QtCore/private/qandroidextras_p.h>
#include <QJniObject>
#include "SmtpMime"
#include "filedownloader.h"
#include "clickablelabel.h"
#include "tdlgimg.h"
#include "tdlgstatus.h"
#include "tdlgoptions.h"
#include "tvars.h"
#include "tlisten.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class TDlgOptions;
class TVars;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static EmailAddress stringToEmail(const QString & str);
    SmtpClient *smtptr;
    FileDownloader *m_pImgCtrl;
    ClickableLabel *lblImg;
signals:
    void rightClicked();
    void ProgStatus(int Status);
    void descargado();
    void textShared(const QString &text);

private slots:
    void on_btnSend_clicked();
    void on_btnAttach_clicked();
    void descargaImagen(QString link,bool send);
    void on_btnGet_clicked();
    void loadImage();
    void loadImage2();
    void cargaImagen();
    void on_btnSave_clicked();
    void openImg(bool showProgress);
    void abreImagen();
    void nuevoTitulo();
    void on_btnOpt_clicked();
    void on_btClean_clicked();
    void on_btnNFile_clicked();
    void continueSendMail();

    void on_btnClear_clicked();

protected:
    void mousePressEvent(QMouseEvent* event);

private:
    Ui::MainWindow *ui;
    //QList<QFile*> files;
    //QList<MimeAttachment*> attachments;
    QClipboard *clipboard;
    void errorMessage(const QString & message);
    void cargaConfig();
    QString Url,LocalFile,Archivo;
    QPointer <TDlgImg> DlgImg;
    QPointer <TDlgStatus> DlgStatus;
    QPointer <TDlgOptions> DlgOptions;
    QPixmap buttonImage;
    bool Sended;
    bool downloaded;
    QUrl url;
    bool Send;
    bool manAttached;
};

#endif // MAINWINDOW_H
