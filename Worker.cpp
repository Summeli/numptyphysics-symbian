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

#include "Worker.h"
#include "Event.h"
#include <stdio.h>

WorkerBase::WorkerBase( int (*func)(void*) ) 
  : m_func(func),
    m_thread(NULL)
{
}

WorkerBase::~WorkerBase()
{
  if (m_thread) {
    //TODO need cleaner termination/forget - worker may leak resources
    SDL_KillThread(m_thread);
  }
}

void WorkerBase::start()
{
  if (m_func) {
    m_thread = SDL_CreateThread(m_func, this);
  }
}


void WorkerBase::wait()
{
  if (m_thread) {
    SDL_WaitThread(m_thread,NULL);
  }
}

bool WorkerBase::done()
{
  return m_thread == NULL;
}

int WorkerBase::startThread(void* wbase)
{
  ((WorkerBase*)wbase)->main();
  ((WorkerBase*)wbase)->m_thread = NULL;

  // Create a user event to signal completion
  SDL_Event event;
  event.type = SDL_USEREVENT;
  event.user.code = WORKER_DONE;
  event.user.data1 = wbase;
  event.user.data2 = 0;
  SDL_PushEvent(&event);
}
