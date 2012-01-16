#include "myqueue.h"

int main() {
  byte          _buffer[MSG_SIZE]; /* recieved message buffer */
  struct st_msg _stout           ; /* send message data structure */
  Msg           _msg, _out       ; /* send and recieved message references */
  size_t        _msglen          ; /* length of the send message string */
  int           _val             ; /* result of the multiplication  */
  _out = &_stout;
  
  key_t          _key   ; /* IPC key */
  int            _handle; /* IPC id  */
  struct msgbuf* _msgbuf = cast(msgbuf*)_buffer;
  
  /* create or open an existing quaue */ 
  asserts( _key = ftok(".", 'x') );
  asserts( _handle = msgget(_key, IPC_CREAT | S_IRUSR | S_IWUSR) );
  
  _out->from   = 1    ;
  _out->to     = 0    ;
  _out->length = 1    ;
  _out->values = &_val;

  while(TRUE) {
    /* wait for the request */
    asserts( msgrcv(_handle, _msgbuf, MSG_SIZE, _out->from, 0) );
    assertnz( _msg = msg_load(_msgbuf->mtext) );
    
    /* write the request into the standard output */
    println("server recieved:");
    println(_msgbuf->mtext);
    
    /* do multiplication */
    _out->to = _msg->from;
    if(_msg->length > 0) _val = _msg->values[0];
    for(size_t i = 1; i < _msg->length; i++) {
      _val *= _msg->values[i];
    }
    _msg = msg_destroy(_msg);
    
    /* prepare the response message string */
    _msgbuf->mtype = _out->to;
    assertnz( msg_save( _out, _msgbuf->mtext, MSG_SIZE - sizeof(long) ) );
    _msglen = strlen(_msgbuf->mtext) + sizeof(msgbuf); /* sizeof(msgbuf) = sizeof(long) + 1 */
    assertnz(_msglen <= MSG_SIZE); /* respect the message size limit */

    /* send the response */
    asserts( msgsnd(_handle, _msgbuf, _msglen + sizeof(long), 0) );
  }
  
  /* This program should be terminated by signal */
  exit(EXIT_SUCCESS);
}
