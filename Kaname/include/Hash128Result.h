/**************************************************************************
 * Copyright (C) 2016 Afa.L Cheng (afa@afa.moe)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************/

#ifndef HASH128RESULT_H
#define HASH128RESULT_H

#include <QByteArray>
#include <QString>

class Hash128Result
{
public:
    Hash128Result() : _hi(0), _lo(0) {}
    Hash128Result(const QByteArray &result)
    {
        if (result.size() == 16) {
            _hi = *(quint64 *)result.data();
            _lo = *(quint64 *)(result.data() + 8);
        }
    }
    Hash128Result(quint64 hi, quint64 lo) : _hi(hi), _lo(lo) {}
    Hash128Result(const QString &hash) : Hash128Result(QByteArray::fromHex(hash.toUtf8()))
    {
    }

    quint64 hi() const
    {
        return _hi;
    }

    void setHi(const quint64 &hi)
    {
        _hi = hi;
    }

    quint64 lo() const
    {
        return _lo;
    }

    void setLo(const quint64 &lo)
    {
        _lo = lo;
    }

    QString toString()
    {
        QByteArray result(16, 0);
        *(quint64 *)result.data() = _hi;
        *(quint64 *)(result.data() + 8) = _lo;
        return result.toHex();
    }

private:
    quint64 _hi;
    quint64 _lo;
};

inline uint qHash(const Hash128Result &val, uint seed = 0)
{
    Q_UNUSED(seed);
    return val.hi() >> 32;
}

inline bool operator==(const Hash128Result &a, const Hash128Result b)
{
    return a.hi() == b.hi() && a.lo() == b.lo();
}

inline bool operator<(const Hash128Result &a, const Hash128Result b)
{
    if (a.hi() < b.hi())
        return true;
    return a.lo() < b.lo();
}

inline bool operator>(const Hash128Result &a, const Hash128Result b)
{
    if (a.hi() > b.hi())
        return true;
    return a.lo() > b.lo();
}

#endif // HASH128RESULT_H
