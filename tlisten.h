#ifndef TLISTEN_H
#define TLISTEN_H

#include <QObject>
#include <QtCore/private/qandroidextras_p.h>
#include <QJniObject>

class TListen
{
public:
    TListen();
    ~TListen();
    static void listenText(const QString &message);
};

#endif // TLISTEN_H
