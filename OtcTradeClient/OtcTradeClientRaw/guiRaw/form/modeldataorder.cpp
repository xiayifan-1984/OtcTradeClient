#include "modeldataorder.h"
#include "trademodule.h"
#include "datamodule.h"
#include "XTCodec.h"

//extern ParkOrderBox* getParkOrderBox();
static void transOrder(tagXTInputOrderField& inputOrder, tagXTOrderField& outputOrder)
{
    memset(&outputOrder, 0, sizeof(tagXTOrderField));
    outputOrder.BrokerID = inputOrder.BrokerID;
    outputOrder.Direction = inputOrder.Direction;
    outputOrder.LimitPrice = inputOrder.LimitPrice;
    outputOrder.Volume = inputOrder.Volume;
    strncpy(outputOrder.OffsetFlag, inputOrder.OffsetFlag, strlen(inputOrder.OffsetFlag));
    strncpy(outputOrder.UserID, outputOrder.UserID, sizeof(outputOrder.UserID));
    outputOrder.PriceType = inputOrder.PriceType;
    memcpy(&outputOrder.ExCode, &inputOrder.ExCode, sizeof(inputOrder.ExCode));
    strncpy(outputOrder.OrderRef, inputOrder.OrderRef, strlen(inputOrder.OrderRef));

}
//排序依据:
//21:00 --24:00 第一梯队   00：00- 16:00 第二梯队
//梯队小的靠前，如果梯队一致，则时间小的靠前   (之所以不用日期，是因为CZCE 的夜盘，使用的是第二天的日期)
bool OrderLower(const tagXTOrderField& a, const tagXTOrderField& b)
{
    int levelone = 0;
    int leveltwo = 0;
    if (a.InsertTime >= 200000)
        levelone = 1;
    else
        levelone = 2;

    if (b.InsertTime >= 200000)
        leveltwo = 1;
    else
        leveltwo = 2;

    if (levelone < leveltwo)
    {
        return false;
    }
    else if (levelone == leveltwo)
    {
        if (a.InsertTime <= b.InsertTime)
            return false;

        return true;
    }

    return true;
}


ModelSingleOrder::ModelSingleOrder(QObject *parent /* = nullptr */)
{
    m_pMgr = nullptr;
    m_showOrderType = ORDER_TO_SHOW::ANY_ORDER;
}

void        ModelSingleOrder::release()
{
    m_arrOrderTable.clear();
}

void        ModelSingleOrder::setOrderMgr(QOrderMgr* p)
{

    beginResetModel();

    release();
    m_pMgr = p;
    if(m_showOrderType == ORDER_TO_SHOW::PARK_ORDER)
    {
    // reserve for parked order
    }
    else
    {
        if(p)
        {
            int ncount = p->GetAllOrder(nullptr, 0);
            if (ncount > 0)
            {
                tagXTOrderField* pArr = new tagXTOrderField[ncount];
                p->GetAllOrder(pArr, ncount);

                for (int i = 0; i < ncount; i++)
                {
                    //[001]加入
                    m_arrOrderTable.push_back(pArr[i]);
                }

                //[004]排序
                std::sort(m_arrOrderTable.begin(), m_arrOrderTable.end(), OrderLower);
                if(m_showOrderType == ORDER_TO_SHOW::ACCEPTED_ORDER)
                {
                    m_arrOrderTable.erase(remove_if(m_arrOrderTable.begin(), m_arrOrderTable.end(), [](tagXTOrderField& p)
                    {return p.OrderStatus != XT_OST_Accepted;}), m_arrOrderTable.end());
                }
                delete[] pArr;
            }
        }
    }

    //刷新所有视图数据
    endResetModel();
}

void ModelSingleOrder::setOrderToShow(ORDER_TO_SHOW toShow)
{
    m_showOrderType = toShow;
}

int         ModelSingleOrder::rowCount(const QModelIndex &parent) const
{
    return  m_arrOrderTable.size();
}

#define     CURRENT_COL_COUNT  (15)
int         ModelSingleOrder::columnCount(const QModelIndex &parent) const
{
    return  CURRENT_COL_COUNT;
}

QVariant    ModelSingleOrder::headerData(int section, Qt::Orientation orientation, int role) const
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
        case 0:str = "OrderSysID"; break;
        case 1:str = "Code"; break;
        case 2:str = "Direction"; break;
        case 3:str = "Offset"; break;
        case 4:str = "Status";break;

        case 5:str = "Price";break;
        case 6:str = "oriVolume";break;
        case 7:str = "UnTraded";break;
        case 8:str = "TradedVol";break;
        case 9:str="msg";break;
        case 10:str="InsertTime";break;
        case 11:str="UpdateTime";break;
        case 12:str="OrderRef";break;
        case 13:str="Hedge";break;
        case 14:str="Exchange";break;
        }
        return str;
    }
}

QColor      ModelSingleOrder::getColorByDirection(char Direction) const
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

