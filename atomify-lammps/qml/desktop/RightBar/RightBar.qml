import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import Atomify 1.0
import "../../visualization"

Page {
    id: root
    property System system
    property AtomifyVisualizer visualizer
    focusPolicy: Qt.NoFocus

    function showExamples() {
        tabBar.currentIndex = 2
    }

    StackLayout {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex
        clip: true

        SimulationSummary {
            width: swipeView.width
            height: swipeView.height
            // contentHeight: 2*height
            system: root.system
            visualizer: root.visualizer
        }

        Rendering {
            width: swipeView.width
            height: swipeView.height
            visualizer: root.visualizer
        }
    }

    header: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        onCurrentIndexChanged: {
            visualizer.focus = true // TODO: must be a better way?
        }

        TabButton {
            text: "Simulation"
            font.pixelSize: 12
        }
        TabButton {
            text: "Rendering"
            font.pixelSize: 12
        }
    }

    footer: Item {
        height: 50
        width: parent.width
        Text {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            anchors.margins: 10
            textFormat: TextEdit.RichText
            font.family: "DejaVu Sans Mono"
            font.pixelSize: 16
            text: "
<style>
h2 { text-align: center; }
a { font-weight: bold; color: #56b1b4; text-decoration: none; }
p { color: white; }
</style>
<p>Please report bugs <a href=\"reportBug\">here</a>.</p>
"
            onLinkActivated: {
                if(link==="reportBug") {
                    Qt.openUrlExternally("https://github.com/ovilab/atomify-lammps/issues")
                }
            }
        }
    }
}
