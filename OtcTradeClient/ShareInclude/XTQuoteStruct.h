#ifndef _XTQUOTESTRUCT_DEFINE_H
#define _XTQUOTESTRUCT_DEFINE_H

#include "XTBase.h"
#pragma pack(1)

//=============================================================================================================================================
//Quote Struct
//=============================================================================================================================================
typedef struct  
{
	double					Price;						//价格
	int						Volume;						//数量(股、份、张、手)
}tagPriceItem;


//期权\股票\期货行情快照
typedef struct  
{
	int						UpdateTime;					//时间(HHMMSSmmm) 

	int						TradingDay;					//业务日期(YYYYMMDD)
	
	tagXTInstrument			ExCode;						//合约代码
	
	double					OpenPrice;					//今开 
	double					HighestPrice;				//最高价
	double					LowestPrice;				//最低价
	double					LastPrice;					//最新价 

	int 					Volume;						//成交量 
	double					Turnover;					//成交金额
	
	double					PreSettlementPrice;			//昨结
	double					PreClosePrice;				//昨收
	int						PreOpenInterest;			//昨持仓

	int						OpenInterest;				//持仓
	double					ClosePrice;					//收盘价
	double					SettlementPrice;			//结算价

	double					UpperLimitPrice;			//涨停价
	double					LowerLimitPrice;			//跌停价

	tagPriceItem			BuyItem[5];					//5档买盘
	tagPriceItem			SellItem[5];				//5档卖盘

	union
	{
		struct 
		{
			double					PreIOPV;					//昨日净值
			double					IOPV;						//净值 
			double					AuctionPrice;				//竞价的价格

			char					TradingPhase;				//交易阶段(见交易阶段定义)
		};
		struct 
		{
			double					PreDelta;					///昨虚实度
			double					CurrDelta;					///今虚实度
			double					AveragePrice;				///当日均价
		};

		char						reserved_[32];				//保留
	};

}tagXTSnapshot;


/*
typedef struct
{
	///交易日YYYYMMDD
	int						TradingDay;	
	//合约代码
	tagXTInstrument			ExCode;
	///询价编号
	char_21					ForQuoteSysID;	
	///询价时间HHMMSS
	int						ForQuoteTime;	
	///业务日期YYYYMMDD
	int						ActionDay;					

}tagXTForQuoteRspField;
*/




#pragma pack()

#endif