QColor      ModelSingleOrder::getColorByOffset(char offset) const
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

int         ModelSingleOrder::getAlignByDirection(char Direction) const
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

QVariant    ModelSingleOrder::data(const QModelIndex &index, int role) const
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
            if (row >= 0 && row < m_arrOrderTable.size())
            {
                return getAlignByDirection(m_arrOrderTable[row].Direction);
            }
        }
        return align;
    }
    else if (role == Qt::TextColorRole)   //文字颜色
    {
        int col = index.column();
        int row = index.row();
        if (row >= 0 && row < m_arrOrderTable.size())
        {
            if(m_arrOrderTable[row].OrderStatus == XT_OST_Canceled)
            {
                return QColor(142,142,142);
            }
            if(col == 0)
            {
                return QColor(142,142,142);  //OrderSysID 的颜色，改为不起眼
            }
            else if(col ==1)  //代码的颜色
            {
                return QColor(72,114,196);
            }
            else if (col == 2)  //买卖的颜色
            {
                return getColorByDirection(m_arrOrderTable[row].Direction);
            }
            else if(col == 3)   //开平的颜色
            {
                return getColorByOffset(m_arrOrderTable[row].OffsetFlag[0]);
            }
            else if(col == 4)   //委托单状态的颜色(完全成交绿色，未成交，暗黄色)
            {
                if(m_arrOrderTable[row].OrderStatus == XT_OST_Traded)
                {
                    return QColor(0, 128, 0);
                }
                else
                {
                    return QColor(128,128,0);
                }
            }

        }

    }
    else if (role == Qt::DisplayRole)
    {
        int row = index.row();
        int col = index.column();

        if (row < 0 || row >= m_arrOrderTable.size())
        {
            return  QVariant();
        }

        if (col < 0 || col >= CURRENT_COL_COUNT )
        {
            return QVariant();
        }

        QString str = "";
        const tagXTOrderField& o = m_arrOrderTable[row];
        switch (col)
        {
        case 0:str = o.OrderSysID; break;
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
            if (o.OffsetFlag[0] == XT_OF_Open)
                str = "开仓";
            else if (o.OffsetFlag[0] == XT_OF_Close)
                str = "平仓";
            else if (o.OffsetFlag[0] == XT_OF_CloseToday)
                str = "平今";
            else if (o.OffsetFlag[0] == XT_OF_CloseYesterday)
                str = "平昨";
            else if (o.OffsetFlag[0] == XT_OF_ForceClose)
                str = "强平";
            else
                str = "其它";
        }
            break;
        case 4:
        {
            if(o.OrderStatus == XT_OST_Traded)
                str = "已成交";
            else if(o.OrderStatus == XT_OST_InsertSubmitted)
                str = "待报";
            else if(o.OrderStatus == XT_OST_Accepted)
                str = "已报";
            else if(o.OrderStatus == XT_OST_CancelSubmitted)
                str = "待撤";
            else if(o.OrderStatus == XT_OST_Canceled)
                str = "已撤单";
            else if(o.OrderStatus == XT_OST_Void)
                str = "废单";
            else if(o.OrderStatus == XT_OST_Unknown)
                str = "未知1";
        }
            break;
        case 5:str.sprintf("%.04lf", o.LimitPrice); break;
        case 6:str.sprintf("%d", o.Volume); break;
        case 7:str.sprintf("%d", o.VolumeTotal); break;
        case 8:str.sprintf("%d", o.VolumeTraded); break;
        case 9:
        {
            std::string strmsg = o.StatusMsg;
            str = XTCodec::AfGbk_ToQString(strmsg);
        }
            break;
        case 10:
        {
            int ntime = o.InsertTime;
            str.sprintf("%02d:%02d:%02d", ntime / 10000, (ntime % 10000) / 100, ntime % 100);
        }
            break;
        case 11:
        {
            int ntime = o.UpdateTime;
            str.sprintf("%02d:%02d:%02d", ntime / 10000, (ntime % 10000) / 100, ntime % 100);
        }
            break;
        case 12:str = o.OrderRef; break;
        case 13:
        {
            if (o.HedgeFlag[0] == XT_HF_Arbitrage)
                str = "套利";
            else if (o.HedgeFlag[0] == XT_HF_Hedge)
                str = "套保";
            else if (o.HedgeFlag[0] == XT_HF_Covered)
                str = "备兑";
            else if (o.HedgeFlag[0] == XT_HF_Speculation)
                str = "投机";
            else
                str = "其他";
        }
            break;
        case 14:
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

tagXTOrderField*    ModelSingleOrder::getDataByRow(const QModelIndex& index)
{
    tagXTOrderField* p = nullptr;
    if (!index.isValid())
    {
        return p;
    }

    int row = index.row();

    if (row < 0 || row >= m_arrOrderTable.size())
    {
        return  p;
    }

    p = &m_arrOrderTable[row];
    return p;
}

