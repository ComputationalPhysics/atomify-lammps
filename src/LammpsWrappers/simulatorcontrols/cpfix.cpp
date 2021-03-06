#include "cpfix.h"
#include "lammpscontroller.h"
#include "../system.h"
#include "../../dataproviders/data2d.h"
#include "../../dataproviders/data1d.h"
#include <iostream>
#include <lmptype.h>

#include <style_compute.h>

CPFix::CPFix(QObject *parent) : SimulatorControl(parent)
{
    setType("Fix");
}

CPFix::~CPFix() { }

// When Fix ave/time etc has one or more variables it averages over, we might need
// to find what kind. One such example is compute rdf which has a special x axis

QString CPFix::getType(LAMMPSController *lammpsController, int which, QString identifier)
{
    enum{COMPUTE,FIX,VARIABLE};

    if(which == COMPUTE) {
        LAMMPS_NS::Compute *compute = lammpsController->findComputeByIdentifier(identifier);
        if(compute) {
            LAMMPS_NS::ComputeRDF *compute_rdf = dynamic_cast<LAMMPS_NS::ComputeRDF *>(compute);
            if(compute_rdf) {
                return QString("Compute RDF");
            }
        }
    }
    return QString("");
}

bool CPFix::copyData(LAMMPS_NS::FixAveChunk *fix, LAMMPSController *lammpsController) {
    enum{BIN1D,BIN2D,BIN3D,BINSPHERE,BINCYLINDER,
         TYPE,MOLECULE,COMPUTE,FIX,VARIABLE};
    // return false; // TODO: This fix is broken
    if(!fix) return false;
    int dimension;
    LAMMPS_NS::ComputeChunkAtom *chunk = static_cast<LAMMPS_NS::ComputeChunkAtom*>(fix->extract("cchunk", dimension));
    int *nvalues = static_cast<int*>(fix->extract("nvalues", dimension));
    int *nchunk = static_cast<int*>(fix->extract("nchunk", dimension));
    int *colextra = static_cast<int*>(fix->extract("colextra", dimension));
    if(!nvalues || !nchunk || !chunk || !colextra) {
        qDebug() << "Warning, could not get values from FixAveChunk::extract.";
        return true;
    }

    int *which = static_cast<int*>(chunk->extract("which", dimension));
    int *dim = static_cast<int*>(chunk->extract("dim", dimension));
    int *nlayers = static_cast<int*>(chunk->extract("nlayers", dimension));

    if(!which || !dim || !nlayers) {
        qDebug() << "Warning, could not get values from ComputeChunkAtom::extract.";
        return true;
    }
    if(*which == BIN2D) {
        return true;
//        setInteractive(true);
//        if(m_dataRaw.size() != *nvalues) {
//            m_dataRaw.clear();
//            m_data.clear();
//            for(int i=0; i<*nvalues; i++) {
//                Data2D *data = new Data2D(this);
//                m_dataRaw.push_back(data);
//                m_data.push_back(QVariant::fromValue(data));
//            }
//        }

//        if(fix->nextvalid() == lammpsController->system->currentTimestep()+1) {
//            QStringList labels = {"x", "y", "z"};

//            int x = dim[0];
//            int z = dim[1];
//            int y = 3 - x-z;

//            QSize size(nlayers[0], nlayers[1]);

//            float xMin = lammpsController->system->origin()[x];
//            float xMax = lammpsController->system->origin()[x] + lammpsController->system->size()[x];
//            float zMin = lammpsController->system->origin()[z];
//            float zMax = lammpsController->system->origin()[z] + lammpsController->system->size()[z];

//            QString xLabel = labels[x];
//            QString yLabel = labels[y];
//            QString zLabel = labels[z];

//            float minValues[*nvalues];
//            float maxValues[*nvalues];

//            for(int i=0; i<*nvalues; i++) {
//                m_dataRaw[i]->setXMin(xMin);
//                m_dataRaw[i]->setXMax(xMax);
//                m_dataRaw[i]->setZMin(zMin);
//                m_dataRaw[i]->setZMax(zMax);
//                m_dataRaw[i]->setXLabel(xLabel);
//                m_dataRaw[i]->setYLabel(yLabel);
//                m_dataRaw[i]->setZLabel(zLabel);
//                m_dataRaw[i]->setSize(size);
//                minValues[i] = 1e9;
//                maxValues[i] = -1e9;
//            }

//            for(int i=0; i<*nchunk; i++) {
//                float x = fix->compute_array(i,0);
//                float z = fix->compute_array(i,1);

//                for(int j=0; j<*nvalues; j++) {
//                    int valueIndex = *colextra+1+j;
//                    float y = fix->compute_array(i,valueIndex);
//                    minValues[j] = std::min(minValues[j], y);
//                    maxValues[j] = std::max(maxValues[j], y);
//                    m_dataRaw[j]->setValue(x,y,z);
//                }
//            }

//            for(int i=0; i<*nvalues; i++) {
//                m_dataRaw[i]->setYMin(minValues[i]);
//                m_dataRaw[i]->setYMax(maxValues[i]);
//                m_dataRaw[i]->update();
//            }
//        }
    } else if(*which == BIN1D) {
        setInteractive(true);
        setIsPerAtom(true);

        LAMMPS_NS::bigint *nextValidTimestep = reinterpret_cast<LAMMPS_NS::bigint*>(fix->extract("nvalid", dimension));
        if(m_nextValidTimestep+1 == lammpsController->system->currentTimestep()) {
            setNumPerAtomValues(*nvalues);

            for(int i=0; i<*nvalues; i++) {
                Data1D *data = ensureExists( QString("Value %1").arg(i+1), true);
                if( !window() && !hovered()) continue;

                data->clear(true);
                for(int j=0; j<*nchunk; j++) {
                    float x = fix->compute_array(j,0);
                    int valueIndex = *colextra+1+i;
                    float y = fix->compute_array(j, valueIndex);
                    data->add(x,y);
                }
            }
        }

        if(hovered()) {
            // Even though we haven't updated contents for a while, atoms might have
            // reorganized in memory, so we need to copy the new data if hovered
            int numAtoms = lammpsController->system->numberOfAtoms();
            m_atomData.resize(numAtoms);
            for(int i=0; i<numAtoms; i++) {
                int chunkID = chunk->ichunk[i];

                int valueIndex = *colextra+1+m_perAtomIndex;
                float value = fix->compute_array(chunkID-1, valueIndex);
                m_atomData[i] = value;
            }
        }

        m_nextValidTimestep = *nextValidTimestep;
    }
}

