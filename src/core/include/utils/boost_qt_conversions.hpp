#pragma once

#include <boost/uuid.hpp>
#include <quuid.h>

/**
 * Converts a boost::uuids::uuid to a QUuid.
 * @param boostUuid The boost::uuids::uuid to convert.
 * @return The converted QUuid.
 */
inline QUuid boostUuidToQUuid(const boost::uuids::uuid& boostUuid)
{
	const auto data = boost::uuids::to_string(boostUuid);
	return QUuid(data);
}

/**
 * Converts a QUuid to a boost::uuids::uuid.
 * @param qUuid The QUuid to convert.
 * @return The converted boost::uuids::uuid.
 */
inline boost::uuids::uuid QUuidToBoostUuid(const QUuid& qUuid)
{
	const QByteArray byteArray = qUuid.toRfc4122();
	boost::uuids::uuid boostUuid;
	std::copy(byteArray.begin(), byteArray.end(), boostUuid.begin());
	return boostUuid;
}