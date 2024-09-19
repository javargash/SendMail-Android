#include "tlisten.h"

TListen::TListen() {}

void TListen::listenText(const QString &message)
{
    QJniObject action = QJniObject::fromString("android.intent.action.VIEW");
    QJniObject intent("android/content/Intent");
    QJniObject activity=QNativeInterface::QAndroidApplication::context();
    if (action.isValid() && intent.isValid())
    {
        intent.callObjectMethod("setAction", "(Ljava/lang/String;)Landroid/content/Intent;", action.object<jstring>());
        QJniObject text = QJniObject::fromString(message);
        intent.callObjectMethod("putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                                QJniObject::fromString("android.intent.extra.TEXT").object<jstring>(),text.object<jstring>());
        intent.callObjectMethod("setType", "(Ljava/lang/String;)Landroid/content/Intent;",
                                QJniObject::fromString("text/plain").object<jstring>());
        QtAndroidPrivate::startActivity(intent.object<jobject>(),0,nullptr);
    }
}

