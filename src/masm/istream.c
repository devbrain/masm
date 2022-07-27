//
// Created by igor on 25/07/2022.
//

#include <stdlib.h>
#include <string.h>
#include "istream.h"

/*
 * FILE Stream
 */

struct file_stream {
  FILE* f;
};

static void file_stream_destructor(void* f) {
  free (f);
}

static int file_stream_eof(void* f) {
  struct file_stream* obj = (struct file_stream*)f;
  return feof (obj->f);
}

static ssize_t file_stream_read(void* f, void* dst, size_t size) {
  struct file_stream* obj = (struct file_stream*)f;
  return fread (dst, 1, size, obj->f);
}

/*
 * String stream
 */

struct string_stream {
  char* s;
  size_t len;
  size_t rd_ptr;
};

static void string_stream_destructor(void* f) {
  struct string_stream* obj = (struct string_stream*)f;
  free(obj->s);
  free(f);
}

static int string_stream_eof(void* f) {
  struct string_stream* obj = (struct string_stream*)f;
  if (obj->rd_ptr >= obj->len) {
    return 1;
  }
  return 0;
}

static ssize_t string_stream_read(void* f, void* dst, size_t size) {
  struct string_stream* obj = (struct string_stream*)f;
  size_t has_bytes = 0;
  size_t to_read = 0;
  if (obj->rd_ptr >= obj->len) {
    return -1;
  }
  has_bytes = obj->len - obj->rd_ptr;
  to_read = has_bytes < size ? has_bytes : size;
  memcpy (dst, obj->s + obj->rd_ptr, to_read);
  obj->rd_ptr += to_read;
  return to_read;
}

struct istream* istream_create_from_file(FILE* input) {
  struct istream* is = malloc (sizeof(struct istream));
  is->opaque = malloc (sizeof (struct file_stream));
  ((struct file_stream*)is->opaque)->f = input;
  is->destructor = file_stream_destructor;
  is->is_eof = file_stream_eof;
  is->read = file_stream_read;
}

struct istream* istream_create_from_string(const char* input) {
  struct string_stream* obj = NULL;
  struct istream* is = malloc (sizeof(struct istream));
  is->opaque = malloc (sizeof (struct string_stream));
  obj = ((struct string_stream*)is->opaque);
  obj->s = strdup(input);
  obj->len = strlen (input);
  obj->rd_ptr = 0;
  is->destructor = string_stream_destructor;
  is->is_eof = string_stream_eof;
  is->read = string_stream_read;
}

void istream_close(struct istream* victim) {
  victim->destructor(victim->opaque);
  free(victim);
}

int istream_eof(struct istream* is) {
  return is->is_eof(is->opaque);
}

ssize_t istream_read(struct istream* is, void* dest, size_t size) {
  return is->read(is->opaque, dest, size);
}