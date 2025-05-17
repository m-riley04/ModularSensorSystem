#include "unitconversionhelper.h"


double UnitConversionHelper::bytesToKilobytes(qint64 bytes) {
	return bytes / static_cast<double>(1024);
}

double UnitConversionHelper::bytesToMegabytes(qint64 bytes) {
	return bytes / static_cast<double>(1024 * 1024);
}

double UnitConversionHelper::bytesToGigabytes(qint64 bytes) {
	return bytes / static_cast<double>(1024 * 1024 * 1024);
}