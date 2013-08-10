//
//  print.h
//  trabCGDraw3d
//
//  Created by Carolina Zamith Cunha on 8/10/13.
//  Copyright (c) 2013 Carolina Zamith Cunha. All rights reserved.
//

/*  Printing convenience (print.c) */
void printv(va_list args, const char* format);
void print(const char* format, ...);
void printAt(int x, int y, const char* format, ...);
void errCheck(char* where);
