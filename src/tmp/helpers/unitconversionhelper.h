#pragma once

#include <QObject>

enum InBytes {
	BYTE = 1,
	KILOBYTE = 1024,
	MEGABYTE = 1024 * 1024,
	GIGABYTE = 1024 * 1024 * 1024
};

class UnitConversionHelper
{
public:
	static double bytesToKilobytes(qint64 bytes);

	static double bytesToMegabytes(qint64 bytes);

	static double bytesToGigabytes(qint64 bytes);
};

