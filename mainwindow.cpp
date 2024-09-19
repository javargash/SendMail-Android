#include "mainwindow.h"
#include "ui_mainwindow.h"

TVars Vars;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Sended=false;
    manAttached=false;
    //TListen::listenText("ui->lnEdtUrl->text()");

    lblImg=new ClickableLabel(this);
    connect(lblImg,SIGNAL(clicked()),this,SLOT(abreImagen()));
    connect(this,SIGNAL(rightClicked()),this,SLOT(nuevoTitulo()));
    connect(this,SIGNAL(descargado()),this,SLOT(continueSendMail()));
    lblImg->setMaximumSize(100,100);
    lblImg->setMinimumSize(100,100);
    lblImg->setScaledContents(true);
    ui->lyImg->addWidget(lblImg);
    cargaConfig();
}

MainWindow::~MainWindow()
{
    delete ui;
}
//----------------------------------------------------------

void MainWindow::on_btnSend_clicked()
{
    Sended=false;
    if(!ui->lnEdtUrl->text().isEmpty())
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        if(!ui->chkOpen->isChecked())
        {
            DlgStatus=new TDlgStatus(this);
            connect(this,SIGNAL(ProgStatus(int)),DlgStatus,SLOT(setStatus(int)));
            DlgStatus->show();
        }
        else
        {
            Sended=true;
        }
        descargaImagen(ui->lnEdtUrl->text(),true);

    }
    else
    {
        if(ui->lstWAttach->count()>0)
        {
            /*if(ui->lnEdtSubj->text().isEmpty())
                ui->lnEdtSubj->setText(ui->lstWAttach->item(0)->text());*/
            QApplication::setOverrideCursor(Qt::WaitCursor);
            if(!ui->chkOpen->isChecked())
            {
                DlgStatus=new TDlgStatus(this);
                connect(this,SIGNAL(ProgStatus(int)),DlgStatus,SLOT(setStatus(int)));
                DlgStatus->show();
            }
            else
            {
                Sended=true;
            }
            if(ui->lnEdtSubj->text().isEmpty())
            {
                if(Vars.SimpleName)
                    ui->lnEdtSubj->setText(QUrl(ui->lstWAttach->item(0)->text()).fileName());
                else
                    ui->lnEdtSubj->setText(ui->lstWAttach->item(0)->text());
            }
            //openImg(true);
            loadImage2();
            continueSendMail();
            //loadImage();
            //descargaImagen(ui->lstWAttach->item(0)->text(),true);
            //openImg(true);
        }
    }
    //Sended=false;
}
//----------------------------------------------------------

void MainWindow::on_btnAttach_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);

    if (dialog.exec())
    {
        ui->lstWAttach->addItems(dialog.selectedFiles());
        manAttached=true;
    }
}
//----------------------------------------------------------
EmailAddress MainWindow::stringToEmail(const QString &str)
{
    int p1 = str.indexOf("<");
    int p2 = str.indexOf(">");

    if (p1 == -1)
    {
        // no name, only email address
        return EmailAddress(str);
    }
    else
    {
        return EmailAddress(str.mid(p1 + 1, p2 - p1 - 1), str.left(p1));
    }

}
//----------------------------------------------------------
void MainWindow::errorMessage(const QString &message)
{
    QErrorMessage err (this);
    err.showMessage(message);
    err.exec();
}
//----------------------------------------------------------
void MainWindow::descargaImagen(QString link,bool send)
{
    if(Vars.PrevTagged)
    {
        link=link.replace("/m3_static_max/","/0/");
        link=link.replace("/p/","/0/");
    }
    QUrl imageUrl(link);
    Url=link;
    m_pImgCtrl = new FileDownloader(imageUrl, this);
    Send=send;
    connect(m_pImgCtrl, SIGNAL (downloaded()), this, SLOT (loadImage()));
}
//----------------------------------------------------------
void MainWindow::on_btnGet_clicked()
{
    descargaImagen(ui->lnEdtUrl->text(),false);
}
//----------------------------------------------------------
void MainWindow::loadImage()
{
    downloaded=false;
    buttonImage.loadFromData(m_pImgCtrl->downloadedData());
    int w = lblImg->width();
    int h = lblImg->height();
    lblImg->setPixmap(buttonImage.scaled(w,h,Qt::KeepAspectRatio));
    url=Url;
    QString nombre=url.fileName();
#if (defined (ANDROID) || defined (__ANDROID__))
    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::AppDataLocation);
    QFile archivo(tempPath+"/"+nombre);
    LocalFile=tempPath+"/"+nombre;
