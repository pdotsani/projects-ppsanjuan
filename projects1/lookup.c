#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>

#include "lookup.h"

void configure_keyword(struct keyword *key, char *argv[]) {
  key->keyword = argv[1];
  key->dlim = argv[2];
  key->keywordIdx = 0; 
  key->dlimIdx = 0; 
  key->keywordLength = strlen(argv[1]);
  key->dlimLength = strlen(argv[2]);
  key->foundKeyword = false;
  key->foundDlim = false;
  key->inDlim = false;
  key->idx = 0;
  key->startCapture = false;
}

void lookup_key(struct keyword *key, char *c) {
  if (*(key->keyword + key->keywordIdx) == *c && key->foundKeyword == false) {
    key->keywordIdx++;
    if (key->keywordIdx == key->keywordLength) {
      key->keywordIdx = 0;
      key->foundKeyword = true;
    }
  } else {
    key->keywordIdx = 0;
  }
}

void lookup_dlim(struct keyword *key, char *c) {
  if (key->foundKeyword == true && key->foundDlim == false) {
    if(*(key->dlim + key->dlimIdx) == *c) {
      key->inDlim = true;
      key->dlimIdx++;
      if(key->dlimIdx == key->dlimLength) {
        key->dlimIdx = 0;
        key->foundDlim = true;
        key->inDlim = false;
      }
    } else {
      key->dlimIdx = 0;
      key->inDlim = false;
    }
  }
};

bool found_value(struct keyword *key, char *c, char *buf) {
  if (key->foundDlim == true && key->foundKeyword == true && key->startCapture == false) {
    if (*c != ' ' && *(key->dlim + key->dlimLength - 1) != *c) {
      key->startCapture = true;
    }
  }

  if (key->foundDlim == true && key->foundKeyword == true && key->startCapture == true) {
    if (*c != '\n') {
      *(key->captured + key->idx) = *c;
      key->idx++;
    } else {
      key->startCapture = false;
      key->foundDlim = false;
      key->foundKeyword = false;
      *(key->captured + key->idx) = '\n';
      strcpy(buf, key->captured);
      return true;
    }
  }

  return false;
}

int lookup(struct keyword *key, int *fd, char *buf) {
  char c;
  char *line = malloc(1000);
  int lineIdx = 0;
  ssize_t read_sz;
  while((read_sz = read(*fd, &c, 1)) > 0) {
    *(line + lineIdx) = c;
    lineIdx++;
      
    lookup_key(key, &c);
    lookup_dlim(key, &c);

    // If we found the keyword and the delimeter, we print out the desired value only.
    if (found_value(key, &c, buf)) {
      return 0;
    }

    // // If we found the keyword, but not the delimeter before we reached the end of the line,
    // // we print out the line.
    // if (key->foundDlim == false && key->foundKeyword == true && c == '\n') {
    //   strcpy(buf, line);
    //   return 0;
    // }
    
    // // Resets the line string every time we hit a newline
    if (c == '\n') {
      free(line);
      lineIdx = 0;
      line = calloc(1000, sizeof(char));
    }
  }

  if (read_sz == -1) {
    perror("read");
    return 1;
  }

  return 0;
}

// int main(int argc, char *argv[]) {
//   struct keyword key;
//   configure_keyword(&key, argv);
  
//   int fd = open(argv[3], O_RDONLY);
//   if (fd == -1) {
//     perror("open");
//     return 1;
//   }

//   char *buf = malloc(1000);

//   lookup(&key, &fd, buf);

//   if (buf[0] != '\0') {
//     printf("%s", buf);
//   }

//   free(buf);

//   return 0;
// }