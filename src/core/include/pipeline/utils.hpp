#pragma once

#include <QByteArray>
#include <gst/gstutils.h>
#include <QJsonDocument>
#include <QJsonObject>

/**
 * @brief Creates an NDJSON payload with a timestamp wrapping the provided JSON payload.
 * @param payload The JSON payload to wrap.
 * @param extraData Optional additional JSON data to include in the wrapper.
 * @return A QByteArray containing the NDJSON-formatted string with timestamp and payload.
 */
inline static QByteArray createTimestampedNdjson(QByteArray payload, QByteArray extraData = "") {
	GstClockTime timestamp = gst_util_get_timestamp();

	// Construct wrapping JSON with timestamp and payload
	QJsonObject wrapperObj;
	wrapperObj.insert("timestamp", static_cast<qint64>(timestamp));
	wrapperObj.insert("payload", QJsonDocument::fromJson(payload).object());
	wrapperObj.insert("extraData", QJsonDocument::fromJson(extraData).object());
	QJsonDocument wrapperDoc(wrapperObj);

	return wrapperDoc.toJson(QJsonDocument::Compact).append('\n');
}