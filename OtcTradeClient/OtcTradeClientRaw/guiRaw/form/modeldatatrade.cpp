
#include "modeldatatrade.h"
#include "trademodule.h"
#include "datamodule.h"

//排序依据:
//21:00 --24:00 第一梯队   00：00- 16:00 第二梯队
//梯队小的靠前，如果梯队一致，则时间小的靠前   (之所以不用日期，是因为CZCE 的夜盘，使用的是第二天的日期)
bool TradeLower(tagXTTradeField& a, tagXTTradeField& b)
{
    int levelone = 0;
    int leveltwo = 0;
    if (a.TradeTime >= 200000)
        levelone = 1;
    else
        levelone = 2;

    if (b.TradeTime >= 200000)
        leveltwo = 1;
    else
        leveltwo = 2;

    if (levelone < leveltwo)
    {
        return false;
    }
    else if (levelone == leveltwo)
    {
        if (a.TradeTime < b.TradeTime)
            return false;

        return true;
    }

    return true;
}

QColor      getColorByDirection(char Direction)
{
    if (Direction == XT_D_Buy)
    {
        return QColor(204, 51, 51);
    }
    else if (Direction == XT_D_Sell)
    {
        return QColor(0, 128, 0);
    }
    else
    {
        return QColor(201, 48, 201);
    }
}

QColor      getColorByOffset(char offset)
{
    if (offset != XT_OF_Open)
    {
        return QColor(201, 48, 201);
    }
    else
    {
        return QColor(0, 0, 0);
    }
}

int         getAlignByDirection(char Direction)
{
    if (Direction == XT_D_Buy)
    {
        return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else if (Direction == XT_D_Sell)
    {
        return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    else
    {
        return int(Qt::AlignCenter | Qt::AlignVCenter);
    }

}

//=================================================================================================================================================================
//=================================================================================================================================================================
//=================================================================================================================================================================
//=================================================================================================================================================================
ModelSingleTrade::ModelSingleTrade(QObject *parent /*= nullptr*/)
{
    m_pMgr = nullptr;

}

void        ModelSingleTrade::release()
{
    m_arrTradeTable.clear();
}

void        ModelSingleTrade::setOrderMgr(QOrderMgr* p)
{
    m_pMgr = p;

    beginResetModel();

    m_arrTradeTable.clear();

    if (p)
    {

        int ncount = p->GetAllTrade(nullptr, 0);
        if (ncount > 0)
        {
            tagXTTradeField* pArr = new tagXTTradeField[ncount];
            p->GetAllTrade(pArr, ncount);

            for (int i = 0; i < ncount; i++)
            {
                //[001]加入
                m_arrTradeTable.push_back(pArr[i]);
            }

            //[004]排序
            std::sort(m_arrTradeTable.begin(), m_arrTradeTable.end(), TradeLower);

            delete[] pArr;
        }

    }

    //刷新所有视图数据
    endResetModel();
}

int         ModelSingleTrade::rowCount(const QModelIndex &parent) const
{
    return m_arrTradeTable.size();
}

int         ModelSingleTrade::columnCount(const QModelIndex &parent) const
{
    return 11;
}

QVariant    ModelSingleTrade::headerData(int section, Qt::Orientation orientation, int role) const
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
        case 0:str = "成交ID"; break;
        case 1:str = "合约"; break;
        case 2:str = "方向"; break;
        case 3:str = "开平"; break;
        case 4:str = "成交价"; break;
        case 5:str = "成交数量"; break;
        case 6:str = "时间"; break;
        case 7:str = "委托单柜台编号"; break;
        case 8:str = "类型"; break;
        case 9:str = "套保"; break;
        case 10:str = "交易所"; break;
        }
        return str;
    }
}

