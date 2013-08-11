//
//  print.h
//  cgTrab2
//
//  Created by Marcelle Guine on 11/08/13.
//  Copyright (c) 2013 Marcelle Guine. All rights reserved.
//

/*  Printing convenience (print.c) */
//void printv(va_list args, const char* format);
void print(const char* format, ...);
void printAt(int x, int y, const char* format, ...);
void errCheck(char* where);

