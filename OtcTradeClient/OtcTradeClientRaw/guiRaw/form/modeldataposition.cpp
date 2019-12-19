#include "modeldataposition.h"
#include "trademodule.h"
#include "datamodule.h"

//排序依据:
//交易所小的靠前，如果交易所一致，则按字母顺序
bool PositionLower(tagXTInvestorPositionField& a, tagXTInvestorPositionField& b)
{
    if (a.ExCode.ExchID < b.ExCode.ExchID)
    {
        return true;
    }
    else if (a.ExCode.ExchID == b.ExCode.ExchID)
    {
        int icmp = stricmp(a.ExCode.Code, b.ExCode.Code);
        if (icmp < 0)
        {
            return true;
        }
        return false;
    }
    return false;
}

//一些公共函数
namespace positionUtil
{

bool        isCompositeCode(const char* pcode)
{
    int nlen = strlen(pcode);
    for (int i = 0; i < nlen; i++)
    {
        if (pcode[i] == ' ' || pcode[i] == '&')
        {
            return true;
        }
    }
    return false;
}

QColor      getColorByDirection(char posiDirection)
{
    if (posiDirection == XT_PD_Long)
    {
        return QColor(204, 51, 51);
    }
    else if (posiDirection == XT_PD_Short)
    {
        return QColor(0, 128, 0);
    }
    else
    {
        return QColor(201, 48, 201);
    }
}

int         getAlignByDirection(char posiDirection)
{
    if (posiDirection == XT_PD_Long)
    {
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else if (posiDirection == XT_PD_Short)
    {
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    else
    {
        return int(Qt::AlignCenter | Qt::AlignVCenter);
    }

}

QString     getStringByPosiDirection(const tagXTInvestorPositionField& o)
{
    QString str = "";

    if (o.PosiDirection == XT_PD_Long)
        str = "买";
    else if (o.PosiDirection == XT_PD_Short)
        str = "卖";
    else if (o.PosiDirection == XT_PD_Net)
        str = "净";
    else
        str = "备兑";

    return str;
}

QString     getStringByHedgeFlag(const tagXTInvestorPositionField& o)
{
    QString str = "";

    if (o.HedgeFlag == XT_HF_Arbitrage)
        str = "套利";
    else if (o.HedgeFlag == XT_HF_Hedge)
        str = "套保";
    else if (o.HedgeFlag == XT_HF_Covered)
        str = "备兑";
    else if (o.HedgeFlag == XT_HF_Speculation)
        str = "投机";
    else
        str = "其他";

    return str;
}

} //end namespace

//================================================================================================================================================================================================
//=================================================================================================================================================================================================

ModelNormalPosition::ModelNormalPosition(QObject *parent/* = nullptr*/)
{
    m_pMgr = nullptr;
}

void        ModelNormalPosition::release()
{
    m_arrPosTable.clear();
}

void        ModelNormalPosition::setOrderMgr(QOrderMgr* p)
{
    m_pMgr = p;

    beginResetModel();

    m_arrPosTable.clear();

    if (p)
    {

        int ncount = p->GetAllPosition(nullptr, 0);
        if (ncount > 0)
        {
            tagXTInvestorPositionField* pArr = new tagXTInvestorPositionField[ncount];
            p->GetAllPosition(pArr, ncount);

            for (int i = 0; i < ncount; i++)
            {
                //[001]过滤总持仓为0的条目
                if (pArr[i].Position <= 0)
                {
                    continue;
                }
                //[002]过滤组合持仓(code中含有&)
                if (positionUtil::isCompositeCode(pArr[i].ExCode.Code))
                {
                    continue;
                }
                //[003]加入
                m_arrPosTable.push_back(pArr[i]);
            }

            //[004]排序
            std::sort(m_arrPosTable.begin(), m_arrPosTable.end(), PositionLower);

            delete[] pArr;
        }

    }

    //刷新所有视图数据
    endResetModel();
}

int         ModelNormalPosition::rowCount(const QModelIndex &parent) const
{
    return m_arrPosTable.size();
}
int         ModelNormalPosition::columnCount(const QModelIndex &parent) const
{
    return 9;
}

QVariant    ModelNormalPosition::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Vertical)
    {
        QString str = QString("%1").arg(section);
        return str;
    }
    if (orientation == Qt::Horizontal)
    {
        QString str = "";
        switch (section)
        {
        case 0:str = "合约"; break;
        case 1:str = "方向"; break;
        case 2:str = "总持仓"; break;
        case 3:str = "今日持仓"; break;
        case 4:str = "持仓成本"; break;
        case 5:str = "盈亏"; break;
        case 6:str = "套保标志"; break;
        case 7:str = "交易所"; break;
        }
        return str;
    }
}

