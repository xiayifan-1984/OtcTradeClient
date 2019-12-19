#ifndef INTERNALMESSAGING_H
#define INTERNALMESSAGING_H


typedef int (*fnConsumeCB)(const char*, int, long long, int, const char*);

class IKafkaMsg;
class CKafkaUnit
{
public:
   CKafkaUnit();

public:
   void     Instance();
   void     Release();

   void     initProducer(const char*);
   void     initConsumer(const char *, const char *, fnConsumeCB , int , char**);
    
   void     sendMsg(const char*, char*, int);
   void     startConsume();
   void     stopConsume();
    
 private:
   IKafkaMsg*  m_impl;
};

extern CKafkaUnit*      GetKafkaUnit();

extern int              fnMsgHandler(const char* topic, int partition, long long offset,
                                       int buflen, const char* buf);


                                       
#endif // INTERNALMESSAGING_H
