#include <stdbool.h>

#ifndef LOOKUP_H
#define LOOKUP_H

// The keyword struct contains all the values you need to run the
// lookup function. All attributes must be set before you can call
// lookup.
struct keyword {
  // keyword tracking
  char *keyword;
  int keywordIdx;
  int keywordLength;
  bool foundKeyword;
  // dlim tracking
  char *dlim;
  int dlimIdx;
  int dlimLength;
  bool foundDlim;
  bool inDlim;
  // capturing value tracking
  char captured[1025];
  bool startCapture;
  int idx;
};

void configure_keyword(struct keyword *key, char *argv[]);

void lookup_key(struct keyword *key, char *c);

void lookup_dlim(struct keyword *key, char *c);

bool found_value(struct keyword *key, char *c, char *buf);

// The main function you would use to lookup a keyword and it's values
// pass in they keyword struct detailed in this header, along with the file and
// a buffer to store the output
int lookup(struct keyword *key, int *fd, char *buf);

#endif