#ifndef ATOMS_H
#define ATOMS_H
#include <QColor>
#include <QObject>
#include <QVector>
#include <QVariantList>
#include <mpi.h>
#include <lammps.h>
#include <SimVis/Bonds>
#include <SimVis/Spheres>
#include "atomdata.h"
struct AtomStyle {
    QColor color;
    float radius;
    AtomStyle(float scale, QColor color) {
        this->color = color;
        this->radius = scale;
    }
};

class Atoms : public QObject
{
    Q_OBJECT
    Q_PROPERTY(CPBonds* bonds READ bonds NOTIFY bondsChanged)
    Q_PROPERTY(QVariantList modifiers READ modifiers WRITE setModifiers NOTIFY modifiersChanged)
    Q_PROPERTY(float bondRadius READ bondRadius WRITE setBondRadius NOTIFY bondRadiusChanged)
    Q_PROPERTY(float sphereRadius READ sphereRadius WRITE setSphereRadius NOTIFY bondSphereChanged)
    Q_PROPERTY(float occlusionFactor READ occlusionFactor WRITE setOcclusionFactor NOTIFY occlusionFactorChanged)
public:
    Atoms(class AtomifySimulator *simulator = nullptr);
    void synchronize(LAMMPS_NS::LAMMPS *lammps);
    void updateData(class System *system, LAMMPS_NS::LAMMPS *lammps);
    QVariantList modifiers() const;
    QVector<AtomStyle*> &atomStyles();
    void setAtomType(int atomType, QString atomTypeName);
    void setAtomColorAndScale(int atomType, QColor color, float radius);
    class CPBonds* bonds() const;
    AtomData &atomData();
    float bondRadius() const;
    void reset();
    QVector<SphereData> &spheresData();
    QVector<BondData> &bondsData();
    float sphereRadius() const;
    float occlusionFactor() const;
    long memoryUsage();

public slots:
    void setBondRadius(float bondRadius);
    void setModifiers(QVariantList modifiers);
    void setSphereRadius(float sphereRadius);
    void setOcclusionFactor(float occlusionFactor);

signals:
    void bondsChanged(class CPBonds* bonds);
    void bondRadiusChanged(float bondRadius);
    void modifiersChanged(QVariantList modifiers);
    void bondSphereChanged(float sphereRadius);
    void occlusionFactorChanged(float occlusionFactor);

private:
    AtomData m_atomData;
    QMap<QString, AtomStyle*> m_atomStyleTypes;
    QVector<AtomStyle*> m_atomStyles;
    QVector<SphereData> m_spheresData;
    QVector<BondData> m_bondsData;
    class CPBonds* m_bonds = nullptr;
    QVariantList m_modifiers;
    float m_bondRadius = 0.1;
    float m_sphereRadius = 1.0;
    float m_occlusionFactor = 1000;
    void generateBondData(AtomData &atomData, LAMMPS_NS::LAMMPS *lammps);
    void generateBondDataFromLammpsNeighborlist(AtomData &atomData, LAMMPS_NS::LAMMPS &lammps);
    void generateSphereData(AtomData &atomData);
    void applyDeltaPositions(AtomData &atomData);
    bool generateBondDataFromBondList(AtomData &atomData, LAMMPS_NS::LAMMPS *lammps);
    bool generateBondDataFromNeighborList(AtomData &atomData, class LAMMPSController *controller);
    bool doWeHavefullNeighborList(LAMMPS_NS::Neighbor *neighbor);
};

#endif // ATOMS_H
