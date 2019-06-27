
#pragma once
#include "XTTradeStruct.h"

class CXTTradeSpi
{
public:
	virtual void	OnFrontConnected(){};
	virtual void	OnFrontDisconnected(int nReason){};

	virtual void	OnRspUserLogin(tagXTRspInfoField *pRspInfo, bool bIsLast) {};
	
	virtual void	OnRspQryOrder(tagXTOrderField *pOrder, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast) {};
	virtual void	OnRspQryTrade(tagXTTradeField *pTrade, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast) {};
	virtual void	OnRspQryInvestorPosition(tagXTInvestorPositionField *pInvestorPosition, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast) {};
	virtual void	OnRspQryTradingAccount(tagXTTradingAccountField* pAccount, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast) {};
	virtual void	OnRspError(tagXTRspInfoField *pRspInfo, bool bIsLast) {};

	virtual void	OnRspOrderInsert(tagXTInputOrderField *pInputOrder, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast) {};
	virtual void	OnRspOrderAction(tagXTInputOrderActionField *pInputOrderAction, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast) {};

	virtual void	OnRtnOrder(tagXTOrderField *pOrder) {};
	virtual void	OnRtnTrade(tagXTTradeField *pTrade) {};

	virtual void	OnRspExtend(int nExFunctionID, void *pData, int nDataLength, tagXTRspInfoField *pRspInfo,int nRequestID, bool bIsLast) {};
};


class CXTTradeApi
{
public:
        virtual void            Release() = 0;

	virtual int		Init() = 0;

        virtual void            RegisterSpi(CXTTradeSpi *pSpi) = 0;

	virtual int		ReqOrderInsert(tagXTInputOrderField *pInputOrder, int nRequestID) = 0;

	virtual int		ReqOrderAction(tagXTInputOrderActionField *pInputOrderAction, int nRequestID) = 0;

	virtual int		ReqExtend(int nExFunctionID, void *pData, int nDataLength, int nRequestID) = 0;

	virtual int		ReqQryOrder(tagXTQryOrderField *pQryOrder, int nRequestID) =0;
	virtual int		ReqQryTrade(tagXTQryTradeField *pQryTrade, int nRequestID) =0;
	virtual int		ReqQryInvestorPosition(tagXTQryInvestorPositionField *pQryInvestorPosition, int nRequestID) =0;
	virtual int		ReqQryTradingAccount(tagXTQryTradingAccountField* pQryAccount, int nRequestID) = 0;

protected:
	virtual ~CXTTradeApi(){};

};


