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

#ifndef OVERLAY_H
#define OVERLAY_H

#include "Common.h"
#include "Ui.h"

class Canvas;
class GameControl;


class Overlay : public Layer
{
public:
};

class ListProvider
{
public:
  virtual int     countItems() =0;
  virtual Canvas* provideItem( int i, Canvas* old ) =0;
  virtual void    releaseItem( Canvas* old ) =0;
  virtual void    onSelection( int i ) {};
  virtual void    onSelection( int i, int ix, int iy ) { onSelection(i); };
};

extern Overlay* createIconOverlay( GameControl& game, const char* file,
				   int x=100,int y=20,
				   bool dragging_allowed=true);

extern Overlay* createEditOverlay( GameControl& game );

extern Overlay* createNextLevelOverlay( GameControl& game );

extern Overlay* createListOverlay( GameControl& game,
				   ListProvider* provider,
				   int x=100, int y=20,
				   int w=200, int h=200 );

extern Overlay* createMenuOverlay( GameControl& game );

#endif //OVERLAY_H
