#ifndef __MYQUEUE_H__
#define __MYQUEUE_H__

#include "utils.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MQ_NAME  "/client-server-mq"
#define MSG_SIZE 0x100

typedef struct st_msg {
  pid_t from;
  pid_t to  ;
  
  size_t length;
  int*   values;
} *Msg;

typedef struct msgbuf {
  long mtype;       /* message type, must be > 0 */
  char mtext[1];    /* message data */
} msgbuf;
 
#define isUpper(chr) ( (chr) >= 'A' && (chr) <= 'Z' ? TRUE : FALSE )
#define isLower(chr) ( (chr) >= 'a' && (chr) <= 'z' ? TRUE : FALSE )
#define isDigit(chr) ( (chr) >= '0' && (chr) <= '9' ? TRUE : FALSE )
#define isLetter(chr) (isUpper(chr) || isLower(chr) ? TRUE : FALSE)
#define isAlpha(chr) (isLetter(chr) || isDigit(chr) ? TRUE : FALSE)
#define isWhitespace(chr) \
  ( (chr) == ' ' || (chr) == '\r' || (chr) == '\n' || (chr) == '\t' ? TRUE : FALSE )

extern Msg msg_load(const string data);
extern string msg_save(const Msg data, string buffer, size_t size);
extern Msg msg_destroy(Msg msg);

#endif /* __MYQUEUE_H__ */
