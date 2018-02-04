#include "zcltools.h"

zclTools::zclTools()
{

}

zclTools::~zclTools()
{

}

void zclTools::strToHex(const char *pstr, QByteArray &dest) //eg:char *src="2 3A2 33" dest={0x02,0x3A,0x02,0x33}
{
    uchar tmpValue = 0;
    uchar tmpHex = 0x00;//用于存放一个字节的HEX码
    char flag2 = 0;  //用与标志tmpHex中现在进入了多少个pstr的字节，只能是0，1..当为2个字节时就必须要塞入dest中此时标志清0


    int sLen = strlen(pstr);
    for(int i = 0;i < sLen;i++)
    {
        if(((pstr[i]>='0')&&(pstr[i]<='9'))
           ||((pstr[i]>='a')&&(pstr[i]<='f'))
           ||((pstr[i]>='A')&&(pstr[i]<='F')))
        {
            if((pstr[i]>='0')&&(pstr[i]<='9'))
            {
                tmpValue=pstr[i]-'0';
            }
            else if((pstr[i]>='A')&&(pstr[i]<='F'))
            {
                tmpValue=pstr[i]-'A'+0x0A;
            }
            else if((pstr[i]>='a')&&(pstr[i]<='f'))
            {
                tmpValue=pstr[i]-'a'+0x0A;
            }

            if(0 == flag2)
            {
                tmpHex = tmpValue;
                flag2 = 1;
            }
            else if(1 == flag2)
            {
               tmpHex =  (tmpHex << 4)|tmpValue;
               dest.append(tmpHex);
               tmpHex = 0x00;
               flag2 = 0;
            }
        }
        else
        {
            if(1 == flag2)
            {
               dest.append(tmpHex);
               tmpHex = 0x00;
               flag2 = 0;
            }
            continue;
        }
    }

    if(1 == flag2)  //当结束是如果tmpHex中好友一个字节
    {
       dest.append(tmpHex);
       tmpHex = 0x00;
       flag2 = 0;
    }

    return;
}

/*字符串转换定长bcd码串(包含A-F, 反向)*/
void zclTools::str2bcdex_r(const char *pstr, uchar *pbcd, short len)
{
    uchar tmpValue;
    short i;
    short j;
    short m;
    short sLen;

    sLen=strlen(pstr);
    for(i=0; i<sLen; i++)
    {
        if((pstr[i]<'0')
            ||((pstr[i]>'9')&&(pstr[i]<'A'))
            ||((pstr[i]>'F')&&(pstr[i]<'a'))
            ||(pstr[i]>'f'))
        {
            sLen=i;
            break;
        }
    }
    sLen=sLen < (len*2)?sLen:(len*2);
    memset(pbcd, 0x00, len);
    for(i=sLen-1, j=0, m=0; (i>=0)&&(m<len); i--, j++)
    {
        if((pstr[i]>='0')&&(pstr[i]<='9'))
        {
            tmpValue=pstr[i]-'0';
        }
        else if((pstr[i]>='A')&&(pstr[i]<='F'))
        {
            tmpValue=pstr[i]-'A'+0x0A;
        }
        else if((pstr[i]>='a')&&(pstr[i]<='f'))
        {
            tmpValue=pstr[i]-'a'+0x0A;
        }
        else
        {
            tmpValue=0;
        }
        if((j%2)==0)
        {
            pbcd[m]=tmpValue;
        }
        else
        {
            pbcd[m++]|=(tmpValue<<4);
        }
        if((tmpValue==0)&&(pstr[i]!='0'))
        {
            break;
        }
    }

    return;
}


QDomElement zclTools::getNodeByPath(QDomElement &root, const char *nodepath)
{
    QDomElement cur = root;
    QDomElement children;
    char buf[256];

    strcpy( buf, nodepath );
    char *p = strtok(buf, "|");

    while(p)
    {
        if(!strcmp(cur.tagName().toStdString().c_str(),p ))
        {
            p = strtok(NULL, "|");
            children = cur.firstChildElement();
            continue;
        }

        if( children.isNull() )
        {
            return children;
        }
        else
        {
            cur = children;
            children = children.nextSiblingElement();
        }
    }

    return cur;
}