#endif
#if (defined(WIN32))
    QString ruta=QStandardPaths::writableLocation(QStandardPaths::TempLocation)+"/"+nombre;
    QFile archivo(ruta);
    LocalFile=ruta;
    #endif

    if (archivo.open(QIODevice::WriteOnly))
    {
        archivo.write(m_pImgCtrl->downloadedData());
        archivo.close();
        downloaded=true;
    }
    //openImg(false);
    if(Sended)
    {
        openImg(true);
    }
    else
    {
        if(ui->chkOpen->isChecked())
        {
            openImg(false);
        }
    }
    if(Send)
        emit descargado();
}
//----------------------------------------------------------
void MainWindow::loadImage2()
{
    downloaded=false;
    QString nombre=url.fileName();
#if (defined (ANDROID) || defined (__ANDROID__))
    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::AppDataLocation);
    QFile archivo(tempPath+"/"+nombre);
    LocalFile=tempPath+"/"+nombre;
    buttonImage.load(ui->lstWAttach->item(0)->text());
    int w = lblImg->width();
    int h = lblImg->height();
    lblImg->setPixmap(buttonImage.scaled(w,h,Qt::KeepAspectRatio));
#endif
#if (defined(WIN32))
    QString ruta=QStandardPaths::writableLocation(QStandardPaths::TempLocation)+"/"+nombre;
    QFile archivo(ruta);
    LocalFile=ruta;