QVariant    ModelNormalPosition::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if(role == Qt::FontRole) //文字字体
    {
        int col = index.column();
        if (col == 0)
        {
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }
    }
    else if (role == Qt::TextAlignmentRole)  //文字排列
    {
        int align = int(Qt::AlignRight | Qt::AlignVCenter);
        int col = index.column();
        int row = index.row();
        if (col == 0)
        {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        }
        else if (col == 1)
        {
            if (row >= 0 && row < m_arrPosTable.size())
            {
                return positionUtil::getAlignByDirection(m_arrPosTable[row].PosiDirection);
            }
        }
        return align;
    }
    else if (role == Qt::TextColorRole)   //文字颜色
    {
        int col = index.column();
        int row = index.row();
        if (row >= 0 && row < m_arrPosTable.size())
        {
            if(col ==0)  //代码的颜色
            {
                return QColor(72,114,196);
            }
            else if (col == 1)
            {
                return positionUtil::getColorByDirection(m_arrPosTable[row].PosiDirection);
            }
        }
    }
    else if (role == Qt::DisplayRole)
    {
        int row = index.row();
        int col = index.column();

        if (row < 0 || row >= m_arrPosTable.size())
        {
            return  QVariant();
        }

        if (col < 0 || col >= 9)
        {
            return QVariant();
        }

        QString str = "";
        const tagXTInvestorPositionField& o = m_arrPosTable[row];
        switch (col)
        {
        case 0:str = o.ExCode.Code; break;
        case 1:
        {
            str = positionUtil::getStringByPosiDirection(o);
        }
            break;
        case 2:str.sprintf("%d", o.Position); break;
        case 3:str.sprintf("%d", o.TodayPosition); break;
        case 4:str.sprintf("%.04lf", o.PositionCost); break;
        case 5:str.sprintf("%.04lf", o.PositionProfit); break;
        case 6:
        {
            str = positionUtil::getStringByHedgeFlag(o);
            break;
        }  
        case 7:
        {
            QExchange* pe = GetDataModule()->GetExchange(o.ExCode.ExchID);
            if (pe)
            {
                char szbuf[255] = { 0 };
                pe->GetExchName(szbuf);
                str = szbuf;
            }
            break;
        }

        }
        return str;
    }

    return QVariant();
}


//================================================================================================================================================================================================
//=================================================================================================================================================================================================

ModelCompositePosition::ModelCompositePosition(QObject *parent/* = nullptr*/)
{
    m_pMgr = nullptr;
}

void        ModelCompositePosition::release()
{
    m_arrPosTable.clear();
}

void        ModelCompositePosition::setOrderMgr(QOrderMgr* p)
{
    m_pMgr = p;

    beginResetModel();

    m_arrPosTable.clear();

    if (p)
    {

        int ncount = p->GetAllPosition(nullptr, 0);
        if (ncount > 0)
        {
            tagXTInvestorPositionField* pArr = new tagXTInvestorPositionField[ncount];
            p->GetAllPosition(pArr, ncount);

            for (int i = 0; i < ncount; i++)
            {
                //[001]过滤总持仓为0的条目
                if (pArr[i].Position <= 0)
                {
                    continue;
                }
                //[002]非组合持仓(code中含有&)
                if (!positionUtil::isCompositeCode(pArr[i].ExCode.Code))
                {
                    continue;
                }
                //[003]加入
                m_arrPosTable.push_back(pArr[i]);
            }

            //[004]排序
            std::sort(m_arrPosTable.begin(), m_arrPosTable.end(), PositionLower);

            delete[] pArr;
        }

    }

    //刷新所有视图数据
    endResetModel();
}

int         ModelCompositePosition::rowCount(const QModelIndex &parent) const
{
    return m_arrPosTable.size();
}
int         ModelCompositePosition::columnCount(const QModelIndex &parent) const
{
    return 5;
}

QVariant    ModelCompositePosition::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Vertical)
    {
        QString str = QString("%1").arg(section);
        return str;
    }
    if (orientation == Qt::Horizontal)
    {
        QString str = "";
        switch (section)
        {
        case 0:str = "合约代码"; break;
        case 1:str = "方向"; break;
        case 2:str = "持仓数量"; break;
        case 3:str = "持仓成本"; break;
        case 4:str = "套保标志"; break;
        }
        return str;
    }
}

QVariant    ModelCompositePosition::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if(role == Qt::FontRole) //文字字体
    {
        int col = index.column();
        if (col == 0)
        {
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }
    }
    else if (role == Qt::TextAlignmentRole)  //文字排列
    {
        int align = int(Qt::AlignRight | Qt::AlignVCenter);
        int col = index.column();
        int row = index.row();
        if (col == 0)
        {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        }
        else if (col == 1)
        {
            if (row >= 0 && row < m_arrPosTable.size())
            {
                return positionUtil::getAlignByDirection(m_arrPosTable[row].PosiDirection);
            }
        }
        return align;
    }
    else if (role == Qt::TextColorRole)   //文字颜色
    {
        int col = index.column();
        int row = index.row();
        if (row >= 0 && row < m_arrPosTable.size())
        {
            if(col ==0)  //代码的颜色
            {
                return QColor(72,114,196);
            }
            else if (col == 1)
            {
                return positionUtil::getColorByDirection(m_arrPosTable[row].PosiDirection);
            }
        }
    }
    else if (role == Qt::DisplayRole)
    {
        int row = index.row();
        int col = index.column();

        if (row < 0 || row >= m_arrPosTable.size())
        {
            return  QVariant();
        }

        if (col < 0 || col >= 5)
        {
            return QVariant();
        }

        QString str = "";
        const tagXTInvestorPositionField& o = m_arrPosTable[row];
        switch (col)
        {
        case 0:str = o.ExCode.Code; break;
        case 1:
        {
            str = positionUtil::getStringByPosiDirection(o);
        }
            break;
        case 2:str.sprintf("%d", o.Position); break;
        case 3:str.sprintf("%.04lf", o.PositionCost); break;
        case 4:
        {
            str = positionUtil::getStringByHedgeFlag(o);
        }
            break;

        }
        return str;
    }

    return QVariant();
}

