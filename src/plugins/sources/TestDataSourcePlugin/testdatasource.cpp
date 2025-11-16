#include "testdatasource.hpp"

TestDataSource::TestDataSource(const std::string& hardwareId, QObject* parent)
	: TestDataSource(getSourceInfo(hardwareId), parent)
{}

TestDataSource::TestDataSource(SourceInfo sourceInfo, QObject* parent)
	: Source(parent), m_id(sourceInfo.id), m_name(sourceInfo.displayName), 
	m_bin(std::make_unique<TestDataSourceBin>(this->uuid(), sourceInfo.id))
{
	m_cfg.sensorId = sourceInfo.id;

	connect(&m_timer, &QTimer::timeout, this, &TestDataSource::onTimerTimeout);
}

TestDataSource::~TestDataSource()
{}

SourceInfo TestDataSource::getSourceInfo(const std::string& id) const
{
	// Get the camera source from the id
	for (const auto& sourceInfo : getAvailableTestDataSources()) {
		if (sourceInfo.id == id) {
			return sourceInfo;
		}
	}

	// If not found, return an empty source info
	return SourceInfo();
}

void TestDataSource::createBinIfNeeded()
{
	if (!m_bin) {
		m_bin = std::make_unique<TestDataSourceBin>(this->uuid(), m_id);
	}
}

GstElement* TestDataSource::srcBin()
{
	createBinIfNeeded();
	return m_bin->bin();
}

void TestDataSource::onSessionStart()
{
	m_seq = 0;
	scheduleNextTick();
}

void TestDataSource::onSessionStop()
{
	m_timer.stop();

	// Reset bin
	m_bin.reset(nullptr);
}

void TestDataSource::onTimerTimeout()
{
	// Generate random value in [minValue, maxValue]
	double r01 = QRandomGenerator::global()->generateDouble();
	double val = m_cfg.minValue + r01 * (m_cfg.maxValue - m_cfg.minValue);

	m_seq++;
	m_bin->pushRandomSample(m_seq, val);

	// Schedule next random interval
	scheduleNextTick();
}

void TestDataSource::scheduleNextTick() {
	if (m_cfg.maxIntervalMs <= 0) return;

	int minMs = std::max(1, m_cfg.minIntervalMs);
	int maxMs = std::max(minMs, m_cfg.maxIntervalMs);

	int span = maxMs - minMs;
	int delay = minMs;
	if (span > 0) {
		delay += QRandomGenerator::global()->bounded(span + 1);
	}

	m_timer.start(delay);
}