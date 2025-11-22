#pragma once

#include <qobject.h>
#include "features/mounts/mount.hpp"

class TripodMount : public Mount
{
	Q_OBJECT

public:
	TripodMount(QObject* parent);
	TripodMount(std::string id, std::string name, QObject* parent);
	~TripodMount() = default;

	bool isDynamic() const override { return false; }
	bool isPosable() const override { return true; }
	bool isAutomaticallyPosed() const override { return false; }

	bool isPosable() const override { return false; }
	std::optional<Pose> pose() const override { return std::nullopt; }
	bool setPose(const Pose& newPose) override { return false; }

private:
	std::string m_id;
	std::string m_name;
	std::string m_displayName;
	std::string m_pluginId;
};

