/*
 * This file is part of NumptyPhysics
 * Copyright (C) 2008 Tim Edmonds
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#ifndef WORKER_H
#define WORKER_H

#ifdef __SYMBIAN32__
#include <SDL.h>
#include <SDL_thread.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#endif
class WorkerBase
{
 public:
  WorkerBase( int (*func)(void*)=startThread );  
  virtual ~WorkerBase();
  void start();
  virtual void main() =0;
  void wait();
  bool done();

 private:
  static int  startThread(void* wbase);
  int        (*m_func)(void*);
  SDL_Thread  *m_thread;
};

typedef WorkerBase Worker;

template< typename A, typename B, typename C >
class WorkerF : public WorkerBase
{
 public:
  typedef void (*F)(A,B,C);

  WorkerF( F ff, A aa, B bb, C cc )
    : f(ff), a(aa), b(bb), c(cc)
    {}
  
  virtual void main() 
  {
    f( a, b, c );
  }

 private:
  F f; A a; B b; C c;
};




#endif //WORKER_H
