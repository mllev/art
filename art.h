/* A highly compressed radix tree
 *  Based on the Adaptive Radix Tree by 
 *    Viktor Leis, Alfons Kemper and Thomas Neumann
 *
 * Copyright (c) 2015, Matthew Levenstein
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. Neither the name of Redis nor the names of its contributors may be used
 *      to endorse or promote products derived from this software without
 *      specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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
