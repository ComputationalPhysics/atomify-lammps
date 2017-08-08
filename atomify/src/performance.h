#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <QObject>

class Performance : public QObject
{
    Q_OBJECT
    Q_PROPERTY(long memoryLAMMPS READ memoryLAMMPS WRITE setMemoryLAMMPS NOTIFY memoryLAMMPSChanged)
    Q_PROPERTY(long memoryAtomify READ memoryAtomify WRITE setMemoryAtomify NOTIFY memoryAtomifyChanged)
    Q_PROPERTY(double effectiveSimulationSpeed READ effectiveSimulationSpeed WRITE setEffectiveSimulationSpeed NOTIFY effectiveSimulationSpeedChanged)
    Q_PROPERTY(double timestepsPerSecond READ timestepsPerSecond WRITE setTimestepsPerSecond NOTIFY timestepsPerSecondChanged)
public:
    explicit Performance(QObject *parent = 0);
    void reset();
    void synchronize(class LAMMPSController *controller);
    long memoryLAMMPS() const;
    long memoryAtomify() const;
    double effectiveSimulationSpeed() const;
    double timestepsPerSecond() const;

signals:
    void memoryLAMMPSChanged(long memoryLAMMPS);
    void memoryAtomifyChanged(long memoryAtomify);
    void effectiveSimulationSpeedChanged(double effectiveSimulationSpeed);
    void timestepsPerSecondChanged(double timestepsPerSecond);

public slots:
    void setMemoryLAMMPS(long memoryLAMMPS);
    void setMemoryAtomify(long memoryAtomify);
    void setEffectiveSimulationSpeed(double effectiveSimulationSpeed);
    void setTimestepsPerSecond(double timestepsPerSecond);

private:
    long m_memoryLAMMPS = 0;
    long m_memoryAtomify = 0;
    double m_effectiveSimulationSpeed = 1.0;
    double m_timestepsPerSecond = 0;
};

#endif // PERFORMANCE_H
