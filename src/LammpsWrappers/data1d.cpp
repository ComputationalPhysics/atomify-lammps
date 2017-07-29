#include "data1d.h"
#include <QLineSeries>
#include <QElapsedTimer>
#include <QDebug>

Data1D::Data1D(QObject *parent) : QObject(parent)
{

}

float Data1D::xMin() const
{
    return m_xMin;
}

float Data1D::xMax() const
{
    return m_xMax;
}

float Data1D::yMin() const
{
    return m_yMin;
}

float Data1D::yMax() const
{
    return m_yMax;
}

void Data1D::updateLimits()
{
    QElapsedTimer t;
    t.start();
    QList<QPointF> validPoints = this->validPoints();

    if(validPoints.size() == 0) return;

    qreal xMax = 0;
    qreal xMin = 0;
    qreal yMax = 0;
    qreal yMin = 0;
    bool firstValueFound = false;

    for(QPointF &p : validPoints) {
        if(!firstValueFound) {
            xMax = p.x();
            xMin = p.x();
            yMax = p.y();
            yMin = p.y();
            firstValueFound = true;
        } else {
            xMax = std::max(p.x(), xMax);
            xMin = std::min(p.x(), xMin);
            yMax = std::max(p.y(), yMax);
            yMin = std::min(p.y(), yMin);
        }
    }
    if(firstValueFound) {
        setXMax(xMax);
        setXMin(xMin);
        setYMax(yMax);
        setYMin(yMin);
    } // if not, then we have no data to update limits with
}

void Data1D::updateData(QAbstractSeries *series)
{
    QElapsedTimer t;
    t.start();

    if(series) {
        QXYSeries *xySeries = static_cast<QXYSeries*>(series);
        xySeries->replace(validPoints());
    }
}

QString Data1D::title() const
{
    return m_title;
}

float Data1D::xScale() const
{
    return m_xScale;
}

float Data1D::yScale() const
{
    return m_yScale;
}

bool Data1D::average() const
{
    return m_average;
}

float Data1D::lastValue() const
{
    return m_lastValue;
}

bool Data1D::enabled() const
{
    return m_enabled;
}

float Data1D::xRange() const
{
    return m_xRange;
}

bool Data1D::pointIsValid(const QPointF &p)
{
    if(m_xRange > 0) {
        // X range test is false if m_xRange == 0
        qreal lowestXValue = m_points.last().x()*m_xScale - m_xRange;
        if(p.x()*m_xScale < lowestXValue) {
            return false;
        }
    }

    return true;
}

QList<QPointF> Data1D::validPoints()
{
    QList<QPointF> validPoints;
    for(QPointF &p : m_points) {
        if(pointIsValid(p)) {
            validPoints.append(QPointF(m_xScale*p.x(), m_yScale*p.y()));
        }
    }

    return validPoints;
}

void Data1D::clear()
{
    m_points.clear();
}

void Data1D::add(float x, float y, bool silent)
{
    m_sum += y;
    if(m_average) {
        m_numValues++;
        double value = m_sum / m_numValues;
        value = 0.0;
        if(m_points.size()) {
            value = m_points.last().y();
        } else {
            value = y;
        }
        value = 0.99*value + 0.01*y; // low pass filter
        m_points.append(QPointF(x,value));
        setLastValue(value);
    } else {
        m_points.append(QPointF(x,y));
        setLastValue(y);
    }
    if(!silent) emit updated();
}

void Data1D::setXMin(float xMin)
{
    if (m_xMin == xMin)
        return;

    m_xMin = xMin;
    emit xMinChanged(xMin);
}

void Data1D::setXMax(float xMax)
{
    if (m_xMax == xMax)
        return;

    m_xMax = xMax;
    emit xMaxChanged(xMax);
}

void Data1D::setYMin(float yMin)
{
    if (m_yMin == yMin)
        return;

    m_yMin = yMin;
    emit yMinChanged(yMin);
}

void Data1D::setYMax(float yMax)
{
    if (m_yMax == yMax)
        return;

    m_yMax = yMax;
    emit yMaxChanged(yMax);
}

void Data1D::setEnabled(bool enabled)
{
    if (m_enabled == enabled)
        return;

    m_enabled = enabled;
    emit enabledChanged(enabled);
}

void Data1D::setXRange(float xRange)
{
    if (m_xRange == xRange)
        return;

    m_xRange = xRange;
    emit xRangeChanged(xRange);
}

void Data1D::setTitle(QString title)
{
    if (m_title == title)
        return;

    m_title = title;
    emit titleChanged(title);
}

void Data1D::setXScale(float xScale)
{
    if (m_xScale == xScale)
        return;

    m_xScale = xScale;
    emit xScaleChanged(xScale);
}

void Data1D::setYScale(float yScale)
{
    if (m_yScale == yScale)
        return;

    m_yScale = yScale;
    emit yScaleChanged(yScale);
}

void Data1D::setAverage(bool average)
{
    if (m_average == average)
        return;

    m_average = average;
    emit averageChanged(average);
}

void Data1D::setLastValue(float lastValue)
{
    if (m_lastValue == lastValue)
        return;

    m_lastValue = lastValue;
    emit lastValueChanged(lastValue);
}
