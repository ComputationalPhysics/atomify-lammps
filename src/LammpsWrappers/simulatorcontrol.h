#ifndef SIMULATORCONTROL_H
#define SIMULATORCONTROL_H

#include <QQuickItem>
#include <QVariantList>

class LAMMPSController;
class AtomifySimulator;
class SimulatorControl : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool forceReset READ forceReset WRITE setForceReset NOTIFY forceResetChanged)
    Q_PROPERTY(QString identifier READ identifier WRITE setIdentifier NOTIFY identifierChanged)
    Q_PROPERTY(QString command READ command WRITE setCommand NOTIFY commandChanged)
    Q_PROPERTY(QVariantList dependencies READ dependencies WRITE setDependencies NOTIFY dependenciesChanged)

protected:
    QString m_command;
    QString m_identifier;
    QString m_commandPrefix = "atomify_";
    QVariantList m_dependencies;
    bool m_forceReset = false;
    bool m_enabled = true;
    long m_lastUpdated = -1;
    bool addToLammps(LAMMPSController *lammpsController);
    virtual void updateCommand() = 0;
    virtual QList<QString> enabledCommands() = 0;
    virtual QList<QString> disableCommands() = 0;
    virtual QList<QString> resetCommands() = 0;
    void addDependency(SimulatorControl *control);
    void removeDependency(SimulatorControl *control);

public:
    explicit SimulatorControl(QQuickItem *parent = 0);
    ~SimulatorControl();
    bool enabled() const;
    QString identifier() const;
    QString command() const;
    QVariantList dependencies() const;
    Q_INVOKABLE virtual void clear();
    bool dependenciesValid(LAMMPSController *lammpsController);
    virtual bool existsInLammps(LAMMPSController *lammpsController) = 0;
    virtual void update(LAMMPSController *lammpsController);
    virtual void handleCommand(QString command);
    bool forceReset() const;

signals:
    void enabledChanged(bool enabled);
    void identifierChanged(QString identifier);
    void commandChanged(QString command);
    void dependenciesChanged(QVariantList dependencies);
    void forceResetChanged(bool forceReset);

public slots:
    void setEnabled(bool enabled);
    void setIdentifier(QString identifier);
    void setCommand(QString command);
    void setDependencies(QVariantList dependencies);
    void setForceReset(bool forceReset);
};

#endif // SIMULATORCONTROL_H
