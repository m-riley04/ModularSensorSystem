import QtQuick
import QtQuick.Window
import QtTest
import app.components

Item {
  Window {
    id: win
    width: 640
    height: 360
    visible: true
    property bool closeRequested: false
    onClosing: e => {
                 closeRequested = true
                 e.accepted = false
               } // keep window alive
    CustomTitleBar {
      id: _bar
      objectName: "titleBar"
    }
  }
  TestCase {
    id: testCase
    name: "CustomTitleBar"

    // Test fixtures
    property var win
    property var bar
    property var btnMin
    property var btnMax
    property var btnClose
    property var lblTitle
    property var imgIcon

    function resolveRefs() {
      bar = findChild(win, "titleBar")
      btnMin = findChild(bar, "minimizeButton")
      btnMax = findChild(bar, "maximizeButton")
      btnClose = findChild(bar, "closeButton")
      lblTitle = findChild(bar, "titleLabel")
      imgIcon = findChild(bar, "titleIcon")

      verify(bar,
             "Title bar not found (add objectName: 'bar' on the instance in the test)")
      verify(btnMin, "minimizeButton not found (add objectName)")
      verify(btnMax, "maximizeButton not found (add objectName)")
      verify(btnClose, "closeButton not found (add objectName)")
      verify(lblTitle, "titleLabel not found (add objectName)")
      verify(imgIcon, "iconLabel not found (add objectName)")
    }

    function initTestCase() {
      testCase.Window.window.requestActivate()
      tryCompare(testCase.Window.window, "active", true)
      resolveRefs()
      verify(waitForRendering(bar))
    }

    function cleanupTestCase() {
      if (win)
        win.destroy()
    }

    function test_sanity() {
      compare(1 + 1, 2)
      verify(true)
    }

    function test_title_alias_reads_and_writes() {
      bar.title = "My App"
      compare(bar.title, "My App")
      compare(lblTitle.text, "My App")
    }

    function test_icon_source_and_visibility() {
      // Empty => hidden
      bar.iconSource = ""
      compare(imgIcon.source, "")
      compare(imgIcon.visible, false)

      // Non-empty => visible (the component uses string emptiness for visibility)
      bar.iconSource = "qrc:/media/icons/app.png"
      compare(imgIcon.source, "qrc:/media/icons/app.png")
      compare(imgIcon.visible, true)
    }

    function test_minimize_button_minimizes_window() {
      win.showNormal()
      tryCompare(win, "visibility", Window.Windowed)

      mouseClick(btnMin, btnMin.width / 2, btnMin.height / 2, Qt.LeftButton)
      tryCompare(win, "visibility", Window.Minimized)
    }

    function test_maximize_and_restore() {
      // Start from normal
      win.showNormal()
      tryCompare(win, "visibility", Window.Windowed)

      // Maximize
      mouseClick(btnMax, btnMax.width / 2, btnMax.height / 2, Qt.LeftButton)
      tryCompare(win, "visibility", Window.Maximized)

      // Restore to normal
      mouseClick(btnMax, btnMax.width / 2, btnMax.height / 2, Qt.LeftButton)
      tryCompare(win, "visibility", Window.Windowed)
    }

    function test_close_button_requests_close() {
      win.closeRequested = false
      mouseClick(btnClose, btnClose.width / 2, btnClose.height / 2,
                 Qt.LeftButton)
      tryCompare(win, "closeRequested", true)
    }
  }
}
