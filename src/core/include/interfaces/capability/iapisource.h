#pragma once

#include <QObject>
#include "idatasource.h"
#include <QUuid>
#include <QJsonDocument>

struct ApiDataSample
{
    QUuid sourceUuid;
	QJsonDocument json; // TODO/CONSIDER: maybe make this just a string or QByteArray?
    quint64 tNs;
};

class IApiSource : public IDataSource {

public:
    virtual ~IApiSource() = default;

signals:
    void apiDataReceived(ApiDataSample data);
};
