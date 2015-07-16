/* A highly compressed radix tree
 *  Based on the Adaptive Radix Tree by 
 *    Viktor Leis, Alfons Kemper and Thomas Neumann
 *  
 * Author  - Matthew Levenstein
 * License - MIT
 */

#ifndef _ART_H
#define _ART_H

typedef unsigned char byte_t;
typedef unsigned long word_t;

#define _LEAF     0
#define _SINGLE   1
#define _LINEAR   4
#define _LINEAR16 16
#define _SPAN     48
#define _RADIX    255

typedef struct {
  byte_t type; 
  byte_t plen; 
  byte_t path[sizeof(word_t)];
  short  rcnt;
} artNodeHeader;

typedef struct {
  artNodeHeader head;
} artNode;

typedef struct {
  artNode* root;
} Art;

typedef struct {
  artNodeHeader  head;
  word_t         val;
} artNodeLeaf;

typedef struct {
  artNodeHeader  head;
  byte_t         map;
  word_t         radix;
  word_t         val;
} artNodeSingle;

typedef struct {
  artNodeHeader  head;
  byte_t         map[_LINEAR];
  word_t         radix[_LINEAR];
  word_t         val;
} artNodeLinear;

typedef struct {
  artNodeHeader  head;
  byte_t         map[_LINEAR16];
  word_t         radix[_LINEAR16];
  word_t         val;
} artNodeLinear16;

typedef struct {
  artNodeHeader  head;
  byte_t         map[256];
  word_t         radix[_SPAN];
  word_t         val;
} artNodeSpan;

typedef struct {
  artNodeHeader  head;
  word_t         val;
  word_t         radix[256];
} artNodeRadix;

/* API */
void      artPut                   (Art*, byte_t*, int, word_t);
word_t    artGet                   (Art*, byte_t*, int);
int       artRemove                (Art*, byte_t*, int);
Art*      artNew                   (void);

/* for tests */
void artNodePrintDetails (artNode*);
void artNodePrintPrefix  (artNode*);

#endif
