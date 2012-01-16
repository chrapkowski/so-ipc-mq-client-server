#include "myqueue.h"

/* Changes each occurence of uppercase [A-Z] letter to the corresponding lowercase [a-z] letter.
**/
static string toLower(const string o_str) {

  string _out = malloc(strlen(o_str) + 1);
  size_t i = 0;
  
  const int CASE_OFFSET = 'a' - 'A'; // 0x61 - 0x41 = 0x20
  for(; o_str[i] != '\0'; i++) if( isUpper(o_str[i]) )
    _out[i] += CASE_OFFSET
  ;
  else
    _out[i] = o_str[i]
  ;
  
  _out[i] = '\0';
  
  return _out;
}

/* This function matches pattern: [A-Za-z][A-Za-z0-9]+
 * caller must free the result
**/
static string parseID(const string o_data, size_t* o_offset) {
  string _out = NULL;
  size_t _end = (*o_offset);
  uint   _len = 0;
  if( isLetter(o_data[_end]) ) while( o_data[++_end] != '\0' && isAlpha(o_data[_end]) );
  
  if( ( _len = _end - (*o_offset) ) != 0 ) {
    _out = malloc(_len);
  }
  
  _out = strncpy(_out, o_data + (*o_offset), _len);
  _out[_len] = '\0';
  (*o_offset) = _end;
  return _out;
}

/* This function moves the offset out of the whitespace
**/
static void parseWhitespace(const string o_data, size_t* o_offset) {
  while( isWhitespace(o_data[*o_offset]) ) { (*o_offset)++; }
}

/* This function tests pattern: [ \t\r\n]+ o_operator [ \t\r\n]+
 * returns if match returns TRUE otherwise returns FALSE
**/
static bool parseOperator(const string o_data, size_t* o_offset, char o_operator) {
  size_t i = (*o_offset); // offset
  parseWhitespace(o_data, &i);

  if(o_data[i] != o_operator) return FALSE;
  i++;

  parseWhitespace(o_data, &i);

  (*o_offset) = i;
  return TRUE;
}

/* This function parses integer value
**/
static long parseInteger(const string o_data, size_t* o_offset) {
  long   _val =  0;
  int    _dig = -1;
  size_t i    = (*o_offset); // offset
  int    _sig =  1;
  int    _mul = 10;
  
  // sign
  if(o_data[i] == '-') {
    _sig = -1;
    i++;
  }
  
  // number can be hex, decimal or octan
  if(o_data[i] == '0') {
    i++;

    if(o_data[i] == 'x' || o_data[i] == 'X') {      // hexadecimal
      _mul = 16;
      i++;
    }
    else if(o_data[i] <= '7' && o_data[i] >= '0') { // octan
      _mul = 8;
    }
    else assertnz(o_data[i] == '\0');
  }
  
  while(o_data[i] != '\0') {
    _dig = -1;

    if(o_data[i] <= '9' && o_data[i] >= '0') _dig = o_data[i] - '0'; else
    if(o_data[i] <= 'a' && o_data[i] >= 'f') _dig = o_data[i] - 'a'; else
    if(o_data[i] <= 'A' && o_data[i] >= 'F') _dig = o_data[i] - 'A'; else break;
    
    assertnz(_dig < _mul);

    _val = _val * _mul + _dig;
    
    i++;
  }

  (*o_offset) = i;
  
  return _val * _sig;
}

/**
 * mul {
 *   from: [0-9]+
 *   to: [0-9]+
 *   length: [0-9]+
 *   values: [
 *     [0-9]+ (, [0-9]+)*
 *   ]
 * }
 */

