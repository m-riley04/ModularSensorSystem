#pragma once

#include <QObject>

enum TC001Pallette
{
	WHITE_HOT,
	BLACK_HOT,
	IRONBOW,
	RED_HOT,
	RAINBOW,
	JUNGLE,
	AURORA,
	CITY,
	NIGHT,
	SEPIA,
	LAVA
};

enum TC001GainMode
{
	WIDE_RANGE,
	HIGH_QUALITY
};

class TopdonTC001  : public QObject
{
	Q_OBJECT

private:

	double mGlobalTemperature;
	
	/// CONTROLS

	double mAtmosphericTemperature;
	double mReflectionTemperature;
	double mDistance;
	double mHumidity;
	double mEmmisivity;
	bool mIsTemperatureBar;
	bool mIsIsothermal;
	bool mIsStretchMode;
	TC001Pallette mPallette;
	TC001GainMode mGainMode;

public:
	TopdonTC001(QObject *parent);
	~TopdonTC001();

	double globalTemperature() const { return mGlobalTemperature; };

	/// CONTROLS

	double atmosphericTemperature() const { return mAtmosphericTemperature; };
	double reflectionTemperature() const { return mReflectionTemperature; };
	double distance() const { return mDistance; };
	double humidity() const { return mHumidity; };
	double emmisivity() const { return mEmmisivity; };
	bool isTemperatureBar() const { return mIsTemperatureBar; };
	bool isIsothermal() const { return mIsIsothermal; };
	bool isStretchMode() const { return mIsStretchMode; };
	TC001Pallette pallette() const { return mPallette; };
	TC001GainMode gainMode() const { return mGainMode; };

public slots:

	/// CONTROLS

	void setAtmosphericTemperature(double temperature);
	void setReflectionTemperature(double temperature);
	void setDistance(double distance);
	void setHumidity(double humidity);
	void setEmmisivity(double emmisivity);
	void setTemperatureBar(bool isTemperatureBar);
	void setIsothermal(bool isIsothermal);
	void setStretchMode(bool isStretchMode);
	void setGainMode(bool isWideRange); // Wide Range (~150 C) or High Quality (-20 to 150 C)

	void activateFCC();

signals:
	void globalTemperatureChanged(double temperature);

	/// CONTROLS
	void atmosphericTemperatureChanged(double temperature);
	void reflectionTemperatureChanged(double temperature);
	void distanceChanged(double distance);
	void humidityChanged(double humidity);
	void emmisivityChanged(double emmisivity);
	void temperatureBarChanged(bool isTemperatureBar);
	void isothermalChanged(bool isIsothermal);
	void stretchModeChanged(bool isStretchMode);
	void palletteChanged(TC001Pallette pallette);
	void globalTemperatureChanged(double temperature);
	void gainModeChanged(TC001GainMode gainMode);

	void fccActivated();
};