#endif

    if (archivo.open(QIODevice::WriteOnly))
    {
        archivo.write(m_pImgCtrl->downloadedData());
        archivo.close();
        downloaded=true;
    }
    //openImg(false);
    if(Sended)
    {
        openImg(true);
    }
    else
    {
        if(ui->chkOpen->isChecked())
        {
            openImg(false);
        }
    }
    //if(Send)
      //  emit descargado();
}
//----------------------------------------------------------
void MainWindow::cargaImagen()
{    
    /*if(ui->chkOpen->isChecked())
        loadImage(false);
    else
        loadImage(true);*/
}
//----------------------------------------------------------
void MainWindow::on_btnSave_clicked()
{
    Archivo=QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/SendMail.ini";
    QSettings Settings(Archivo,QSettings::IniFormat);
    Settings.setValue("SMTP",ui->lnEdtSMTP->text());
    Settings.setValue("Port",ui->spPort->value());
    Settings.setValue("SSL",ui->chkSSL->isChecked());
    Settings.setValue("AutoOpen",ui->chkOpen->isChecked());
    Settings.setValue("Autenticate",ui->chkAuth->isChecked());
    Settings.setValue("User",ui->lnEdtUser->text());
    Settings.setValue("Password",ui->lnEdtPasswd->text());
    Settings.setValue("Sender",ui->lnEdtSender->text());
    Settings.setValue("Recipient",ui->lnEdtRecip->text());
    QMessageBox::information(this,"SendMail","Configuración guardada exitosamente",QMessageBox::Ok);
}
//----------------------------------------------------------
void MainWindow::cargaConfig()
{
    Archivo=QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/SendMail.ini";
    QSettings Settings(Archivo,QSettings::IniFormat);
    ui->lnEdtSMTP->setText(Settings.value("SMTP","").toString());
    ui->spPort->setValue(Settings.value("Port","").toInt());
    ui->chkSSL->setChecked(Settings.value("SSL","").toBool());
    ui->chkOpen->setChecked(Settings.value("AutoOpen","").toBool());
    ui->chkAuth->setChecked(Settings.value("Autenticate","").toBool());
    ui->lnEdtUser->setText(Settings.value("User","").toString());
    ui->lnEdtPasswd->setText(Settings.value("Password","").toString());
    ui->lnEdtSender->setText(Settings.value("Sender","").toString());
    ui->lnEdtRecip->setText(Settings.value("Recipient","").toString());
    Vars.PrevTagged=Settings.value("PrevTagged","false").toBool();
    Vars.SimpleName=Settings.value("SimpleName","false").toBool();
}
//----------------------------------------------------------
void MainWindow::openImg(bool showProgress)
{
    DlgImg=new TDlgImg(this,buttonImage,showProgress);
    DlgImg->setWindowFlag(Qt::WindowMaximizeButtonHint, true);
    connect(this,SIGNAL(ProgStatus(int)),DlgImg,SLOT(setStatus(int)));
    DlgImg->showMaximized();
}
//----------------------------------------------------------
void MainWindow::abreImagen()
{
    openImg(false);
}
//----------------------------------------------------------
void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::RightButton)
    {
        emit rightClicked();
    }
}
//----------------------------------------------------------
void MainWindow::nuevoTitulo()
{
    ui->lnEdtSubj->clear();
}
//----------------------------------------------------------
void MainWindow::on_btnOpt_clicked()
{
    DlgOptions=new TDlgOptions(this);
    DlgOptions->setWindowFlag(Qt::WindowMaximizeButtonHint, true);
    DlgOptions->exec();
}
//----------------------------------------------------------
void MainWindow::on_btClean_clicked()
{
    ui->lnEdtUrl->clear();
}
//----------------------------------------------------------
void MainWindow::on_btnNFile_clicked()
{
    if(!ui->lnEdtUrl->text().isEmpty())
    {
        clipboard->setText(QUrl(ui->lnEdtUrl->text()).fileName());
        ui->lnEdtUrl->clear();
        ui->statusbar->showMessage("Nombre del archivo copiado al porpapapeles exitosamente...",2000);
    }
}
//----------------------------------------------------------
void MainWindow::continueSendMail()
{
    emit ProgStatus(20);

    if(ui->lnEdtSubj->text().isEmpty())
    {
        if(Vars.SimpleName)
        {
            ui->lnEdtSubj->setText(QUrl(ui->lnEdtUrl->text()).fileName());
        }
        else
        {
            ui->lnEdtSubj->setText(ui->lnEdtUrl->text());
        }
    }
    QStringList stringList;
#if (defined (ANDROID) || defined (__ANDROID__))
    stringList<<QStandardPaths::writableLocation(QStandardPaths::StandardLocation::AppDataLocation)+"/"+QUrl(ui->lnEdtUrl->text()).fileName();
#endif
#if (defined(WIN32))
    stringList<<QStandardPaths::writableLocation(QStandardPaths::TempLocation)+"/"+QUrl(ui->lnEdtUrl->text()).fileName();
#endif
    ui->lstWAttach->addItems(stringList);
    QString host = ui->lnEdtSMTP->text();
    int port = ui->spPort->value();
    bool ssl = ui->chkSSL->isChecked();
    bool auth = ui->chkAuth->isChecked();
    QString user = ui->lnEdtUser->text();
    QString password = ui->lnEdtPasswd->text();

    EmailAddress sender = stringToEmail(ui->lnEdtSender->text());

    QStringList rcptStringList = ui->lnEdtRecip->text().split(';');

    QString subject = ui->lnEdtSubj->text();
    QString html = ui->txtMsg->toHtml();

    SmtpClient smtp (host, port, ssl ? SmtpClient::SslConnection : SmtpClient::TcpConnection);

    MimeMessage message;

    message.setSender(sender);
    message.setSubject(subject);

    for (int i = 0; i < rcptStringList.size(); ++i)
        message.addRecipient(stringToEmail(rcptStringList.at(i)));

    MimeHtml content;
    content.setHtml(html);

    message.addPart(&content);

    QList<QFile*> files;
    QList<MimeAttachment*> attachments;
    for (int i = 0; i < ui->lstWAttach->count(); ++i)
    {
        QFile* file = new QFile(ui->lstWAttach->item(i)->text());
        if(file->size()>0)
            files.append(file);

        MimeAttachment* attachment = new MimeAttachment(file);
        attachments.append(attachment);

        message.addPart(attachment);
    }

    smtp.connectToHost();
    emit ProgStatus(30);
    if (!smtp.waitForReadyConnected())
    {
        errorMessage("Conexión fallida.");
        return;
    }

    if (auth)
    {
        smtp.login(user, password);
        emit ProgStatus(50);
        if (!smtp.waitForAuthenticated())
        {
            errorMessage("Autenticación fallida.");
            return;
        }
    }

    smtp.sendMail(message);
    emit ProgStatus(70);
    if (!smtp.waitForMailSent())
    {
        errorMessage("Error al enviar correo.");
        return;
    }
    else
    {
        QApplication::restoreOverrideCursor();
        QMessageBox okMessage (this);
        if(DlgStatus!=nullptr)
        {
            DlgStatus->close();
        }
        emit ProgStatus(100);
        okMessage.setText("Correo enviado exitosamente.");
        okMessage.exec();
    }

    smtp.quit();

    for (auto file : files) {
        delete file;
    }

    for (auto attachment : attachments) {
        delete attachment;
    }
    ui->lstWAttach->clear();
    ui->lnEdtUrl->clear();
    QApplication::restoreOverrideCursor();
}
//----------------------------------------------------------

void MainWindow::on_btnClear_clicked()
{
    ui->lnEdtSubj->clear();
}


