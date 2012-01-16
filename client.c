#include "myqueue.h"

int main(int o_argc, const_stringv_t o_argv) {

  if(o_argc < 2) {
    println("Usage: client values ...");
    exit(EXIT_SUCCESS);
  }

  byte          _buffer[MSG_SIZE]; /* recieved message buffer */
  struct st_msg _stout           ; /* send message data structure */
  Msg           _msg, _out       ; /* send and recieved message references */
  size_t        _msglen          ; /* length of the send message string */
  
  _out = &_stout;
  
  key_t          _key   ; /* IPC key */
  int            _handle; /* IPC id  */
  struct msgbuf* _msgbuf = cast(struct msgbuf*)_buffer;
 
  /* open an existing quaue */ 
  asserts( _key = ftok(".", 'x') );
  asserts( _handle = msgget(_key, S_IRUSR | S_IWUSR) );
  
  _out->from   = getpid()  ; /* set 'from' field into current process id */
  _out->to     = 1         ; /* set 'to' field into 1 which means server */
  _out->length = o_argc - 1; /* set vector length to number of command line arguments */
  
  /* allocate space for apropriate number of values */
  assertnz( _out->values = malloc(sizeof(int) * _out->length) );

  /* setup values */
  for(size_t i = 0; i < _out->length; i++) {
    _out->values[i] = atoi(o_argv[i + 1]);
  }
    
  /* prepare message buffer */
  _msgbuf->mtype = _out->to;
  assertnz( msg_save(_out, _msgbuf->mtext, MSG_SIZE - sizeof(long) ) );
  _msglen = strlen(_msgbuf->mtext) + sizeof(msgbuf);
  assertnz(_msglen <= MSG_SIZE); /* respect the message size limit */
  
  /* send the request */
  asserts( msgsnd(_handle, _msgbuf, _msglen, 0) );
  
  /* wait for the respose */
  asserts( msgrcv(_handle, _msgbuf, MSG_SIZE, _out->from, 0) );
  assertnz( _msg = msg_load(_msgbuf->mtext) );
  
  /* write the respose into the standard output */
  println("client recieved:");
  println(_msgbuf->mtext);
  
  _msg = msg_destroy(_msg);
  
  exit(EXIT_SUCCESS);
}