/* This function converts message string into message structure
 * Caller must free the result
**/
Msg msg_load(const string o_data) {

  string _data = toLower(o_data);
  Msg    _out  = malloc( sizeof(struct st_msg) );
  assertnz(_out);
  size_t _cur  = 0              ;
  string _id   = NULL           ;
  size_t i = 0; // offset

  _out->length = 0;

  // match mul head
  parseWhitespace(o_data, &i);
  _id = parseID(o_data, &i);
  assertz( _id == NULL || strcmp(_id, "mul") );
  
  assertnz( parseOperator(o_data, &i, ':') );
  assertnz( parseOperator(o_data, &i, '{') );
  
  // match mul body
  while(o_data[i] != '\0') {
  
    if(_id != NULL) {
      free(_id);
      _id = NULL;
    }
    
    _id = parseID(o_data, &i);
    if(_id == NULL) break;

    // read the number of values
    if( !strcmp(_id, "length") ) {
      assertnz( parseOperator(o_data, &i, ':') );
      parseWhitespace(o_data, &i);
      _out->length = parseInteger(o_data, &i);
      if(_out->length) {
        _out->values = malloc(sizeof(int) * _out->length);
        assertnz(_out->values);
      }
    }
    
    // read from message pid
    else if( !strcmp(_id, "from") ) {
      assertnz(parseOperator(o_data, &i, ':') );
      parseWhitespace(o_data, &i);
      
      _out->from = parseInteger(o_data, &i);
    }
    
    // read to message id
    else if( !strcmp(_id, "to") ) {
      assertnz( parseOperator(o_data, &i, ':') );
      parseWhitespace(o_data, &i);
      _out->to = parseInteger(o_data, &i);
    }
    
    // read values
    else if( !strcmp(_id, "values") ) {
      assertnz( parseOperator(o_data, &i, ':') );
      if( parseOperator(o_data, &i, '[') ) {
        parseWhitespace(o_data, &i);
        
        // read one value
        while(_cur < _out->length) {
          assertnz(_out->values);
          assertnz(_cur < _out->length);
          _out->values[_cur++] = cast(int)parseInteger(o_data, &i);
          
          if(_cur < _out->length) assertnz( parseOperator(o_data, &i, ',') );
        }
        assertnz(_cur == _out->length);
        parseWhitespace(o_data, &i);
        assertnz( parseOperator(o_data, &i, ']') );
      }
      else {
        parseWhitespace(o_data, &i);
        _out->values[0] = cast(int)parseInteger(o_data, &i);
        assertnz(_out->length == 1);
      }
    }

    if( !parseOperator(o_data, &i, '}') ) {
      assertnz( parseOperator(o_data, &i, ',') );
    }
    else break;  
  }
  
  if(_id != NULL) free(_id);
  free(_data);
  
  return _out;
}

/* This function converts message structure into message string 
**/
string msg_save(const Msg o_data, string o_buffer, size_t o_size) {
  size_t _len = 0   ;
  
  snprintf (
    o_buffer,
    o_size,
    "mul: {\n"
    "  from  : %ld,\n"
    "  to    : %ld,\n"
    "  length: %ld,\n"
    "  values: ",
    cast(long)o_data->from,
    cast(long)o_data->to,
    cast(long)o_data->length
  );
  
  _len = strlen(o_buffer);
  
  if(o_data->length == 1) {
    snprintf(o_buffer + _len, o_size - _len, "%d\n", o_data->values[0]);
    _len += strlen(o_buffer + _len);
  }
  else {
    snprintf(o_buffer + _len, o_size - _len, "[\n");
    _len += strlen(o_buffer + _len);
    
    for(size_t i = 0; i < o_data->length; i++) {
      if(i == o_data->length - 1)
        snprintf(o_buffer + _len, o_size - _len, "    %d\n",  o_data->values[i])
      ;
      else
        snprintf(o_buffer + _len, o_size - _len, "    %d,\n", o_data->values[i])
      ;
      _len += strlen(o_buffer + _len);
    }
    
    snprintf(o_buffer + _len, o_size - _len, "  ]\n");
    _len += strlen(o_buffer + _len);
  }
  
  snprintf(o_buffer + _len, o_size - _len, "}\n");
  
  return o_buffer;
}

/* This function is Msg destructor / deallocator
**/
Msg msg_destroy(Msg o_msg) {
  free(o_msg->values);
  free(o_msg);
  return NULL;
}
