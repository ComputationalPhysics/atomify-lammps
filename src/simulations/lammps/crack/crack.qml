import QtQuick 2.5
import "qrc:/core"
import "qrc:/mobile/dashboard/controls"

Simulation {
    id: root
    name: "2D Crack"
    description: "<p>This 2D simulation shows how a crack can propagate through a material.</p>
<p><b>Things to look for</b></p>
<p>Notice how the full system suddenly breaks apart. Can you understand why the temperature and pressure behave like they do?</p>"
    initialCameraPosition: Qt.vector3d(0.0, 0.0, -65.0)
    initialUpVector: Qt.vector3d(0,-1,0)
    controllers: [
        Summary {
            system: root.system
        },
        Thermometer {
            xLabel: "Time"
            yScale: 1000
        },
        Pressure {
            xLabel: "Time"
        }
    ]
}

