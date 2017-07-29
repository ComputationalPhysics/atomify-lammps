import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import Atomify 1.0
import "qrc:/core"
import "qrc:/mobile/dashboard/controls"

Simulation {
    id: root
    name: "Patterns"
    description: "<p>When granular materials (such as sand and marbles) are stacked on a vibrating plate, they can end up producing standing wave patterns.</p>
<p>We only need to tune the vibrational frequency and amplitude to certain values.</p>
<p>It takes quite some time to reach a stable pattern, so check out 'Pattern final' if you don't want to wait.</p>
"
    customColorShaderCode: "
highp float zScaled = 0.2 + 0.8* (position.z + 7.5) / 5.0;
zScaled = clamp(zScaled, 0.0, 1.0);
customColor *= zScaled;
"
    initialCameraPosition: Qt.vector3d(0,0,50)
    controllers: [
        Summary {
            system: root.system
        }
    ]
}

