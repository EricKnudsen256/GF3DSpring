#ifndef __GFC_TEXT__H__
#define __GFC_TEXT__H__
/**
 * gfc_text
 * @license The MIT License (MIT)
   @copyright Copyright (c) 2019 EngineerOfLies
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

/**
 * @purpose the MGL text library provides common string support for fixed length
 *  strings.  For use with unlimited strings mgl will use glib's GString type
 */
#include <string.h>
#include "gfc_types.h"
#include "gfc_color.h"
/**
 * constant lenth character buffers.
 * These will be used when the convenience of GString is not needed
 */

/**
 * @brief 16 character buffer used for short tags
 */
typedef char TextWord[16];
#define GFCWORDLEN   16
#define gfc_word_cmp(a,b) (strncmp(a,b,GFCWORDLEN))
#define gfc_word_cpy(dst,src) (strncpy(dst,src,GFCWORDLEN))
#define gfc_word_clear(a)  (memset(a,0,sizeof(char)*GFCWORDLEN))
/**
 * @brief 128 character buffer used for statements
 */
typedef char TextLine[128];
#define GFCLINELEN   128
#define gfc_line_cmp(a,b) (strncmp(a,b,GFCLINELEN))
#define gfc_line_cpy(dst,src) (strncpy(dst,src,GFCLINELEN))
#define gfc_line_clear(a)  (memset(a,0,sizeof(char)*GFCLINELEN))
/**
 * @brief 512 character buffer used for text blocks
 */
typedef char TextBlock[512];
#define GFCTEXTLEN   512
#define gfc_block_cmp(a,b) (strncmp(a,b,GFCTEXTLEN))
#define gfc_block_cpy(dst,src) (strncpy(dst,src,GFCTEXTLEN))
#define gfc_block_clear(a)  (memset(a,0,sizeof(char)*GFCTEXTLEN))


#endif
