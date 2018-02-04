#ifndef ZCLTOOLS_H
#define ZCLTOOLS_H

#include <QByteArray>
#include <QDomDocument> //用与操作XML .pro中QT += xml
class zclTools
{
public:
    zclTools();
    ~zclTools();

    /*字符串转hex  eg:char *src="2 3A2 33" dest={0x02,0x3A,0x02,0x33}*/
    static void strToHex(const char *pstr,QByteArray &dest);
    /*字符串转换定长bcd码串(包含A-F, 反向)*/
    void str2bcdex_r(const char *pstr, uchar *pbcd, short len);

    /*根据节点路径返回节点*/
    static QDomElement getNodeByPath(QDomElement &root, const char *nodepath);
};

#endif // ZCLTOOLS_H