bool CPFix::copyData(LAMMPS_NS::FixAveTime *fix, LAMMPSController *lammpsController) {
    if(!fix) return false;
    if(m_nextValidTimestep > lammpsController->system->currentTimestep()) {
        // Not ready to measure yet
        return true;
    }

    enum{SCALAR,VECTOR};
    int nrows, nvalues, dim, mode;
    int *value = reinterpret_cast<int*>(fix->extract("nrows", dim));
    nrows = *value;
    value = reinterpret_cast<int*>(fix->extract("nvalues", dim));
    nvalues = *value;
    value = reinterpret_cast<int*>(fix->extract("mode", dim));
    mode = *value;

    LAMMPS_NS::bigint *nextValidTimestep = reinterpret_cast<LAMMPS_NS::bigint*>(fix->extract("nvalid", dim));
    if(m_nextValidTimestep+1 == lammpsController->system->currentTimestep()) {
        // +1 because fix_atomify is invoked before all other fixes, so we need the next timestep
        if(mode == SCALAR) {
            // Time dependent solution with 1 or more values
            if(nvalues == 1) {
                // A single value
                double value = fix->compute_scalar();
                setHasScalarData(true);
                setScalarValue(value);
                Data1D *data = ensureExists(QString("scalar"), true);
                data->setLabel(identifier());
                data->add(lammpsController->system->simulationTime(), value);
            } else {
                // Multiple values
                for(int i=0; i<nvalues; i++) {
                    double value = fix->compute_vector(i);
                    QString key = QString("Value %1").arg(i+1);
                    Data1D *data = ensureExists(key, true);
                    data->setLabel(key);
                    data->add(lammpsController->system->simulationTime(), value);
                }
            }
            setXLabel("Time");
            setYLabel("Value");
            setInteractive(true);
            return true;
        } else {
            char **ids = reinterpret_cast<char**>(fix->extract("ids", dim));
            int *which = reinterpret_cast<int*>(fix->extract("which", dim));

            for(int i=0; i<nvalues; i++) {
                QString type = getType(lammpsController, which[i], ids[i]);
                QString key = QString("Value %1").arg(i+1);
                Data1D *data = ensureExists(key, true);
                data->setLabel(key);
                data->clear(true);
                for(int j=0; j<nrows; j++) {
                    double value = fix->compute_array(j, i);
                    if(type=="Compute RDF") {
                        LAMMPS_NS::ComputeRDF *compute_rdf = dynamic_cast<LAMMPS_NS::ComputeRDF *>(lammpsController->findComputeByIdentifier(QString::fromUtf8(ids[i])));
                        double binCenter = compute_rdf->array[j][0];
                        data->add(binCenter, value);
                    } else {
                        data->add(j, value);
                    }
                }
            }
            setInteractive(true);
        }
    }

    m_nextValidTimestep = *nextValidTimestep;
    return true;
}

bool CPFix::copyData(LAMMPS_NS::FixAveHisto *fix, LAMMPSController *lammpsController) {
    // TODO Implement this
    if(!fix) return false;

    int dim;
    int *value = reinterpret_cast<int*>(fix->extract("nbins", dim));
    int nbins = *value;

    QVector<QPointF> histogramPoints;
    histogramPoints.reserve(nbins);
    for(int i=0; i<nbins; i++) {
        // TODO: add normalized choice
        double binCenter = fix->compute_array(i, 0);
        double binContent = fix->compute_array(i, 2);
        histogramPoints.push_back(QPointF(binCenter, binContent));
    }
    Data1D *data = ensureExists(QString("histogram"), true);
    data->copyHistogram(histogramPoints);

    setInteractive(true);
    return true;
}

void CPFix::copyData(LAMMPSController *lammpsController)
{
    LAMMPS_NS::Fix *lmp_fix = lammpsController->findFixByIdentifier(identifier());
    if(lmp_fix == nullptr) return;
    if(copyData(dynamic_cast<LAMMPS_NS::FixAveChunk*>(lmp_fix), lammpsController)) return;
    if(copyData(dynamic_cast<LAMMPS_NS::FixAveHisto*>(lmp_fix), lammpsController)) return;
    if(copyData(dynamic_cast<LAMMPS_NS::FixAveTime*>(lmp_fix), lammpsController)) return;
}

bool CPFix::existsInLammps(LAMMPSController *lammpsController)
{
    return lammpsController->fixExists(identifier());
}
