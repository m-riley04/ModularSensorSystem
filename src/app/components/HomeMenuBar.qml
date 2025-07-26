import QtQuick
import QtQuick.Controls
import app.actions

MenuBar {
  id: menuBar
  Menu {
    title: qsTr("File")
    MenuItem {
      action: GlobalActions.quit
    }
    MenuItem {
      action: GlobalActions.restart
    }
  }
  Menu {
    title: qsTr("Edit")
  }
  Menu {
    title: qsTr("View")
    Menu {
      title: qsTr("Control Row")
      MenuItem {
        action: GlobalActions.viewControlRowPresetList
      }
      MenuItem {
        action: GlobalActions.viewControlRowSourcesList
      }
      MenuItem {
        action: GlobalActions.viewControlRowProcessorsList
      }
      MenuItem {
        action: GlobalActions.viewControlRowControls
      }
      MenuSeparator {}
      MenuItem {
        action: GlobalActions.viewControlRowEntireRow
      }
    }
    MenuSeparator {}
    MenuItem {
      action: GlobalActions.viewMenuBar
    }
    MenuItem {
      action: GlobalActions.viewToolbar
    }
    MenuItem {
      action: GlobalActions.viewCustomWindowHandle
    }
  }
  Menu {
    title: qsTr("Session")
    Menu {
      title: qsTr("Sources")
      MenuItem {
        action: SourcesActions.addSource
      }
      MenuItem {
        action: SourcesActions.removeSource
      }
      MenuItem {
        action: SourcesActions.configureSource
      }
    }
    Menu {
      title: qsTr("Processors")
      MenuItem {
        action: ProcessorsActions.addProcessor
      }
      MenuItem {
        action: ProcessorsActions.removeProcessor
      }
      MenuItem {
        action: ProcessorsActions.configureProcessor
      }
      MenuItem {
        action: ProcessorsActions.toggleProcessing
      }
    }
    Menu {
      title: qsTr("Recording")
    }
    MenuItem {
      action: SessionActions.clipping
    }
    MenuItem {
      action: SessionActions.sessionOptions
    }
  }
  Menu {
    title: qsTr("Plugins")
  }
  Menu {
    title: qsTr("About")
    MenuItem {
      action: GlobalActions.aboutGitHub
    }
    MenuItem {
      action: GlobalActions.aboutCredits
    }
  }
  Menu {
    title: qsTr("Help")
    MenuItem {
      action: GlobalActions.help
    }
  }
}
