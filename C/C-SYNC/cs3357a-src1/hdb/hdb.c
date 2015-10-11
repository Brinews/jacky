#include "hdb.h"
#include <stdlib.h>
#include <string.h>

#include <hiredis/hiredis.h>

hdb_connection* hdb_connect(const char* server) {
  // Connect to the Redis server.  
  // See https://github.com/redis/hiredis/blob/master/examples/example.c
  //
  // Cast the redisContext to an hdb_connection.
  // See the definition of hdb_connection in hdb.h -- notice that it's
  // just a typedef of void* (i.e. an alias of void*).
  //
  // Why are we doing this?  To hide our implementation details from
  // any users of the library.  We want to be able to change our 
  // implementation at any time without affecting external code.
  // We don't want external users of the code to know we're using
  // Redis, so that, if we decided to switch to another data store
  // in the future, we could make the change internally, and no
  // external code would break.
  //
  // To avoid a compiler warning when casting the redisContext to
  // an hdb_connection, you may find the following line helpful
  // (don't be scared):
  // return *(hdb_connection**)&context;

  struct timeval timeout = {1, 500000 };
  redisContext *context = redisConnectWithTimeout((char *)"127.0.0.1",
         6379, timeout);

#ifdef AUTHNEED
  redisReply *reply;
  reply = redisCommand(context, "AUTH duocisthemostsunnysite");
  freeReplyObject(reply);
#endif

  if (context->err) {
      fprintf(stderr, "connection error: %s\n", context->errstr);
      return NULL;
  }

  return *(hdb_connection**)&context;
}

void hdb_disconnect(hdb_connection* con) {
  // "Disconnect" from the Redis server (i.e. free the Redis context)
  // See https://github.com/redis/hiredis/blob/master/examples/example.c
  
  redisReply *reply;

  redisContext *c = (redisContext *) con;

  reply = redisCommand(c, "FLUSHDB");
  freeReplyObject(reply);

  redisFree(c);
}

void hdb_store_file(hdb_connection* con, hdb_record* record) {
  // Store the specified record in the Redis server.  There are many ways to
  // do this with Redis.  Whichever way you choose, the checksum should be 
  // associated with the file, and the file should be associated with the user.
  //
  // Hint: look up the HSET command. 
  //
  // See https://github.com/redis/hiredis/blob/master/examples/example.c for
  // an example of how to execute it on the Redis server.
  redisContext *c = (redisContext *) con;
  redisReply *reply;

  if (record != NULL) {
      reply = redisCommand(c, "HSET %s %s %s", record->username, 
              record->filename, record->checksum);

      freeReplyObject(reply);
  }
}

int hdb_remove_file(hdb_connection* con, const char* username, const char* filename) {
  // Remove the specified file record from the Redis server.

  redisContext *c = (redisContext *) con;
  redisReply *reply;
  int ret;

  reply = redisCommand(c, "HDEL %s %s", username, filename);
  ret = reply->integer;

  freeReplyObject(reply);

  return ret; // Remove me
}

char* hdb_file_checksum(hdb_connection* con, const char* username, const char* filename) {
  // If the specified file exists in the Redis server, return its checksum.
  // Otherwise, return NULL.
  char *ret = NULL;

  redisContext *c = (redisContext *) con;
  redisReply *reply;

  reply = redisCommand(c, "HGET %s %s", username, filename);

  if (reply->type != REDIS_REPLY_NIL) {
      ret = strdup(reply->str);
  }

  freeReplyObject(reply);

  return ret; //need free if not NULL
}

int hdb_file_count(hdb_connection* con, const char* username) {
  // Return a count of the user's files stored in the Redis server.
  int ret = 0;

  redisContext *c = (redisContext *) con;
  redisReply *reply;

  reply = redisCommand(c, "HLEN %s", username);

  ret = reply->integer;

  freeReplyObject(reply);

  return ret; // Remove me
}

bool hdb_user_exists(hdb_connection* con, const char* username) {
  // Return a Boolean value indicating whether or not the user exists in
  // the Redis server (i.e. whether or not he/she has files stored).

  bool ret = false;

  redisContext *c = (redisContext *) con;
  redisReply *reply;

  reply = redisCommand(c, "EXISTS %s", username);
  if (reply->integer > 0) ret = true;

  freeReplyObject(reply);

  return ret; 
}

bool hdb_file_exists(hdb_connection* con, const char* username, const char* filename) {
  // Return a Boolean value indicating whether or not the file exists in
  // the Redis server.

  bool ret = false;

  redisContext *c = (redisContext *) con;
  redisReply *reply;

  reply = redisCommand(c, "HEXISTS %s %s", username, filename);
  if (reply->integer > 0) ret = true;

  freeReplyObject(reply);

  return ret; 
}

hdb_record* hdb_user_files(hdb_connection* con, const char* username) {
  // Return a linked list of all the user's file records from the Redis
  // server.  See the hdb_record struct in hdb.h  -- notice that it 
  // already has a pointer 'next', allowing you to set up a linked list
  // quite easily.
  //
  // If the user has no files stored in the server, return NULL.
  hdb_record *ret_list = NULL;
  
  redisContext *c = (redisContext *) con;
  redisReply *reply;

  if (hdb_user_exists(con, username)) {
      reply = redisCommand(c, "HKEYS %s", username);

      for (int i = 0; i < reply->elements; i++) {
          hdb_record* rec = (hdb_record *) malloc(sizeof(hdb_record));
          rec->username = strdup(username);
          rec->filename = strdup(reply->element[i]->str);
          rec->checksum = hdb_file_checksum(con, username, rec->filename);
          rec->next = ret_list;

          ret_list = rec;
      }

      freeReplyObject(reply);
  }

  return ret_list;
}

void hdb_free_result(hdb_record* record) {
  // Free up the memory in a linked list allocated by hdb_user_files().
  hdb_record *ptr = record, *next;

  while (ptr != NULL) {
      next = ptr->next;

      free(ptr->username);
      free(ptr->filename);
      free(ptr->checksum);

      free(ptr);

      ptr = next;
  }
}

int hdb_delete_user(hdb_connection* con, const char* username) {
  // Delete the user and all of his/her file records from the Redis server.
  int ret = 0;

  redisContext *c = (redisContext *) con;
  redisReply *reply;

  reply = redisCommand(c, "DEL %s", username);
  ret = reply->integer;

  freeReplyObject(reply);

  return ret;
}
