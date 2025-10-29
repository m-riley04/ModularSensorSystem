#include "usbvideoplugin.h"

std::vector<SourceInfo> USBVideoPlugin::availableSources() const
{ 
    // Init gst if not already done
    if (!gst_is_initialized()) {
        gst_init(nullptr, nullptr);
    }

    GstDeviceMonitor* monitor = gst_device_monitor_new();
    GstCaps* caps = gst_caps_new_empty_simple("video/x-raw");
    gst_device_monitor_add_filter(monitor, "Video/Source", caps);
    gst_caps_unref(caps);
    gst_device_monitor_start(monitor);

    GList* devices = gst_device_monitor_get_devices(monitor);

    // Iterate over devices and add them to QList
    std::vector<SourceInfo> list;
    int idx = 0;
    for (GList* l = devices; l; l = l->next, ++idx) {
        auto* dev = GST_DEVICE(l->data);
        // Keep a ref we control
        g_object_ref(dev);
        gchar* name = gst_device_get_display_name(dev);
        SourceInfo info = { dev->parent.name, name ? name : "Unknown device", Source::Type::VIDEO };
        list.push_back(info);

        // Free our ref
        g_object_unref(dev);
        g_free(name);
    }

    // Frees
    gst_device_monitor_stop(monitor);
    gst_object_unref(monitor);
    g_list_free(devices);

    if (list.empty()) {
        qDebug() << "USBVideoPlugin: No video source devices found.";
    }

    return list;
}

Source* USBVideoPlugin::createSource(const std::string& id, QObject* parent)
{
    // Find the QCameraDevice by id from QMediaDevices
    QCameraDevice selected;
    for (const QCameraDevice& cam : QMediaDevices::videoInputs()) {
        if (cam.id() == id) { selected = cam; break; }
    }
    if (!selected.isNull()) {
        return new USBVideoSource(selected, parent);  // create the concrete source
    }
    return nullptr;
}