QVariant    ModelSingleTrade::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if(role == Qt::FontRole) //文字字体
    {
        int col = index.column();
        if (col == 1)
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
        if (col == 1)
        {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        }
        else if (col == 2)
        {
            if (row >= 0 && row < m_arrTradeTable.size())
            {
                return getAlignByDirection(m_arrTradeTable[row].Direction);
            }
        }
        return align;
    }
    else if (role == Qt::TextColorRole)   //文字颜色
    {
        int col = index.column();
        int row = index.row();
        if (row >= 0 && row < m_arrTradeTable.size())
        {
            if(col == 0 || col == 7)
            {
                return QColor(142,142,142);  //TradeID, OrderSysID 的颜色，改为不起眼
            }
            else if(col ==1)  //代码的颜色
            {
                return QColor(72,114,196);
            }
            else if (col == 2)
            {
                return getColorByDirection(m_arrTradeTable[row].Direction);
            }
            else if(col ==3)
            {
                return getColorByOffset(m_arrTradeTable[row].OffsetFlag);
            }
        }
    }
    else if (role == Qt::DisplayRole)
    {
        int row = index.row();
        int col = index.column();

        if (row < 0 || row >= m_arrTradeTable.size())
        {
            return  QVariant();
        }

        if (col < 0 || col >= 11)
        {
            return QVariant();
        }

        QString str = "";
        const tagXTTradeField& o = m_arrTradeTable[row];
        switch (col)
        {
        case 0:str = o.TradeID; break;
        case 1:str = o.ExCode.Code; break;
        case 2:
        {
            if (o.Direction == XT_D_Buy)
                str = "买";
            else if (o.Direction == XT_D_Sell)
                str = "卖";
            else
                str = "其它";
        }
            break;
        case 3:
        {
            if (o.OffsetFlag == XT_OF_Open)
                str = "开仓";
            else if (o.OffsetFlag == XT_OF_Close)
                str = "平仓";
            else if (o.OffsetFlag == XT_OF_CloseToday)
                str = "平今";
            else if (o.OffsetFlag == XT_OF_CloseYesterday)
                str = "平昨";
            else if (o.OffsetFlag == XT_OF_ForceClose)
                str = "强平";
            else
                str = "其它";
        }
            break;
        case 4:str.sprintf("%.04lf", o.Price); break;
        case 5:str.sprintf("%d", o.Volume); break;
        case 6:
        {
            int ntime = o.TradeTime;
            str.sprintf("%02d:%02d:%02d", ntime / 10000, (ntime % 10000) / 100, ntime % 100);
        }
            break;
        case 7:str = o.OrderSysID; break;
        case 8:
        {
            if (o.TradeType == XT_TT_CombinationDerived)
                str = "拆分";
            else if (o.TradeType == XT_TT_Common)
                str = "普通成交";
            else if (o.TradeType == XT_TT_OptionsExecution)
                str = "期权执行";
            else if (o.TradeType == XT_TT_OTC)
                str = "OTC成交";
            else if (o.TradeType == XT_TT_EFPDerived)
                str = "期转现衍生成交";
            else if (o.TradeType == XT_TT_CombinationDerived)
                str = "组合衍生成交";
        }
            break;
        case 9:
        {
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
        }
            break;

        case 10:
        {
            QExchange* pe = GetDataModule()->GetExchange(o.ExCode.ExchID);
            if (pe)
            {
                char szbuf[255] = { 0 };
                pe->GetExchName(szbuf);
                str = szbuf;
            }
        }
            break;
        }
        return str;
    }

    return QVariant();
}


//=================================================================================================================================================================
//=================================================================================================================================================================
//=================================================================================================================================================================
//=================================================================================================================================================================
ModelTotalTrade::ModelTotalTrade(QObject *parent /*= nullptr*/)
{
    m_pMgr = nullptr;
}

void        ModelTotalTrade::release()
{
    m_arrTradeSum.clear();
}

int         ModelTotalTrade::rowCount(const QModelIndex &parent) const
{
    return m_arrTradeSum.size();
}

int         ModelTotalTrade::columnCount(const QModelIndex &parent) const
{
    return 8;
}

QVariant    ModelTotalTrade::headerData(int section, Qt::Orientation orientation, int role) const
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
        case 1:str = "品种"; break;
        case 2:str = "买卖"; break;
        case 3:str = "开平"; break;
        case 4:str = "成交均价"; break;
        case 5:str = "成交手数"; break;
        case 6:str = "套保"; break;
        case 7:str = "交易所"; break;
        }
        return str;
    }
}

