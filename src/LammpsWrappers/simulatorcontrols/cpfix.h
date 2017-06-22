#ifndef CPFIX_H
#define CPFIX_H
#include "simulatorcontrol.h"
#include <fix_ave_chunk.h>
#include <fix_ave_histo.h>
class CPFix : public SimulatorControl
{
    Q_OBJECT
    Q_PROPERTY(QString group READ group WRITE setGroup NOTIFY groupChanged)
    Q_PROPERTY(int frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
    Q_PROPERTY(bool interactive READ interactive WRITE setInteractive NOTIFY interactiveChanged)
    Q_PROPERTY(QVariantList data READ data WRITE setData NOTIFY dataChanged)
    Q_PROPERTY(QVariant model READ model WRITE setModel NOTIFY modelChanged)
protected:
    QList<QString> enabledCommands() override;
    QList<QString> disableCommands() override;
    virtual void updateCommand() override;
    virtual QList<QString> resetCommands() override;
    QString createCommandPrefix() override;
    long lastUpdate = -1;

public:
    CPFix(Qt3DCore::QNode *parent = 0);
    ~CPFix();
    void update(class LAMMPSController *lammpsController) override;
    virtual bool existsInLammps(class LAMMPSController *lammpsController);
    QString group() const;
    int frequency() const;
    void copyData(class LAMMPSController *lammpsController);
    bool interactive() const;
    QVariantList data() const;
    QVariant model() const;
    class Data1D* data1D() const;

signals:
    void groupChanged(QString group);
    void frequencyChanged(int frequency);
    void interactiveChanged(bool interactive);
    void dataChanged(QVariantList data);
    void updatedContent();
    void modelChanged(QVariant model);
    void data1DChanged(class Data1D* data1D);

public slots:
    void setGroup(QString group);
    void setFrequency(int frequency);
    void setInteractive(bool interactive);
    void setData(QVariantList data);
    void setModel(QVariant model);
    void setData1D(class Data1D* data1D);

private:
    QString m_group = "all";
    int m_frequency = 10;
    bool m_interactive = false;
    bool copyData(LAMMPS_NS::FixAveChunk *fix, class LAMMPSController *lammpsController);
    bool copyData(LAMMPS_NS::FixAveHisto *fix, class LAMMPSController *lammpsController);
    QVariantList m_data;
    QList<class Data2D*> m_dataRaw;
    QVariant m_model;
    // QVariantMap m_data1D;
    QMap<QString, class Data1D*> m_data1DRaw;
    class Data1D* m_data1D;
    class Data1D *ensureExists(QString key, bool enabledByDefault);
};

#endif // CPFIX_H
