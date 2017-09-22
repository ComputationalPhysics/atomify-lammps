#include "periodicimages.h"
#include "../system.h"
#include "../atomdata.h"
PeriodicImages::PeriodicImages()
{

}


void PeriodicImages::apply(AtomData &atomData)
{
    if(!enabled()) return;

    int numImages[3];
    numImages[0] = m_showImagesX ? std::max(m_numberOfCopiesX, 1) : 1;
    numImages[1] = m_showImagesY ? std::max(m_numberOfCopiesY, 1) : 1;
    numImages[2] = m_showImagesZ ? std::max(m_numberOfCopiesZ, 1) : 1;
    int numberOfCopies = numImages[0]*numImages[1]*numImages[2];
    if(numberOfCopies == 1) return; // No copies

    int imageLow[3];
    int imageHigh[3];
    imageLow[0] = -(numImages[0]-1)/2;
    imageLow[1] = -(numImages[1]-1)/2;
    imageLow[2] = -(numImages[2]-1)/2;

    imageHigh[0] = numImages[0]/2;
    imageHigh[1] = numImages[1]/2;
    imageHigh[2] = numImages[2]/2;

    int nextIndex = atomData.size();
    int originalCount = atomData.size();
    atomData.resize(numberOfCopies*atomData.size());
    for(int imageX = imageLow[0]; imageX <= imageHigh[0]; imageX++) {
        for(int imageY = imageLow[1]; imageY <= imageHigh[1]; imageY++) {
            for(int imageZ = imageLow[2]; imageZ <= imageHigh[2]; imageZ++) {
                if(imageX == 0 && imageY == 0 && imageZ == 0) continue;

                const QMatrix3x3 &T = m_system->cellMatrix();
                const QVector3D deltaPosition(
                            T(0,0)*imageX + T(0,1)*imageY + T(0,2)*imageZ,
                            T(1,0)*imageX + T(1,1)*imageY + T(1,2)*imageZ,
                            T(2,0)*imageX + T(2,1)*imageY + T(2,2)*imageZ
                            );

                for(int atomIndex = 0; atomIndex<originalCount; atomIndex++) {
                    atomData.positions[nextIndex] = atomData.positions[atomIndex];
                    atomData.deltaPositions[nextIndex] = deltaPosition;
                    atomData.colors[nextIndex] = atomData.colors[atomIndex];
                    atomData.radii[nextIndex] = atomData.radii[atomIndex];
                    atomData.originalIndex[nextIndex] = atomIndex;
                    atomData.bitmask[nextIndex] = atomData.bitmask[atomIndex];
                    atomData.types[nextIndex] = atomData.types[atomIndex];
                    atomData.visible[nextIndex] = atomData.visible[atomIndex];
                    nextIndex++;
                }
            }
        }
    }
}

int PeriodicImages::numberOfCopiesX() const
{
    return m_numberOfCopiesX;
}

int PeriodicImages::numberOfCopiesY() const
{
    return m_numberOfCopiesY;
}

int PeriodicImages::numberOfCopiesZ() const
{
    return m_numberOfCopiesZ;
}

bool PeriodicImages::showImagesX() const
{
    return m_showImagesX;
}

bool PeriodicImages::showImagesY() const
{
    return m_showImagesY;
}

bool PeriodicImages::showImagesZ() const
{
    return m_showImagesZ;
}

void PeriodicImages::setNumberOfCopiesX(int numberOfCopiesX)
{
    if (m_numberOfCopiesX == numberOfCopiesX)
        return;

    m_numberOfCopiesX = numberOfCopiesX;
    emit numberOfCopiesXChanged(numberOfCopiesX);
}

void PeriodicImages::setNumberOfCopiesY(int numberOfCopiesY)
{
    if (m_numberOfCopiesY == numberOfCopiesY)
        return;

    m_numberOfCopiesY = numberOfCopiesY;
    emit numberOfCopiesYChanged(numberOfCopiesY);
}

void PeriodicImages::setNumberOfCopiesZ(int numberOfCopiesZ)
{
    if (m_numberOfCopiesZ == numberOfCopiesZ)
        return;

    m_numberOfCopiesZ = numberOfCopiesZ;
    emit numberOfCopiesZChanged(numberOfCopiesZ);
}

void PeriodicImages::setShowImagesX(bool showImagesX)
{
    if (m_showImagesX == showImagesX)
        return;

    m_showImagesX = showImagesX;
    emit showImagesXChanged(showImagesX);
}

void PeriodicImages::setShowImagesY(bool showImagesY)
{
    if (m_showImagesY == showImagesY)
        return;

    m_showImagesY = showImagesY;
    emit showImagesYChanged(showImagesY);
}

void PeriodicImages::setShowImagesZ(bool showImagesZ)
{
    if (m_showImagesZ == showImagesZ)
        return;

    m_showImagesZ = showImagesZ;
    emit showImagesZChanged(showImagesZ);
}


void PeriodicImages::parseCommand(QString cmd)
{
    if(!cmd.startsWith("periodic")) return;
    QStringList words = cmd.split(" ");
    if(words.size() != 3) return;
    QString dimension = words[1];

    bool ok;
    int copies = words[2].toInt(&ok);
    if(!ok) return;
    if(copies <= 0 || copies > 5) return;

    setEnabled(true);
    if(dimension=="x") setNumberOfCopiesX(copies);
    else if(dimension=="y") setNumberOfCopiesY(copies);
    else if(dimension=="z") setNumberOfCopiesZ(copies);
}
