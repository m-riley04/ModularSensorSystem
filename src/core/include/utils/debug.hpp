#pragma once
#include <gst/gst.h>
#include <gst/gstbin.h>
#include <QProcess>
#include <qdesktopservices.h>
#include <QUrl>
#include <chrono>

static inline QString debugDisplayGstBin(GstElement* element, bool useUniqueName = false) {
    // Run Graphviz to generate the PNG
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString debugDumpDir = env.value("GST_DEBUG_DUMP_DOT_DIR", "");

    if (debugDumpDir.isEmpty()) {
        return "The GST_DEBUG_DUMP_DOT_DIR environment variable is not set. Please set it to a valid directory to save the pipeline/bin debug files.";
    }

    // Export the pipeline to a DOT file
    QString binDotFileName = "bin_debug";
    if (!useUniqueName) gst_debug_bin_to_dot_file(GST_BIN(element), GST_DEBUG_GRAPH_SHOW_ALL, binDotFileName.toStdString().c_str());
    else {
        // Get the raw data so I can write the file myself so I know what the name of the file is
        gchar* data = gst_debug_bin_to_dot_data(GST_BIN(element), GST_DEBUG_GRAPH_SHOW_ALL);
        std::chrono::time_point<std::chrono::system_clock> timestamp = std::chrono::system_clock::now();
        const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(timestamp.time_since_epoch()).count(); // thanks https://stackoverflow.com/questions/43872601/convert-timestamp-to-nanoseconds-qt
        binDotFileName += QString::fromStdString(std::to_string(ns)); // Append manually-tracked timestamp
        QString filePath = debugDumpDir + "/" + binDotFileName + ".dot";
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return "Could not open file '" + filePath + "' for writing.";

        QTextStream out(&file);
        out << data;
        file.close();
    }

    

    // dot should be in path
    // Run command: dot -Tpng pipeline_debug.dot -o pipeline_debug.png
    QString dotPath = "dot";
    QString inputDotFile = debugDumpDir + "/" + binDotFileName + ".dot";
    QString outputPngFile = debugDumpDir + "/" + binDotFileName + ".png";
    QProcess dotProcess;
    QStringList arguments;
    arguments << "-Tpng" << inputDotFile << "-o" << outputPngFile;
    dotProcess.start(dotPath, arguments);
    dotProcess.waitForFinished();
    if (dotProcess.exitCode() != 0) {
        return "Could not generate bin debug PNG. Please ensure Graphviz is installed and 'dot' is in your system PATH.";
    }

    // Open the PNG file
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(outputPngFile))) {
        return "Could not open the bin debug PNG file.";
    }

    // Return empty string to indicate fine
    return QString();
}