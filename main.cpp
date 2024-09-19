#include "mainwindow.h"
#include <QApplication>
/*#include <QGuiApplication>
#include <QtCore/private/qandroidextras_p.h>
#include <QJniObject>
#include <QCoreApplication>

class MyActivityReceiver : public QObject
{
    Q_OBJECT

public:
    MyActivityReceiver(QObject *parent = nullptr) : QObject(parent) {
        QtAndroid::runOnAndroidThread([this]() {
            QJniObject activity = QtAndroid::androidActivity();
            activity.callMethod<void>("setIntentListener", "(Landroid/content/Intent;)V", [this](QJniObject intent) {
                handleIntent(intent);
            });
        });
    }

private slots:
    void handleIntent(const QJniObject &intent) {
        QJniObject action = intent.callObjectMethod("getAction", "()Ljava/lang/String;");
        if (action.toString() == "android.intent.action.VIEW") {
            QJniObject data = intent.callObjectMethod("getDataString", "()Ljava/lang/String;");
            if (data.isValid()) {
                QString url = data.toString();
                // Procesar la URL recibida
                qDebug() << "URL recibida:" << url;
            }
        }
    }
};*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
