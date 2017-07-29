#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include <SimVis/Simulator>
#include <QMap>
#include <QElapsedTimer>
#include <memory>
#include <cmath>
#include <QVector3D>
#include <mpi.h>
#include <lammps.h>

#include "lammpscontroller.h"
#include "scripthandler.h"
#include "LammpsWrappers/lammpserror.h"

using namespace LAMMPS_NS;

class MyWorker : public SimulatorWorker
{
    Q_OBJECT
public:
    MyWorker();
    bool willPause() const;
    void setWillPause(bool willPause);

private:
    QElapsedTimer m_elapsed;
    QElapsedTimer m_sinceStart;
    long m_timesteps = 0;
    long m_elapsedTime = 0;
    LAMMPSController m_lammpsController;
    bool m_addPeriodicCopies = false;
    float m_cameraToSystemCenterDistance = 1.0;
    Atoms *m_atoms = nullptr;
    bool m_running = false;

    // SimulatorWorker interface
    virtual void synchronizeSimulator(Simulator *simulator) override;
    virtual void synchronizeRenderer(Renderable *renderableObject) override;
    virtual void work() override;
    void synchronizeSpheres(class Spheres *spheres);
    void synchronizeBonds(class Bonds *bonds);
    bool m_willPause = false;
};

class AtomifySimulator : public Simulator
{
    Q_OBJECT
    Q_PROPERTY(int simulationSpeed READ simulationSpeed WRITE setSimulationSpeed NOTIFY simulationSpeedChanged)
    Q_PROPERTY(bool paused READ paused WRITE setPaused NOTIFY pausedChanged)
    Q_PROPERTY(ScriptHandler* scriptHandler READ scriptHandler WRITE setScriptHandler NOTIFY scriptHandlerChanged)
    Q_PROPERTY(bool willReset READ willReset WRITE setWillReset NOTIFY willResetChanged)
    Q_PROPERTY(LammpsError* lammpsError READ lammpsError WRITE setLammpsError NOTIFY lammpsErrorChanged)
    Q_PROPERTY(System* system READ system WRITE setSystem NOTIFY systemChanged)
    Q_PROPERTY(bool hasExecutedRunCommand READ hasExecutedRunCommand WRITE setHasExecutedRunCommand NOTIFY hasExecutedRunCommandChanged)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(int currentRunStep READ currentRunStep WRITE setCurrentRunStep NOTIFY currentRunStepChanged)
    Q_PROPERTY(int runStepCount READ runStepCount WRITE setRunStepCount NOTIFY runStepCountChanged)

public:
    AtomifySimulator();
    ~AtomifySimulator();

    // Simulator interface
    int simulationSpeed() const;
    bool paused() const;
    ScriptHandler* scriptHandler() const;
    bool willReset() const;
    Q_INVOKABLE void clearSimulatorControls();
    Q_INVOKABLE void addSimulatorControl(SimulatorControl *simulatorControl);
    class System* system() const;
    LammpsError* lammpsError() const;
    bool hasExecutedRunCommand() const;
    bool running() const;
    int currentRunStep() const;
    int runStepCount() const;

public slots:
    void setSimulationSpeed(int arg);
    void setPaused(bool paused);
    void setScriptHandler(ScriptHandler* scriptHandler);
    void setWillReset(bool willReset);
    void setSystem(class System* system);
    void setLammpsError(LammpsError* lammpsError);
    void setHasExecutedRunCommand(bool hasExecutedRunCommand);
    void setRunning(bool running);

signals:
    void simulationSpeedChanged(int arg);
    void pausedChanged(bool paused);
    void errorInLammpsScript();
    void lammpsDidReset();
    void scriptHandlerChanged(ScriptHandler* scriptHandler);
    void willResetChanged(bool willReset);
    void cameraToSystemCenterDistanceChanged(float cameraToSystemCenterDistance);
    void systemChanged(class System* system);
    void lammpsErrorChanged(LammpsError* lammpsError);
    void hasExecutedRunCommandChanged(bool hasExecutedRunCommand);
    void runningChanged(bool running);
    void currentRunStepChanged(int currentRunStep);
    void runStepCountChanged(int runStepCount);

protected:
    virtual MyWorker *createWorker() override;

private:
    void setCurrentRunStep(int currentRunStep);
    void setRunStepCount(int runStepCount);

    friend class MyWorker;
    LammpsError* m_lammpsError = nullptr;
    class System* m_system = nullptr;
    ScriptHandler* m_scriptHandler = nullptr;
    LammpsState lammpsState;
    int m_simulationSpeed = 1;
    bool m_paused = false;
    bool m_willReset = false;
    QList<SimulatorControl*> m_simulatorControls;
    bool m_running = false;
    bool m_hasExecutedRunCommand = false;
    int m_currentRunStep;
    int m_runStepCount;
};

#endif // MYSIMULATOR_H
