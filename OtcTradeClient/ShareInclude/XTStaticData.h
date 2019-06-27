
#pragma once
#include "XTBase.h"

#pragma pack(1)

//.............................................................................................................................................................
typedef struct  
{
	tagXTInstrument				ExCode;
    char						Name[32];
	char						ContractID[32];			//合约在交易所的代码
	char						ProductClass;			//文档另
	char						ProductID[10];			//文档另

	char						Group;					//文档另(期货,期权,A股，B股，债券)
	tagXTInstrument				Underlying;				//标的(如果能填)

	int							VolumeMultiple;			//合约乘数 
	double						PriceTick;				//价格变动Tick
	int							Delivery;				//交割年月YYYYMM
	int							ExecPrice;				//执行价(放大SHOPT_MULTI)
	char						InstrumentType;			//合约类型，call or put, normal

	int							CreateDate;				//创建日
	int							OpenDate;				//上市日
	int							ExpireDate;				//到期日 
	int							StartDelivDate;			//开始交割日
	int							EndDelivDate;			//结束交割日

	//
	char						InstLifePhase;			//合约生命周期状态(未上市，上市，停牌，到期)
	int							IsTrading;				//是否交易
	char						PositionType;			//持仓类型(净，双边)
	char						PosTradeType;			//持仓交易类型(T+0, T+1)

	int 						InnerProductID;

        int                                             LotSize;        //手比率(期货，期权都是1， 股票、ETF、国债等需要注意)
        int                                             ShowDot;        //显示小数位数(根据priceTick推算出来的)

	char						_reserved[32];
}tagXTNameTable;

typedef struct 
{
	char						ProductID[10];			//代码
	char						Name[32];				//名称
	unsigned char				ExchID;					//交易所
	char						ProductClass;			//产品类型
	int							Multiple;				//合约乘数
	double						PriceTick;				//PriceTick
	int 						InnerProductID;			//内部数值的ID
}tagXTProduct;



#pragma pack()
