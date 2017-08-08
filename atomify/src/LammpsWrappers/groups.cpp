#include "groups.h"
#include "../mysimulator.h"
#include <group.h>
using namespace LAMMPS_NS;

Groups::Groups(AtomifySimulator *simulator)
{
    Q_UNUSED(simulator)
}

void Groups::add(QString identifier) {
    if(m_dataMap.contains(identifier)) return;
    CPGroup *group = new CPGroup();
    group->setIdentifier(identifier);
    m_data.push_back(group);
    m_dataMap.insert(identifier, group);
}

void Groups::remove(QString identifier) {
    if(!m_dataMap.contains(identifier)) return;

    CPGroup *group = static_cast<CPGroup*>(m_dataMap[identifier]);
    m_data.removeOne(group);
    m_dataMap.remove(identifier);
    delete group;
}

bool Groups::addOrRemove(LAMMPSController *lammpsController)
{
    Group *lammpsGroup = lammpsController->lammps()->group;
    int numGroups = lammpsGroup->ngroup;
    setCount(numGroups);

    bool anyChanges = false;
    for(int groupIndex=0; groupIndex<numGroups; groupIndex++) {
        QString groupName = QString::fromUtf8(lammpsGroup->names[groupIndex]);
        if(!m_dataMap.contains(groupName)) {
            anyChanges = true;
            add(groupName);
        }
    }

    QList<QString> groupsToBeRemoved;
    for(QObject *obj : m_data) {
        CPGroup *group = static_cast<CPGroup*>(obj);
        if(!lammpsController->groupExists(group->identifier())) {
            anyChanges = true;
            groupsToBeRemoved.append(group->identifier());
        }
    }

    for(QString identifier : groupsToBeRemoved) {
        remove(identifier);
    }

    return anyChanges;
}

void Groups::synchronizeQML(LAMMPSController *lammpsController)
{
    if(!lammpsController->lammps()) { return; }
    bool anyChanges = addOrRemove(lammpsController);
    if(anyChanges) {
        setModel(QVariant::fromValue(m_data));
    }
}

void Groups::synchronize(LAMMPSController *lammpsController)
{
    for(QObject *obj : m_data) {
        CPGroup *group = static_cast<CPGroup*>(obj);
        group->update(lammpsController->lammps());
    }
}

QVariant Groups::model() const
{
    return m_model;
}

int Groups::count() const
{
    return m_count;
}

bool Groups::active() const
{
    return m_active;
}

QList<CPGroup *> Groups::groups()
{
    QList<CPGroup *> groups;
    for(QObject *obj : m_data) {
        CPGroup *group = qobject_cast<CPGroup*>(obj);
        if(group) groups.append(group);
    }

    return groups;
}

void Groups::reset()
{
    for(QObject *obj : m_data) { // TODO obj not used
        CPGroup *group = static_cast<CPGroup*>(obj); // TODO what? group cannot be cast from itself...
        delete group; // TODO crashes here because of the above line
    }
    m_data.clear();
    m_dataMap.clear();
    setModel(QVariant::fromValue(m_data));
}

void Groups::setModel(QVariant model)
{
    if (m_model == model)
        return;

    m_model = model;
    emit modelChanged(model);
}

void Groups::setCount(int count)
{
    if (m_count == count)
        return;

    m_count = count;
    emit countChanged(count);
}

void Groups::setActive(bool active)
{
    if (m_active == active)
        return;

    m_active = active;
    emit activeChanged(active);
}


CPGroup::CPGroup(QObject *parent) : QObject(parent) { }

int CPGroup::count() const
{
    return m_count;
}

int CPGroup::bitmask() const
{
    return m_bitmask;
}

bool CPGroup::hovered() const
{
    return m_hovered;
}

bool CPGroup::visible() const
{
    return m_visible;
}

QString CPGroup::identifier() const
{
    return m_identifier;
}

void CPGroup::setCount(int count)
{
    if (m_count == count)
        return;

    m_count = count;
    emit countChanged(count);
}

void CPGroup::setBitmask(int bitmask)
{
    if (m_bitmask == bitmask)
        return;

    m_bitmask = bitmask;
    emit bitmaskChanged(bitmask);
}

void CPGroup::setHovered(bool hovered)
{
    if (m_hovered == hovered)
        return;

    m_hovered = hovered;
    emit hoveredChanged(hovered);
}

void CPGroup::setVisible(bool visible)
{
    if (m_visible == visible)
        return;

    m_visible = visible;
    emit visibleChanged(visible);
}

void CPGroup::setIdentifier(QString identifier)
{
    if (m_identifier == identifier)
        return;

    m_identifier = identifier;
    emit identifierChanged(identifier);
}

void CPGroup::update(LAMMPS *lammps)
{
    Group *group = lammps->group;
    QByteArray identifierBytes = m_identifier.toUtf8();
    int index = group->find(identifierBytes.constData());
    setBitmask(group->bitmask[index]);
    setCount(group->count(index));
}
