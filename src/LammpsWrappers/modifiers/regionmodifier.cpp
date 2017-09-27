#include <QApplication>
#include "regionmodifier.h"
#include "../regions.h"
#include "../system.h"
#include "../atomdata.h"
RegionModifier::RegionModifier()
{

}


void RegionModifier::apply(AtomData &atomData)
{
    if(!enabled()) return;
    QList<CPRegion*> regions = m_system->regions()->regions();
    auto modifiers = QApplication::queryKeyboardModifiers();
    for(int atomIndex=0; atomIndex<atomData.size(); atomIndex++) {
        for(CPRegion* region : regions) {
            bool isInsideRegion = region->containsAtom(atomIndex); // Each group in LAMMPS is represented as a bit in an int. Bitwise or to check membership.
            if(isInsideRegion) {
                if(region->hovered()) atomData.colors[atomIndex] = QVector3D(1.0, 0.0, 0.0);
                if(!region->visible()) atomData.visible[atomIndex] = false;
            } else {
                if(region->hovered()) {
                    if(modifiers &= Qt::AltModifier) {
                        atomData.visible[atomIndex] = false;
                    }
                    else {
                        atomData.colors[atomIndex] = QVector3D(1.0, 1.0, 1.0);
                    }
                }
            }
        }
    }
}