QVariant    ModelTotalTrade::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if(role == Qt::FontRole) //文字字体
    {
        int col = index.column();
        if (col == 1)
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
        if (col == 0 || col ==1 )
        {
            return int(Qt::AlignLeft | Qt::AlignVCenter);
        }
        else if (col == 2)
        {
            if (row >= 0 && row < m_arrTradeSum.size())
            {
                return getAlignByDirection(m_arrTradeSum[row].Direction);
            }
        }
        return align;
    }
    else if (role == Qt::TextColorRole)   //文字颜色
    {
        int col = index.column();
        int row = index.row();
        if (row >= 0 && row < m_arrTradeSum.size())
        {
            if(col == 1)
            {
                return QColor(142,142,142);  //品种 的颜色，改为不起眼
            }
            else if(col ==0)  //代码的颜色
            {
                return QColor(72,114,196);
            }
            else if (col == 2)
            {
                return getColorByDirection(m_arrTradeSum[row].Direction);
            }
            else if(col ==3)
            {
                return getColorByOffset(m_arrTradeSum[row].Offset);
            }
        }
    }
    else if (role == Qt::DisplayRole)
    {
        int row = index.row();
        int col = index.column();

        if (row < 0 || row >= m_arrTradeSum.size())
        {
            return  QVariant();
        }

        if (col < 0 || col >= 11)
        {
            return QVariant();
        }

        QString str = "";
        const tagTradeSumInfo& o = m_arrTradeSum[row];
        switch (col)
        {
        case 0:str = o.ExCode.Code; break;
        case 1:str = o.Product; break;
        case 2:
        {
            if (o.Direction == XT_D_Buy)
                str = "买";
            else if (o.Direction == XT_D_Sell)
                str = "卖";
            else
                str = "其它";
        }
            break;
        case 3:
        {
            if (o.Offset == XT_OF_Open)
                str = "开仓";
            else if (o.Offset == XT_OF_Close)
                str = "平仓";
            else if (o.Offset == XT_OF_CloseToday)
                str = "平今";
            else if (o.Offset == XT_OF_CloseYesterday)
                str = "平昨";
            else if (o.Offset == XT_OF_ForceClose)
                str = "强平";
            else
                str = "其它";
        }
            break;
        case 4:str.sprintf("%.04lf", o.Average); break;
        case 5:str.sprintf("%d", o.Volume); break;
        case 6:
        {
            if (o.Hedge == XT_HF_Arbitrage)
                str = "套利";
            else if (o.Hedge == XT_HF_Hedge)
                str = "套保";
            else if (o.Hedge == XT_HF_Covered)
                str = "备兑";
            else if (o.Hedge == XT_HF_Speculation)
                str = "投机";
            else
                str = "其他";
        }
            break;
        case 7:
        {
            QExchange* pe = GetDataModule()->GetExchange(o.ExCode.ExchID);
            if (pe)
            {
                char szbuf[255] = { 0 };
                pe->GetExchName(szbuf);
                str = szbuf;
            }
        }
            break;
        }
        return str;
    }

    return QVariant();
}

void        ModelTotalTrade::setOrderMgr(QOrderMgr* p)
{
    m_pMgr = p;

    beginResetModel();

    m_arrTradeSum.clear();
    map<string, tagTradeSumInfo> m_map;


    if (p)
    {

        int ncount = p->GetAllTrade(nullptr, 0);
        if (ncount > 0)
        {
            tagXTTradeField* pArr = new tagXTTradeField[ncount];
            p->GetAllTrade(pArr, ncount);

            for (int i = 0; i < ncount; i++)
            {
                //[001]加入
                //按照  exchange_code_direction_offset_hedge
                char szkey[128] = {0};
                _snprintf(szkey, 127, "%c_%s_%c_%c_%c", pArr[i].ExCode.ExchID, pArr[i].ExCode.Code, pArr[i].Direction, pArr[i].OffsetFlag, pArr[i].HedgeFlag
                          );
                string strkey = szkey;
                map<string, tagTradeSumInfo>::iterator itb = m_map.find(strkey);
                if(itb != m_map.end() )
                {
                    double oldAmount = itb->second.Average * itb->second.Volume;
                    itb->second.Volume += pArr[i].Volume;
                    itb->second.Average = (oldAmount + pArr[i].Volume * pArr[i].Price)/ (itb->second.Volume);
                }
                else
                {
                    tagTradeSumInfo oInfo;
                    memset(&oInfo, 0, sizeof(oInfo));
                    oInfo.ExCode = pArr[i].ExCode;
                    oInfo.Direction = pArr[i].Direction;
                    oInfo.Offset = pArr[i].OffsetFlag;
                    oInfo.Average = pArr[i].Price;
                    oInfo.Volume = pArr[i].Volume;
                    tagXTNameTable oNameTable;
                    GetDataModule()->GetExchange(oInfo.ExCode.ExchID)->GetOneNameTable(&oInfo.ExCode, &oNameTable);
                    strcpy(oInfo.Product, oNameTable.ProductID);
                    oInfo.Hedge = pArr[i].HedgeFlag;

                    m_map.insert(make_pair(strkey, oInfo));
                }

            }

            delete[] pArr;
        }

    }

    map<string, tagTradeSumInfo>::iterator itb =m_map.begin();
    while(itb != m_map.end() )
    {
        m_arrTradeSum.push_back(itb->second);
        itb++;
    }

    //刷新所有视图数据
    endResetModel();
}























