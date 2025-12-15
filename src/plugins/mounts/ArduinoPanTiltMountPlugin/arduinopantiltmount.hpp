#pragma once

#include <qobject.h>
#include "features/mounts/mount.hpp"
#include "interfaces/capability/mounts/ipantiltmount.hpp"
#include "arduinopantiltmountbin.hpp"
#include "arduinopantiltmountrecorderbin.hpp"
#include <QSerialPort>

class ArduinoPanTiltMount 
	: public Mount
	, public IPanTiltMount
	, public IPipelineElement
	, public IRecordable
{
	Q_OBJECT

public:
	ArduinoPanTiltMount(const ElementInfo& element, QObject* parent);
	virtual ~ArduinoPanTiltMount() = default;

	// IPanTiltMount implementations
	bool moveTo(double panAngle, double tiltAngle) override final;
	Pose pose() const override final;
	bool recenter() override final;
	bool refreshInfo() override final;
	MountError error() const override final;

	// IPipelineElement implementations
	GstElement* gstSrcBin() override final;

	// IRecordable implementations
	GstElement* recorderSinkBin() override;
	std::string recorderFileExtension() const override;
	bool setRecordingFilePath(const std::string& filePath) override;
	bool startRecording() override;
	bool stopRecording() override;

private slots:
	bool sendCommand(const QString& command);
	bool sendInfoCommand();
	void readSerialData();

private:
	void createBinIfNeeded();
	void createRecorderBinIfNeeded();
	void parseResponse();
	void setError(const QString& errorMsg);

	/**
	 * @brief Creates an NDJSON payload from the given Pose object.
	 * @param pose The Pose object containing pan-tilt information.
	 * @return The NDJSON-formatted QByteArray payload.
	 */
	static QByteArray createNdjsonPayload(Pose& pose);
	static QString cleanUriField(const QString& field);

private:
	QSerialPort* m_serialPort = nullptr;
	Pose m_panTiltInfo;
	QByteArray m_readBuffer;
	MountError m_error;

	std::unique_ptr<ArduinoPanTiltMountBin> m_bin;
	std::unique_ptr<ArduinoPanTiltMountRecorderBin> m_recorderBin;

	std::string m_recordingFilePath;
};

