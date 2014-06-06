/*
* ASCIIWar
* Copyright (c) 2014 Frédéric Brachfogel
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of Frédéric Brachfogel may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY FREDERIC BRACHFOGEL ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL FREDERIC BRACHFOGEL BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "asciiwar.h"

/*
 *
 */

void BTN_UpdateEntry(AW_game_instance_t *gi, AW_btn_t *b) {
  for(int f = 0; f < gi->key_counter; f++)
    if(!gi->keys[f].pressed)
      if(gi->keys[f].vk == TCODK_CHAR)
        b->s += gi->keys[f].c;
      else
      if(gi->keys[f].vk == TCODK_SPACE)
        b->s += ' ';
      else
      if(gi->keys[f].vk == TCODK_BACKSPACE) 
        if(b->s.length() > 0)
          b->s.erase(b->s.length()-1);
}

void BTN_Render(AW_game_instance_t *gi, AW_btn_t *b) {
  TCOD_console_set_default_foreground(con, b->color);
  AW_PrintString(b->s, b->pos_x, b->pos_y);
  if(b->highlight) {
    if(BTN_IsMouseOver(gi, b)
    || BTN_GonnaBeClicked(gi, b)) {
      TCOD_color_t c      = BTN_GonnaBeClicked(gi, b) ? TCOD_light_blue : TCOD_blue;
      int idx[]           = { 0, (b->s.length()+BTN_GLARE_SIZE)>>1, (b->s.length()+BTN_GLARE_SIZE)-1 };
      TCOD_color_t cols[] = {{ 0,0,0 },{c.r,c.g,c.b},{0,0,0}};
      TCOD_color_t map[CON_RES_X];
      TCOD_color_gen_map(map, 3, cols, idx);
      for(int i = b->pos_x-BTN_GLARE_SIZE/2, f = 0; i < b->pos_x+b->s.length()+BTN_GLARE_SIZE/2; i++, f++)
        if(INSIDE_FULL_CON(i, b->pos_y))
          TCOD_console_set_char_background(con, i, b->pos_y, map[f], TCOD_BKGND_SET);
    }
  } else
    for(int i = b->pos_x-BTN_GLARE_SIZE/2; i < b->pos_x+b->s.length()+BTN_GLARE_SIZE/2; i++)
      if(INSIDE_FULL_CON(i, b->pos_y))
        TCOD_console_set_char_background(con, i, b->pos_y, TCOD_black, TCOD_BKGND_SET);
  if(b->entry
  && INSIDE_FULL_CON(b->pos_x+b->s.length(), b->pos_y))
    if((gi->frame_mark&31) < 16)
      TCOD_console_set_char_background(con, b->pos_x+b->s.length(), b->pos_y, TCOD_black, TCOD_BKGND_SET);
    else 
      TCOD_console_set_char_background(con, b->pos_x+b->s.length(), b->pos_y, TCOD_white, TCOD_BKGND_SET);
}

void BTN_Render(AW_game_instance_t *gi, AW_btn_list_t *l, bool center_x, short step) {
  DO_TIMES(l->btn_count) {
    AW_btn_t *b = &l->btns[f];
    b->pos_y = l->pos_y + f*step;
    if(center_x)
      BTN_CenterX(gi, b, l->pos_x);
    else
      b->pos_x = l->pos_x;
    BTN_Render(gi, b);
  }
}

bool BTN_IsMouseOver(AW_game_instance_t *gi, AW_btn_t *b) {
  return gi->mouse.cx >= b->pos_x 
      && gi->mouse.cx <= b->pos_x + b->s.length()
      && gi->mouse.cy == b->pos_y;
}

bool BTN_IsClicked(AW_game_instance_t *gi, AW_btn_t *b) {
  return (BTN_IsMouseOver(gi, b) && gi->lbtn_up) 
      || GI_IsKeyReleased(gi, TCODK_CHAR, b->shortcut)
      || GI_IsKeyReleased(gi, b->shortcut_special);
}

bool BTN_GonnaBeClicked(AW_game_instance_t *gi, AW_btn_t *b) {
  return (BTN_IsMouseOver(gi, b) && gi->lbtn_down) 
      || GI_IsKeyPressed(gi, TCODK_CHAR, b->shortcut)
      || GI_IsKeyPressed(gi, b->shortcut_special);
}

void BTN_CenterX(AW_game_instance_t *gi, AW_btn_t *b, short axe) {
  if(axe == -1) axe = (CON_RES_X/2);
  b->pos_x = axe - ceil((float)b->s.length()/2);
}

void BTN_CenterY(AW_game_instance_t *gi, AW_btn_t *b) {
  b->pos_y = CON_RES_Y>>1;
}

void BTN_CenterY(AW_game_instance_t *gi, AW_btn_list_t *l) {
  l->pos_y = CON_RES_Y/2 - l->btn_count;
}

/*
 *
 */

void UO_Init(AW_game_instance_t *gi, int argc, char** argv) {
  for(int i = 0; i < MAX_UNIT_ORDER; i++) {
    order(i).order = AW_unit_order_none;
    if(i != MAX_UNIT_ORDER-1)
      order(i).fnext = i+1;
    else
      order(i).fnext = AW_null;
    order(i).next     = AW_null;
  }
  gi->free_unit_order_head = 0;
  trace("Unit orders initiated.");
}

AW_unit_order_ptr UO_New(AW_game_instance_t *gi) {
  if(gi->free_unit_order_head != AW_null) {
    AW_unit_order_ptr o         = gi->free_unit_order_head;
    AW_unit_order_t   *uo       = &order(o);
    gi->free_unit_order_head    = uo->fnext;
    uo->order                   = AW_unit_order_none;
    uo->started                 = false;
    uo->starting                = false;
    uo->started_proxy           = false;
    uo->done                    = false;
    uo->attack_here             = false;
    uo->precise_target          = true;
    uo->with_counter            = false;
    uo->following_friend        = false;
    uo->time_stamp              = -1;
    uo->last_target_cx          = -1;
    uo->counter                 = 0;
    uo->task_id                 = -1;
    uo->target                  = AW_null;
    uo->next                    = AW_null;
    uo->user_data               = AW_null;
    uo->unit_order_completed_cb = null;
    uo->unit_order_failed_cb    = null;
    return o;
  }
  trace("No more unit order left.");
  return AW_null;
}

AW_unit_order_ptr UO_PushFront(AW_game_instance_t *gi, AW_unit_ptr u) {
  AW_unit_order_ptr o = UO_New(gi);
  if(o != AW_null) {
    AW_unit_order_t   *uo     = &order(o);
    uo->next                  = unit(u).order_head;
    unit(u).order_head        = o;
  }
  return o;
}

AW_unit_order_ptr UO_PushBack(AW_game_instance_t *gi, AW_unit_ptr u) {
  AW_unit_order_ptr o = UO_New(gi);
  if(o != AW_null) {
    if(unit(u).order_head == AW_null)
      unit(u).order_head = o;
    else {
      AW_unit_order_ptr last = unit(u).order_head;
      while(order(last).next != AW_null)
        last = order(last).next;
      order(last).next = o;
    }
  }
  return o;
}

void UO_FreeFront(AW_game_instance_t *gi, AW_unit_ptr u, bool complete) {
  AW_unit_t *un             = &unit(u);
  AW_unit_order_ptr o       = un->order_head;
  AW_unit_order_t *uo       = &order(o);
  if(o != AW_null) {
    un->order_head             = uo->next;
    if(complete) {
      if(uo->unit_order_completed_cb)
        uo->unit_order_completed_cb(gi, u, o);
    } else {
      if(uo->unit_order_failed_cb)
        uo->unit_order_failed_cb(gi, u, o);
    }
    uo->task_id                = gi->task_mark++;
    uo->fnext                  = gi->free_unit_order_head;
    gi->free_unit_order_head   = o;
  }
}

void UO_FreeAll(AW_game_instance_t *gi, AW_unit_ptr u) {
  while(unit(u).order_head != AW_null)
    UO_FreeFront(gi, u);
}

AW_unit_order_type_t UO_GetFrontType(AW_game_instance_t *gi, AW_unit_ptr u) {
  if(unit(u).order_head == AW_null)
    return AW_unit_order_none;
  return order(unit(u).order_head).order;
}

AW_unit_order_t* UO_GetFront(AW_game_instance_t *gi, AW_unit_ptr u) {
  if(unit(u).order_head == AW_null)
    return null;
  return &order(unit(u).order_head); 
}

bool UO_IsTarget(AW_game_instance_t *gi, AW_unit_ptr u, AW_unit_ptr target, AW_unit_order_ptr o) {
  if(o == AW_null)
    o = unit(u).order_head;
  if(o != AW_null) {
    AW_unit_order_t *uo = &order(o);
    if(uo->order == AW_unit_order_follow_target
    && uo->target == target)
      return true;
    if(uo->next != AW_null)
      return UO_IsTarget(gi, u, target, uo->next);
  }
  return false;
}

/*
 * balls
 */

void BA_Init(AW_game_instance_t *gi, int argc, char** argv) {
  // clear ball map.
  for(int i = 0; i < MAP_SIZE; i++)
    gi->ball_map[i] = AW_null;
  // create balls.
  for(int i = 0; i < MAX_BALL; i++) {
    if(i != MAX_BALL-1)
      ball(i).fnext = i+1;
    else
      ball(i).fnext = AW_null;
  }
  gi->free_ball_head = 0;
  gi->ball_head      = AW_null;
  trace("Balls initiated.");
}

AW_ball_ptr BA_New(AW_game_instance_t *gi) {
  if(gi->free_ball_head != AW_null) {
    AW_ball_ptr r            = gi->free_ball_head;
    gi->free_ball_head       = ball(gi->free_ball_head).fnext;
    ball(r).previous         = AW_null;
    ball(r).next             = gi->ball_head;
    if(gi->ball_head != AW_null)
      ball(gi->ball_head).previous = r;
    gi->ball_head = r;
    return r;
  }
  trace("No more ball left.");
  return AW_null;
}

void BA_Free(AW_game_instance_t *gi, AW_ball_ptr b) {
  if(gi->ball_head == b)
    gi->ball_head = ball(b).next;
  if(ball(b).previous != AW_null)
    ball(ball(b).previous).next = ball(b).next;
  if(ball(b).next != AW_null)
    ball(ball(b).next).previous = ball(b).previous;
  ball(b).fnext = gi->free_ball_head;
  gi->free_ball_head = b;
}

void BA_FreeAll(AW_game_instance_t *gi) {
  while(gi->ball_head != AW_null)
    BA_Free(gi, gi->ball_head);
  trace("Balls freed.");
}

bool BA_Update(AW_game_instance_t *gi, AW_ball_ptr b) {
  /* the ball has been sent only if the target has
     virtually enough hp so we know it's alive */
  AW_ball_t *ba = &ball(b);
  ba->move += gi->game_time_step * BALL_SPEED;
  if(ba->move >= 1000) {
    ba->move -= 1000;
    AW_player_ptr target_p  = GI_GetPlayerPtr(gi, ba->target_player_id);
    AW_unit_t *target_un = &unit(ba->target);
    int x = ba->pos_x,
        y = ba->pos_y;
    TCOD_line_init(x, y, target_un->pos_x, target_un->pos_y);
    TCOD_line_step(&x, &y);
    ba->pos_x = x;
    ba->pos_y = y;
    SOCLE(SIZE(target_un))
      if(ba->pos_x == target_un->pos_x+i
      && ba->pos_y == target_un->pos_y+j) {
        assert(target_un->hp > 0);
        target_un->hp        -= ba->damage;
        target_un->blink_acc = 0;//1000;
        if(target_un->hp <= 0) {
          assert(target_un->hp == 0);
          PL_KillUnit(gi, ba->p, target_p, ba->target);
          AW_blood_ptr o = BLOOD_New(gi);
          if(o != AW_null) {
            blood(o).pos_x = target_un->pos_x+(rand()%3)-1;
            blood(o).pos_y = target_un->pos_y+(rand()%3)-1;
          }
        }
        AW_smoke_ptr s = SMOKE_New(gi);
        if(s != AW_null) {
          smoke(s).pos_x = ba->pos_x;
          smoke(s).pos_y = ba->pos_y;
        }
        return true; /* free the ball */
      }
  }
  return false; /* don't free the ball */
}

/*
 * smokes
 */

void SMOKE_Init(AW_game_instance_t *gi, int argc, char** argv) {
  // create smokes.
  for(int i = 0; i < MAX_SMOKE; i++) {
    if(i != MAX_SMOKE-1)
      smoke(i).fnext = i+1;
    else
      smoke(i).fnext = AW_null;
  }
  gi->free_smoke_head = 0;
  gi->smoke_head      = AW_null;
  trace("smokes initiated.");
}

AW_smoke_ptr SMOKE_New(AW_game_instance_t *gi) {
  if(gi->free_smoke_head != AW_null) {
    AW_smoke_ptr r            = gi->free_smoke_head;
    gi->free_smoke_head       = smoke(gi->free_smoke_head).fnext;
    smoke(r).d                = SMOKE_MAX_RADIUS;
    smoke(r).span             = SMOKE_LIFE_SPAN;
    smoke(r).previous         = AW_null;
    smoke(r).next             = gi->smoke_head;
    if(gi->smoke_head != AW_null)
      smoke(gi->smoke_head).previous = r;
    gi->smoke_head = r;
    return r;
  }
  trace("No more smoke left.");
  return AW_null;
}

void SMOKE_Free(AW_game_instance_t *gi, AW_smoke_ptr s) {
  if(gi->smoke_head == s)
    gi->smoke_head = smoke(s).next;
  if(smoke(s).previous != AW_null)
    smoke(smoke(s).previous).next = smoke(s).next;
  if(smoke(s).next != AW_null)
    smoke(smoke(s).next).previous = smoke(s).previous;
  smoke(s).fnext = gi->free_smoke_head;
  gi->free_smoke_head = s;
}

void SMOKE_FreeAll(AW_game_instance_t *gi) {
  while(gi->smoke_head != AW_null)
    SMOKE_Free(gi, gi->smoke_head);
  trace("smokes freed.");
}

bool SMOKE_Update(AW_game_instance_t *gi, AW_smoke_ptr s) {
  AW_smoke_t *sm = &smoke(s);
  sm->span -= gi->game_time_step;
  if(sm->span <= 0)
    return true; /* free the smoke */
  return false; /* don't free the smoke */
}

/*
 * bloods
 */

void BLOOD_Init(AW_game_instance_t *gi, int argc, char** argv) {
  // create bloods.
  for(int i = 0; i < MAX_BLOOD; i++) {
    if(i != MAX_BLOOD-1)
      blood(i).fnext = i+1;
    else
      blood(i).fnext = AW_null;
  }
  gi->free_blood_head = 0;
  gi->blood_head      = AW_null;
  trace("bloods initiated.");
}

AW_blood_ptr BLOOD_New(AW_game_instance_t *gi) {
  if(gi->free_blood_head != AW_null) {
    AW_blood_ptr r            = gi->free_blood_head;
    gi->free_blood_head       = blood(gi->free_blood_head).fnext;
    blood(r).span             = BLOOD_LIFE_SPAN;
    blood(r).previous         = AW_null;
    blood(r).next             = gi->blood_head;
    blood(r).c                = TCOD_color_multiply_scalar(BLOOD_COLOR, (float)(128+(rand()&127))/255);
    if(gi->blood_head != AW_null)
      blood(gi->blood_head).previous = r;
    gi->blood_head = r;
    return r;
  }
  trace("No more blood left.");
  return AW_null;
}

void BLOOD_Free(AW_game_instance_t *gi, AW_blood_ptr b) {
  if(gi->blood_head == b)
    gi->blood_head = blood(b).next;
  if(blood(b).previous != AW_null)
    blood(blood(b).previous).next = blood(b).next;
  if(blood(b).next != AW_null)
    blood(blood(b).next).previous = blood(b).previous;
  blood(b).fnext = gi->free_blood_head;
  gi->free_blood_head = b;
}

void BLOOD_FreeAll(AW_game_instance_t *gi) {
  while(gi->blood_head != AW_null)
    BLOOD_Free(gi, gi->blood_head);
  trace("bloods freed.");
}

bool BLOOD_Update(AW_game_instance_t *gi, AW_blood_ptr b) {
  AW_blood_t *bl = &blood(b);
  bl->span -= gi->game_time_step;
  if(bl->span <= 0)
    return true; /* free the blood */
  return false; /* don't free the blood */
}

/*
 * floating_texts
 */

void FT_Init(AW_game_instance_t *gi, int argc, char** argv) {
  // create floating_texts.
  for(int i = 0; i < MAX_FLOATING_TEXT; i++) {
    if(i != MAX_FLOATING_TEXT-1)
      floating_text(i).fnext = i+1;
    else
      floating_text(i).fnext = AW_null;
  }
  gi->free_floating_text_head = 0;
  gi->floating_text_head      = AW_null;
  trace("floating_texts initiated.");
}

AW_floating_text_ptr FT_New(AW_game_instance_t *gi) {
  if(gi->free_floating_text_head != AW_null) {
    AW_floating_text_ptr r            = gi->free_floating_text_head;
    gi->free_floating_text_head       = floating_text(gi->free_floating_text_head).fnext;
    floating_text(r).span             = FLOATING_TEXT_LIFE_SPAN;
    floating_text(r).previous         = AW_null;
    floating_text(r).next             = gi->floating_text_head;
    floating_text(r).c                = TCOD_white;
    floating_text(r).s                = "666";
    if(gi->floating_text_head != AW_null)
      floating_text(gi->floating_text_head).previous = r;
    gi->floating_text_head = r;
    return r;
  }
  trace("No more floating_text left.");
  return AW_null;
}

void FT_Free(AW_game_instance_t *gi, AW_floating_text_ptr ft) {
  if(gi->floating_text_head == ft)
    gi->floating_text_head = floating_text(ft).next;
  if(floating_text(ft).previous != AW_null)
    floating_text(floating_text(ft).previous).next = floating_text(ft).next;
  if(floating_text(ft).next != AW_null)
    floating_text(floating_text(ft).next).previous = floating_text(ft).previous;
  floating_text(ft).fnext = gi->free_floating_text_head;
  gi->free_floating_text_head = ft;
}

void FT_FreeAll(AW_game_instance_t *gi) {
  while(gi->floating_text_head != AW_null)
    FT_Free(gi, gi->floating_text_head);
  trace("floating_texts freed.");
}

bool FT_Update(AW_game_instance_t *gi, AW_floating_text_ptr ft) {
  AW_floating_text_t *flt = &floating_text(ft);
  flt->span -= gi->game_time_step;
  if(flt->span <= 0)
    return true; /* free the floating text */
  return false; /* don't free the floating text */
}

/*
 * lights
 */

void LI_Init(AW_game_instance_t *gi, int argc, char** argv) {
  // create lights.
  gi->tcod_light_map = TCOD_map_new(RANGE_MAP_SIZE_X, RANGE_MAP_SIZE_Y);
  gi->torch_noise    = TCOD_noise_new(1, 1.f, 1.f, null);
  for(int i = 0; i < MAX_LIGHT; i++) {
    if(i != MAX_LIGHT-1)
      light(i).fnext = i+1;
    else
      light(i).fnext = AW_null;
  }
  gi->free_light_head = 0;
  gi->light_head      = AW_null;
  trace("lights initiated.");
}

AW_light_ptr LI_New(AW_game_instance_t *gi) {
  if(gi->free_light_head != AW_null) {
    AW_light_ptr r            = gi->free_light_head;
    gi->free_light_head       = light(gi->free_light_head).fnext;
    light(r).flickx           = 0;
    light(r).previous         = AW_null;
    light(r).next             = gi->light_head;
    if(gi->light_head != AW_null)
      light(gi->light_head).previous = r;
    gi->light_head = r;
    return r;
  }
  trace("No more light left.");
  return AW_null;
}

void LI_Free(AW_game_instance_t *gi, AW_light_ptr l) {
  if(gi->light_head == l)
    gi->light_head = light(l).next;
  if(light(l).previous != AW_null)
    light(light(l).previous).next = light(l).next;
  if(light(l).next != AW_null)
    light(light(l).next).previous = light(l).previous;
  light(l).fnext = gi->free_light_head;
  gi->free_light_head = l;
}

void LI_FreeAll(AW_game_instance_t *gi) {
  while(gi->light_head != AW_null)
    LI_Free(gi, gi->light_head);
  if(gi->torch_noise) {
    TCOD_noise_delete(gi->torch_noise);
    gi->torch_noise = null;
  }
  trace("lights freed.");
}

void LI_Update(AW_game_instance_t *gi) {
  DO_TIMES(MAP_SIZE)
    gi->dyn_light_map[f] = TCOD_black;
  AW_light_ptr l = gi->light_head;
  while(l != AW_null) {
    AW_light_t *li = &light(l);
    float tdx, di;
    int dx, dy;
    int x, y;
    do {
      li->flickx += 0.05f;
      tdx = li->flickx + 20.0f;
      dx  = 0;//(float)TCOD_noise_get(gi->torch_noise, &tdx) * 16;
      tdx += 30.0f;
      dy  = 0;//(float)TCOD_noise_get(gi->torch_noise, &tdx) * 16;
      di  = MIN(1.f, 0.5f + 0.5f * TCOD_noise_get(gi->torch_noise, &li->flickx) * 0.75);
      x   = li->pos_x+dx,
      y   = li->pos_y+dy;
    } while(AT_ENV(x>>RANGE_SHIFT, y>>RANGE_SHIFT) == AW_WALL);
    TCOD_map_compute_fov(
      gi->tcod_light_map, 
      x>>RANGE_SHIFT, 
      y>>RANGE_SHIFT, 
      li->r>>RANGE_SHIFT, 
      true, 
      FOV_RESTRICTIVE);
    FOR_RECT(-li->r, li->r-1, -li->r, li->r-1) {
      if(INSIDE_MAP(x+i, y+j)
      && TCOD_map_is_in_fov(gi->tcod_light_map, (x+i)>>RANGE_SHIFT, (y+j)>>RANGE_SHIFT)) {
        TCOD_color_t c = TORCH_COLOR;
        float intensity = (float)((i+dx) * (i+dx) + (j+dy) * (j+dy)) / (li->r * li->r) + di * 0.5f;
        intensity = MAX(0.f, 1.f - intensity);
        c = TCOD_color_multiply_scalar(c, intensity);
        AT(dyn_light_map, x+i, y+j) = TCOD_color_add(AT(dyn_light_map, x+i, y+j), c);
      }
    }
    l = li->next;
  }
}

/*
 * sounds
 */

void SO_Init(AW_game_instance_t *gi, int argc, char** argv) {
  // create sounds.
  for(int i = 0; i < MAX_SOUND; i++) {
    if(i != MAX_SOUND-1)
      sound(i).fnext = i+1;
    else
      sound(i).fnext = AW_null;
  }
  gi->free_sound_head = 0;
  gi->sound_head      = AW_null;
  trace("sounds initiated.");
}

AW_sound_ptr SO_New(AW_game_instance_t *gi) {
  if(gi->free_sound_head != AW_null) {
    AW_sound_ptr r            = gi->free_sound_head;
    gi->free_sound_head       = sound(gi->free_sound_head).fnext;
    sound(r).u                = AW_null;
    sound(r).volume           = MIX_MAX_VOLUME/2;
    sound(r).pos_x            = -1;
    sound(r).previous         = AW_null;
    sound(r).next             = gi->sound_head;
    if(gi->sound_head != AW_null)
      sound(gi->sound_head).previous = r;
    gi->sound_head = r;
    return r;
  }
  trace("No more sound left.");
  return AW_null;
}

void SO_Free(AW_game_instance_t *gi, AW_sound_ptr l) {
  if(gi->sound_head == l)
    gi->sound_head = sound(l).next;
  if(sound(l).previous != AW_null)
    sound(sound(l).previous).next = sound(l).next;
  if(sound(l).next != AW_null)
    sound(sound(l).next).previous = sound(l).previous;
  sound(l).fnext = gi->free_sound_head;
  gi->free_sound_head = l;
}

void SO_FreeAll(AW_game_instance_t *gi) {
  while(gi->sound_head != AW_null)
    SO_Free(gi, gi->sound_head);
  trace("sounds freed.");
}

void SO_Update(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  AW_sound_ptr s = gi->sound_head;
  while(s != AW_null) {
    AW_sound_t *so = &sound(s);
    int dx = 0, dy = 0;
    int x = cl->viewport_x + CON_RES_X/2,
        y = cl->viewport_y + CON_RES_Y/2;
    if(so->u != AW_null) {
      AW_unit_t *un = &unit(so->u);
      dx = un->pos_x - x,
      dy = un->pos_y - y;
    } else 
    if(so->pos_x != -1) {
      dx = so->pos_x - x,
      dy = so->pos_y - y;
    }
    Mix_Volume(so->channel, so->volume);
    Mix_SetDistance(so->channel, MIN(255, ABS(dy)*6));
    Mix_SetPanning(
      so->channel, 
      MAX(0, 255 - ABS(dx + CON_RES_X/4)*6), 
      MAX(0, 255 - ABS(dx - CON_RES_X/4)*6));
    s = so->next;
  }
}

/*
 * build order
 */

void BO_Init(AW_game_instance_t *gi,int argc, char** argv) {
  for(int i = 0; i < MAX_BUILD_ORDER; i++) {
    if(i != MAX_BUILD_ORDER-1)
      build_order(i).fnext = i+1;
    else
      build_order(i).fnext = AW_null;
  }
  gi->free_build_order_head = 0;
  trace("build_orders initiated.");
}

AW_build_order_ptr BO_New(AW_game_instance_t *gi) {
  if(gi->free_build_order_head != AW_null) {
    AW_build_order_ptr r           = gi->free_build_order_head;
    gi->free_build_order_head  = build_order(gi->free_build_order_head).fnext;
    build_order(r).previous    = AW_null;
    build_order(r).next        = AW_null;
    build_order(r).progress    = 0;
    build_order(r).started     = false;
    return r;
  }
  trace("No more build_order left.");
  return AW_null;
}

void BO_Free(AW_game_instance_t *gi, AW_unit_ptr u, AW_build_order_ptr b) {
  AW_unit_t *un = &unit(u);
  AW_build_order_t *bo = &build_order(b);
  if(un->build_order_head[bo->unit_player_id] == b)
    un->build_order_head[bo->unit_player_id] = build_order(b).next;
  if(build_order(b).previous != AW_null)
    build_order(build_order(b).previous).next = build_order(b).next;
  if(build_order(b).next != AW_null)
    build_order(build_order(b).next).previous = build_order(b).previous;
  build_order(b).fnext = gi->free_build_order_head;
  gi->free_build_order_head = b;
}

void BO_FreeAll(AW_game_instance_t *gi, AW_unit_ptr u) {
  AW_unit_t *un = &unit(u);
  DO_TIMES(MAX_PLAYER)
    while(un->build_order_head[f] != AW_null)
      BO_Free(gi, u, un->build_order_head[f]);
}

void BO_PushFrontBuildOrder(AW_game_instance_t *gi, AW_unit_ptr u, AW_build_order_ptr b) {
  AW_unit_t *un           = &unit(u);
  AW_build_order_t *bo = &build_order(b);
  build_order(b).next     = un->build_order_head[bo->unit_player_id];
  if(un->build_order_head[bo->unit_player_id] != AW_null)
    build_order(un->build_order_head[bo->unit_player_id]).previous = b;
  un->build_order_head[bo->unit_player_id] = b;
}

void BO_PushBackBuildOrder(AW_game_instance_t *gi, AW_unit_ptr u, AW_build_order_ptr b) {
  AW_unit_t *un = &unit(u);
  AW_build_order_t *bo = &build_order(b);
  if(un->build_order_head[bo->unit_player_id] == AW_null) {
    un->build_order_head[bo->unit_player_id] = b;
  } else {
    AW_build_order_ptr b2 = un->build_order_head[bo->unit_player_id];
    while(build_order(b2).next != AW_null)
      b2 = build_order(b2).next;
    build_order(b2).next = b;
    build_order(b).previous = b2;
  }
}

short BO_GetBuildOrderCount(AW_game_instance_t *gi, AW_id_t player_id, AW_client_ptr c, AW_unit_ptr u) {
  AW_client_t *cl = &client(c);
  AW_unit_t *un = &unit(u);
  AW_build_order_ptr b = un->build_order_head[player_id];
  int i = 0;
  while(b != AW_null) {
    i++;
    b = build_order(b).next;
  }
  AW_cmd_store_t *cs = &cmd_store(cl->cs);
  AW_cmd_ptr cm = cs->cmd_head;
  while(cm != AW_null) {
    AW_cmd_t *cmd = &cmd(cm);
    if(cmd->id == un->cmd_id
    && cmd->type == AW_cmd_type_build_unit
    && !cmd->cancel_it)
      i++;
    cm = cmd->cmd_next;
  }
  return i;
}

/*
 *
 */

void UN_Init(AW_game_instance_t *gi, int argc, char** argv) {
  // clear unit map.
  for(int i = 0; i < MAP_SIZE; i++)
    gi->unit_map[i] = AW_null;
  // make units.
  for(int i = 0; i < MAX_UNIT; i++) {
    if(i != MAX_UNIT-1)
      unit(i).fnext = i+1;
    else
      unit(i).fnext = AW_null;
    unit(i).visibility_map = 
      TCOD_map_new(RANGE_MAP_SIZE_X, RANGE_MAP_SIZE_Y);
    unit(i).previous = AW_null;
    unit(i).next     = AW_null;
  }
  gi->free_unit_head = 0;
  gi->unit_head = AW_null;
  trace("Units initiated.");
}

AW_unit_ptr UN_New(AW_game_instance_t *gi) {
  if(gi->free_unit_head != AW_null) {
    AW_unit_ptr u       = gi->free_unit_head;
    AW_unit_t   *un     = &unit(u);
    TCOD_map_clear(un->visibility_map, true, true);
    FOR_RECT(0, RANGE_MAP_SIZE_X-1, 0, RANGE_MAP_SIZE_Y-1)
      if(AT_ENV(i, j) == AW_WALL)
        TCOD_map_set_properties(un->visibility_map, i, j, false, false);
    un->old_pos_x       = -1;
    un->user_data       = AW_null;
    un->order_head      = AW_null;
    DO_TIMES(MAX_PLAYER)
      un->build_order_head[f] = AW_null;
    un->path_len        = 0;
    un->path_index      = 0;
    un->blocked_time    = 0;
    un->move            = 0;   
    un->attack          = 0;
    un->push_power      = 1;
    DO_TIMES(MAX_CLIENT) {
      un->sprevious[f]  = AW_null;
      un->snext[f]      = AW_null;
    }
    DO_TIMES(MAX_GROUP_SELECTION) {
      un->gsprevious[f] = AW_null;
      un->gsnext[f] = AW_null;
    }
    un->pprevious       = AW_null;
    un->pnext           = AW_null;
    un->previous        = AW_null;
    un->next            = gi->unit_head;
    if(gi->unit_head != AW_null)
      unit(gi->unit_head).previous = u;
    gi->unit_head = u;
    gi->free_unit_head  = unit(gi->free_unit_head).fnext;
    return u;
  }
  trace("No more units left.");
  return AW_null;
}

void UN_Free(AW_game_instance_t *gi, AW_unit_ptr u) {
  BO_FreeAll(gi, u);
  UO_FreeAll(gi, u);
  if(gi->unit_head == u)
    gi->unit_head = unit(u).next;
  if(unit(u).previous != AW_null)
    unit(unit(u).previous).next = unit(u).next;
  if(unit(u).next != AW_null)
    unit(unit(u).next).previous = unit(u).previous;
  unit(u).fnext = gi->free_unit_head;
  gi->free_unit_head = u;
}

void UN_FreeAll(AW_game_instance_t *gi) {
  while(gi->unit_head != AW_null)
    UN_Free(gi, gi->unit_head);
  trace("Units freed.");
}

bool UN_CanBeHere(AW_game_instance_t *gi, AW_unit_ptr not_this_one, short size, short pos_x, short pos_y) {
  SOCLE(size)
    if(!INSIDE_MAP(pos_x+i, pos_y+j)
    || AT_ENV((pos_x+i)>>RANGE_SHIFT, (pos_y+j)>>RANGE_SHIFT) == AW_WALL
    || (AT(unit_map, pos_x+i, pos_y+j) != AW_null
    &&  AT(unit_map, pos_x+i, pos_y+j) != not_this_one))
      return false;
  return true;
}

void UN_GetUnitHere(
    AW_get_unit_here_t *data, 
    AW_game_instance_t *gi, 
    AW_unit_ptr not_this_one, 
    short size, 
    short pos_x, 
    short pos_y) {
  data->count = 0;
  SOCLE(size)
    if(INSIDE_MAP(pos_x+i, pos_y+j)) {
      AW_unit_ptr u = AT(unit_map, pos_x+i, pos_y+j);
      if(u != AW_null
      && u != not_this_one) {
        bool already = false;
        for(int f = 0; !already && f < data->count; f++)
          already = data->result[f] == u;
        if(!already)
          data->result[data->count++] = u;
      }
    }
}

void UN_PutInMap(AW_game_instance_t *gi, AW_unit_ptr u) {
  AW_unit_t *un = &unit(u);
  SOCLE(SIZE(un))
    AT(unit_map, un->pos_x+i, un->pos_y+j) = u;
  TCOD_map_compute_fov(
    un->visibility_map, 
    un->pos_x>>RANGE_SHIFT, 
    un->pos_y>>RANGE_SHIFT, 
    VISIBILITY_RANGE(un)>>RANGE_SHIFT, 
    true, 
    FOV_RESTRICTIVE);
}

void UN_RemoveFromMap(AW_game_instance_t *gi, AW_unit_ptr u) {
  AW_unit_t *un = &unit(u);
  SOCLE(SIZE(un))
    AT(unit_map, un->pos_x+i, un->pos_y+j) = AW_null;
}

bool UN_HasOrder(AW_game_instance_t *gi, AW_unit_ptr u, AW_id_t cmd_mask) {
  AW_unit_t *un       = &unit(u);
  AW_unit_order_ptr o = un->order_head;
  while(o != AW_null) {
    AW_unit_order_t *uo = &order(o);
    if(uo->cmd_mask == cmd_mask)
      return true;
    o = uo->next;
  }
  return false;
}

int UN_GetDist(AW_game_instance_t *gi, AW_unit_ptr u, int x, int y) {
  AW_unit_t *un       = &unit(u);
  int     min_dist  = 0x7fffffff;
  SOCLE(SIZE(un)) {
    int pos_x = un->pos_x+i,
          pos_y = un->pos_y+j,
          dist  = (pos_x - x) * (pos_x - x)
                + (pos_y - y) * (pos_y - y);
    if(dist < min_dist)
      min_dist = dist;
  }
  return min_dist;
}

int UN_GetDist(AW_game_instance_t *gi, AW_unit_ptr u, AW_unit_ptr u2) {
  AW_unit_t *un       = &unit(u);
  AW_unit_t *un2      = &unit(u2);
  int     min_dist  = 0x7fffffff;
  SOCLE(SIZE(un)) {
    int pos_x = un->pos_x+i,
          pos_y = un->pos_y+j;
    SOCLE(SIZE(un2)) {
      int pos2_x = un2->pos_x+i,
            pos2_y = un2->pos_y+j,
            dist  = (pos_x - pos2_x) * (pos_x - pos2_x)
                  + (pos_y - pos2_y) * (pos_y - pos2_y);
      if(dist < min_dist)
        min_dist = dist;
    }
  }
  return min_dist;
}

int UN_GetLight(AW_game_instance_t *gi, AW_unit_ptr u, short pos_x, short pos_y) {
  AW_unit_t *un = &unit(u);
  /* this fun is bottleneck */
  bool is_vis = TCOD_map_is_in_fov(un->visibility_map, pos_x, pos_y);
  if(is_vis) {
    int x     = un->pos_x>>RANGE_SHIFT,
        y     = un->pos_y>>RANGE_SHIFT,
        vis   = VISIBILITY_RANGE(un)>>RANGE_SHIFT,
        vis2  = vis * vis,
        dist  = (x - pos_x) * (x - pos_x)
              + (y - pos_y) * (y - pos_y);
    return MAX(0, 255 - (dist<<8) / vis2);
  }
  return 0;
}

/*
 * cmd
 */

void CMD_Init(AW_game_instance_t *gi, int argc, char** argv) {
  for(int i = 0; i < MAX_CMD; i++) {
    if(i != MAX_CMD-1)
      cmd(i).fnext = i+1;
    else
      cmd(i).fnext = AW_null;
  }
  gi->free_cmd_head = 0;
  gi->cmd_head      = AW_null;
  trace("Cmds initiated.");
}

AW_cmd_ptr CMD_New(AW_game_instance_t *gi, AW_id_t id) {
  if(gi->free_cmd_head != AW_null) {
    AW_cmd_ptr r                    = gi->free_cmd_head;
    gi->free_cmd_head               = cmd(gi->free_cmd_head).fnext;
    cmd(r).user_data                = AW_null;
    cmd(r).id                       = id;
    cmd(r).sent                     = false;
    cmd(r).cmd_next                 = AW_null;
    cmd(r).previous                 = AW_null;
    cmd(r).next                     = gi->cmd_head;
    cmd(r).unit_order_completed_cb  = null;
    cmd(r).unit_order_failed_cb     = null;
    cmd(r).start_it                 = false;
    if(gi->cmd_head != AW_null)
      cmd(gi->cmd_head).previous = r;
    gi->cmd_head = r;
    return r;
  }
  trace("No more cmd left.");
  return AW_null;
}

void CMD_Free(AW_game_instance_t *gi, AW_cmd_ptr cmd) {
  if(gi->cmd_head == cmd)
    gi->cmd_head = cmd(cmd).next;
  if(cmd(cmd).previous != AW_null)
    cmd(cmd(cmd).previous).next = cmd(cmd).next;
  if(cmd(cmd).next != AW_null)
    cmd(cmd(cmd).next).previous = cmd(cmd).previous;
  cmd(cmd).fnext = gi->free_cmd_head;
  gi->free_cmd_head = cmd;
}

void CMD_FreeAll(AW_game_instance_t *gi) {
  while(gi->cmd_head != AW_null)
    CMD_Free(gi, gi->cmd_head);
  trace("Cmds freed.");
}

/*
 * cmd store
 */

void CS_Init(AW_game_instance_t *gi, int argc, char** argv) {
  for(int i = 0; i < MAX_PLAYER; i++) {
    if(i != MAX_PLAYER-1)
      cmd_store(i).fnext = i+1;
    else
      cmd_store(i).fnext = AW_null;
  }
  gi->free_cmd_store_head = 0;
  gi->cmd_store_head      = AW_null;
  trace("Cmd stores initiated.");
}

AW_cmd_store_ptr CS_New(AW_game_instance_t *gi) {
  if(gi->free_cmd_store_head != AW_null) {
    AW_cmd_store_ptr r = gi->free_cmd_store_head;
    gi->free_cmd_store_head = cmd_store(gi->free_cmd_store_head).fnext;
    AW_cmd_store_t *cs = &cmd_store(r);
    cmd_store(r).cmd_head   = AW_null;
    cmd_store(r).previous   = AW_null;
    cmd_store(r).next       = gi->cmd_store_head;
    if(gi->cmd_store_head != AW_null)
      cmd_store(gi->cmd_store_head).previous = r;
    gi->cmd_store_head = r;
    return r;
  }
  trace("No more cmd_store left.");
  return AW_null;
}

void CS_Free(AW_game_instance_t *gi, AW_cmd_store_ptr cs) {
  if(gi->cmd_store_head == cs)
    gi->cmd_store_head = cmd_store(cs).next;
  if(cmd_store(cs).previous != AW_null)
    cmd_store(cmd_store(cs).previous).next 
      = cmd_store(cs).next;
  if(cmd_store(cs).next != AW_null)
    cmd_store(cmd_store(cs).next).previous 
      = cmd_store(cs).previous;
  cmd_store(cs).fnext = gi->free_cmd_store_head;
  gi->free_cmd_store_head = cs;
}

void CS_FreeAll(AW_game_instance_t *gi) {
  while(gi->cmd_store_head != AW_null)
    CS_Free(gi, gi->cmd_store_head);
  trace("Cmd stores freed.");
}

void CS_ClearCmdStore(AW_game_instance_t *gi, AW_cmd_store_ptr c) {
  AW_cmd_store_t *cs    = &cmd_store(c);
  AW_cmd_ptr  r         = cs->cmd_head,
              previous  = r;
  while(r != AW_null) {
    if(cmd(r).turn == gi->turn) {
      if(r == cs->cmd_head) {
        cs->cmd_head = cmd(r).cmd_next;
        CMD_Free(gi, r);
        previous = r = cs->cmd_head;
      } else {
        cmd(previous).cmd_next = cmd(r).cmd_next;
        CMD_Free(gi, r);
        r = cmd(previous).cmd_next;
      }
    } else {
      previous = r;
      r = cmd(r).cmd_next;
    }
  }
}

void CS_InsertCmd(AW_game_instance_t *gi, AW_cmd_store_ptr r, AW_cmd_ptr c) {
  AW_cmd_store_t *cs    = &cmd_store(r);
  AW_cmd_ptr  d         = cs->cmd_head,
              previous  = d;
  while(d != AW_null) {
    previous = d;
    d = cmd(d).cmd_next;
  }
  if(previous != AW_null)
    cmd(previous).cmd_next = c;
  else
    cs->cmd_head = c;
}

/*
 *
 */

void RS_Init(AW_game_instance_t *gi, int argc, char** argv) {
  for(int i = 0; i < MAX_PLAYER; i++) {
    if(i != MAX_PLAYER-1)
      remote_cmd_store(i).fnext = i+1;
    else
      remote_cmd_store(i).fnext = AW_null;
  }
  gi->free_remote_cmd_store_head = 0;
  gi->remote_cmd_store_head      = AW_null;
  trace("Remote cmd stores initiated.");
}

AW_remote_cmd_store_ptr RS_New(AW_game_instance_t *gi, AW_cmd_store_ptr cmd_store) {
  if(gi->free_remote_cmd_store_head != AW_null) {
    AW_remote_cmd_store_ptr r 
      = gi->free_remote_cmd_store_head;
    gi->free_remote_cmd_store_head 
      = remote_cmd_store(gi->free_remote_cmd_store_head).fnext;
    AW_remote_cmd_store_t *rs = &remote_cmd_store(r);
    rs->cs                  = cmd_store;
    rs->previous            = AW_null;
    rs->peer                = null;
    rs->host                = null;
    rs->connected           = false;
    rs->next                = gi->remote_cmd_store_head;
    if(gi->remote_cmd_store_head != AW_null)
      remote_cmd_store(gi->remote_cmd_store_head).previous = r;
    gi->remote_cmd_store_head = r;
    return r;
  }
  trace("No more remote_cmd_store left.");
  return AW_null;
}

void RS_Connect(AW_game_instance_t *gi, AW_remote_cmd_store_ptr r, const str &game_name) {
  AW_remote_cmd_store_t *rs = &remote_cmd_store(r);
  rs->address.host          = ENET_HOST_ANY;
  rs->address.port          = 0;
  rs->host                  = enet_host_create(&rs->address, MAX_PEER, 1, 0, 0);
  rs->peer                  = enet_host_connect(rs->host, &gi->master_server_addr, 1, 0);
  assert(rs->peer);
  enet_host_flush(rs->host);
}

void RS_WaitForConnection(AW_game_instance_t *gi, AW_remote_cmd_store_ptr r) {
  AW_remote_cmd_store_t *rs = &remote_cmd_store(r);
  ENetEvent e;
  if(!rs->connected
  && enet_host_service(rs->host, &e, 50) != ENET_EVENT_TYPE_NONE) {
    switch(e.type) {
      case ENET_EVENT_TYPE_CONNECT: {
          trace("RS: Connection to master server succeed.");
          ENetPacket *packet =
          enet_packet_create(null, 0, ENET_PACKET_FLAG_RELIABLE);
          AW_leech_game_packet_t data;
          data.type         = AW_cmd_type_leech_game;
          data.player_id    = rs->player_id;
          trace("rs player id: " + i2a(rs->player_id));
          strcpy(data.game_name, game_desc.game_name.c_str());
          enet_packet_resize(packet, sizeof(data));
          memcpy(packet->data, &data, sizeof(data));
          enet_peer_send(rs->peer, 0, packet);
          enet_host_flush(rs->host);
          rs->connected = true;
        } break;
      case ENET_EVENT_TYPE_DISCONNECT:
        trace("RS: cannot connect to master server.");
        break;
      case ENET_EVENT_TYPE_RECEIVE:
        assert(false);
        break;
    }
  }
}

void RS_Free(AW_game_instance_t *gi, AW_remote_cmd_store_ptr r) {
  AW_remote_cmd_store_t *rs = &remote_cmd_store(r);
  if(gi->remote_cmd_store_head == r)
    gi->remote_cmd_store_head = rs->next;
  if(rs->previous != AW_null)
    remote_cmd_store(rs->previous).next = rs->next;
  if(rs->next != AW_null)
    remote_cmd_store(rs->next).previous = rs->previous;
  if(rs->peer) {
    enet_peer_disconnect(rs->peer, 0);
    ENetEvent e;
    enet_host_service(rs->host, &e, 500);
    enet_peer_reset(rs->peer);
  }
  if(rs->host)
    enet_host_destroy(rs->host);
  trace(gi->name + " rs: disconnection.");
  rs->fnext = gi->free_remote_cmd_store_head;
  gi->free_remote_cmd_store_head = r;
}

void RS_FreeAll(AW_game_instance_t *gi) {
  while(gi->remote_cmd_store_head != AW_null)
    RS_Free(gi, gi->remote_cmd_store_head);
  trace("Remote cmd stores freed.");
}

void RS_Update(AW_game_instance_t *gi, AW_remote_cmd_store_ptr r) {
  AW_remote_cmd_store_t *rs = &remote_cmd_store(r);
  rs->ping = gi->ping;
  RS_Leech(gi, r);
}

void RS_Leech(AW_game_instance_t *gi, AW_remote_cmd_store_ptr r) {
  AW_remote_cmd_store_t *rs = &remote_cmd_store(r);
  ENetEvent e;
  while(enet_host_service(rs->host, &e, 0) != ENET_EVENT_TYPE_NONE) {
    ENetPacket *packet = e.packet;
    switch(e.type) {
      case ENET_EVENT_TYPE_CONNECT:
        trace(gi->name + " rs: connection succeed.");
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        e.peer->data = NULL;
        trace(gi->name + " rs: disconnection.");
        break;
      case ENET_EVENT_TYPE_RECEIVE:
        if(packet->dataLength > 0) {
          z_stream strm;
          strm.zalloc   = Z_NULL;
          strm.zfree    = Z_NULL;
          strm.opaque   = Z_NULL;
          strm.avail_in = 0;
          strm.next_in  = Z_NULL;
          AW_ZErr(inflateInit(&strm));
          strm.avail_in   = packet->dataLength;
          strm.next_in    = packet->data;
          strm.avail_out  = ZLIB_CHUNK;
          strm.next_out   = zlib_out;
          int ret = inflate(&strm, Z_FINISH);
          AW_ZErr(ret);
          assert(ret == Z_STREAM_END);
          inflateEnd(&strm);
          //trace("" + i2a(packet->dataLength) + " " + i2a((int)(ZLIB_CHUNK - strm.avail_out)));
          unsigned char *pack_data = zlib_out;
          AW_cmd_type_t type = ((AW_cmd_type_t*)pack_data)[0];
          while(type != AW_cmd_type_none) {
            AW_id_t cmd_id      = ((AW_end_of_turn_packet_t*)pack_data)->id;
            AW_cmd_ptr r        = CMD_New(gi, cmd_id);
            AW_cmd_t *cmd       = &cmd(r);
            switch(type) {
              case AW_cmd_type_unit_order: {
                  AW_unit_order_packet_t *data = 
                    (AW_unit_order_packet_t*)pack_data;
                  cmd->type             = data->type;
                  cmd->id               = data->id;
                  cmd->turn             = data->turn;
                  cmd->cmd_mask         = data->cmd_mask;
                  cmd->r_squared        = data->r_squared;
                  cmd->target_cx        = data->target_cx;
                  cmd->target_cy        = data->target_cy;
                  cmd->target           = data->target;
                  cmd->target_player_id = data->target_player_id;
                  cmd->target_cmd_id    = data->target_cmd_id;
                  cmd->attack_here      = data->attack_here;
                  cmd->push_back        = data->push_back;
                  cmd->time_stamp       = data->time_stamp;
                  #if !REPLAY
                  CS_InsertCmd(gi, rs->cs, r);
                  #endif
                  pack_data += sizeof(AW_unit_order_packet_t);
                }
                break;
              case AW_cmd_type_spawn_unit: {
                  AW_spawn_unit_packet_t *data = 
                    (AW_spawn_unit_packet_t*)pack_data;
                  cmd->type       = data->type;
                  cmd->id         = data->id;
                  cmd->unit_type  = data->unit_type;
                  cmd->turn       = data->turn;
                  cmd->target_cx  = data->target_cx;
                  cmd->target_cy  = data->target_cy;
                  #if !REPLAY
                  CS_InsertCmd(gi, rs->cs, r);
                  #endif
                  pack_data += sizeof(AW_spawn_unit_packet_t);
                }
                break;
              case AW_cmd_type_build_unit: {
                  AW_build_unit_packet_t *data = 
                    (AW_build_unit_packet_t*)pack_data;
                  cmd->type               = data->type;
                  cmd->id                 = data->id;
                  cmd->turn               = data->turn;
                  cmd->cmd_mask           = data->cmd_mask;
                  cmd->unit_type          = data->unit_type;
                  cmd->player_id          = data->player_id;
                  cmd->target_player_id   = data->target_player_id;
                  cmd->target_cx          = data->target_cx;
                  cmd->target_cy          = data->target_cy;
                  cmd->start_it           = data->start_it;
                  cmd->cancel_it          = data->cancel_it;
                  #if !REPLAY
                  CS_InsertCmd(gi, rs->cs, r);
                  #endif
                  pack_data += sizeof(AW_build_unit_packet_t);
                }
                break;
              case AW_cmd_type_generic: {
                  AW_generic_packet_t *data = 
                    (AW_generic_packet_t*)pack_data;
                  cmd->type               = data->type;
                  cmd->id                 = data->id;
                  cmd->turn               = data->turn;
                  cmd->cmd_mask           = data->cmd_mask;
                  cmd->unit_type          = data->unit_type;
                  cmd->player_id          = data->player_id;
                  cmd->target_cx          = data->target_cx;
                  cmd->target_cy          = data->target_cy;
                  #if !REPLAY
                  CS_InsertCmd(gi, rs->cs, r);
                  #endif
                  pack_data += sizeof(AW_generic_packet_t);
                }
                break;
              case AW_cmd_type_end_of_turn: {
                AW_end_of_turn_packet_t *data = 
                  (AW_end_of_turn_packet_t*)pack_data;
                cmd->type               = data->type;
                cmd->id                 = data->id;
                cmd->turn               = data->turn;
                cmd->turn_frame_count   = data->turn_frame_count;
                cmd->frame_time_step    = data->frame_time_step;
                cmd->hash               = data->hash;
                CS_InsertCmd(gi, rs->cs, r);
                pack_data += sizeof(AW_end_of_turn_packet_t);
                //trace("EOT: " + i2a(data->turn));
              }
              break;
            }
            type = *(AW_cmd_type_t*)pack_data;
          }
        }
        enet_packet_destroy(packet);
        break;
    }
  }
}

/*
 * client
 */

void CL_Init(AW_game_instance_t *gi, int argc, char** argv) {
  for(int i = 0; i < MAX_CLIENT; i++) {
    if(i != MAX_CLIENT-1)
      client(i).fnext = i+1;
    else
      client(i).fnext = AW_null;
  }
  gi->free_client_head = 0;
  gi->client_head      = AW_null;
  trace("Clients initiated.");
}

AW_client_ptr CL_New(AW_game_instance_t *gi, AW_cmd_store_ptr cs) {
  if(gi->free_client_head != AW_null) {
    AW_client_ptr r     = gi->free_client_head;
    gi->free_client_head    = client(gi->free_client_head).fnext;
    client(r).selected_units_head  = AW_null;
    client(r).cmd_id              = 0;
    client(r).attack_here         = false;
    client(r).cs                  = cs;
    client(r).previous            = AW_null;
    client(r).peer                = null;
    client(r).host                = null;
    client(r).viewport_x          = 0;
    client(r).viewport_y          = 0;
    client(r).user_data           = AW_null;
    client(r).selected_sub_group  = -1;
    client(r).hud_state           = 0;
    client(r).window_opened       = false;
    DO_TIMES(MAX_GROUP_SELECTION) {
      client(r).gs_head[f] = AW_null;
      client(r).gs_sub_group[f] = -1;
    }
    client(r).next                = gi->client_head;
    if(gi->client_head != AW_null)
      client(gi->client_head).previous = r;
    gi->client_head         = r;
    return r;
  } 
  trace("No more client left.");
  return AW_null;
}

void CL_Free(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  if(gi->client_head == c)
    gi->client_head = cl->next;
  if(cl->previous != AW_null)
    client(cl->previous).next = cl->next;
  if(cl->next != AW_null)
    client(cl->next).previous = cl->previous;
  cl->fnext = gi->free_client_head;
  if(cl->peer) {
    enet_peer_disconnect(cl->peer, 0);
    ENetEvent e;
    enet_host_service(cl->host, &e, 500);
    enet_peer_reset(cl->peer);
  }
  if(cl->host)
    enet_host_destroy(cl->host);
  trace(gi->name + " cl: disconnection.");
  gi->free_client_head = c;
}

void CL_FreeAll(AW_game_instance_t *gi) {
  while(gi->client_head != AW_null)
    CL_Free(gi, gi->client_head);
  trace("Clients freed.");
}

void CL_Connect(AW_game_instance_t *gi, AW_client_ptr c, const str &game_name) {
  AW_client_t *cl         = &client(c);
  cl->host                = null;
  ENetPacket *packet =
    enet_packet_create(null, 0, ENET_PACKET_FLAG_RELIABLE);
  AW_seed_game_packet_t data;
  data.type         = AW_cmd_type_seed_game;
  data.player_id    = cl->player_id;
  trace("cl player id: " + i2a(cl->player_id));
  strcpy(data.game_name, game_name.c_str());
  enet_packet_resize(packet, sizeof(data));
  memcpy(packet->data, &data, sizeof(data));
  enet_peer_send(gi->peer, 0, packet);
  enet_host_flush(gi->host);
}

void CL_WaitForConnection(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  if(cl->host) {
    ENetEvent e;
    while(enet_host_service(cl->host, &e, 50) != ENET_EVENT_TYPE_NONE) {
      switch(e.type) {
        case ENET_EVENT_TYPE_CONNECT:
          trace("CL: Connection to master server succeed.");
          cl->connected = true;
          break;
        case ENET_EVENT_TYPE_DISCONNECT:
          trace("CL: cannot connect to master server.");
          GI_FailureFlash(gi, "Cannot connect to master server.");
          GI_RemoveState(gi, AW_state_ready);
          GI_AddState(gi, AW_state_main_menu);
          break;
        case ENET_EVENT_TYPE_RECEIVE:
          trace("CL: a packet has been received.");
          enet_packet_destroy(e.packet);
          break;
      }
    }
  }
}

void CL_Seed(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  AW_cmd_store_t *cs = &cmd_store(cl->cs);
  if(cs->cmd_head != AW_null) {
    int len = 0;
    AW_cmd_ptr cm = cs->cmd_head;
    while(cm != AW_null) {
      AW_cmd_t *cmd = &cmd(cm);
      if(!cmd->sent
      && gi->turn + MAX_ADVANCE_TURN > cmd->turn) {
        switch(cmd->type) {
          case AW_cmd_type_unit_order:
            len += sizeof(AW_unit_order_packet_t);
            break;
          case AW_cmd_type_spawn_unit:
            len += sizeof(AW_spawn_unit_packet_t);
            break;
          case AW_cmd_type_build_unit:
            len += sizeof(AW_build_unit_packet_t);
            break;
          case AW_cmd_type_generic:
            len += sizeof(AW_generic_packet_t);
            break;
          case AW_cmd_type_end_of_turn:
            len += sizeof(AW_end_of_turn_packet_t);
            break;
        }
      }
      cm = cmd->cmd_next;
    }
    if(len > 0) {
      len += sizeof(AW_cmd_type_t);
      bool dyn_alloc = len >= ZLIB_CHUNK;
      unsigned char *input      = dyn_alloc ? (unsigned char*)malloc(len) : zlib_in,
                    *pack_data  = input,
                    *output     = dyn_alloc ? (unsigned char*)malloc(len) : zlib_out;
      cm = cs->cmd_head;
      while(cm != AW_null) {
        AW_cmd_t *cmd = &cmd(cm);
        if(!cmd->sent
        && gi->turn + MAX_ADVANCE_TURN > cmd->turn) {
          cmd->sent = true;
          switch(cmd->type) {
            case AW_cmd_type_unit_order: {
                AW_unit_order_packet_t data;
                data.type             = AW_cmd_type_unit_order;
                data.id               = cmd->id;
                data.order            = AW_unit_order_move;
                data.turn             = cmd->turn;
                data.cmd_mask         = cmd->cmd_mask;
                data.r_squared        = cmd->r_squared;
                data.target_cx        = cmd->target_cx;
                data.target_cy        = cmd->target_cy;
                data.target           = cmd->target;
                data.target_player_id = cmd->target_player_id;
                data.target_cmd_id    = cmd->target_cmd_id;
                data.attack_here      = cmd->attack_here;
                data.push_back        = cmd->push_back;
                data.time_stamp       = cmd->time_stamp;
                memcpy(pack_data, &data, sizeof(data));
                pack_data += sizeof(data);
              }
              break;
            case AW_cmd_type_spawn_unit: {
                AW_spawn_unit_packet_t data;
                data.type       = AW_cmd_type_spawn_unit;
                data.id         = cmd->id;
                data.unit_type  = cmd->unit_type;
                data.turn       = cmd->turn;
                data.target_cx  = cmd->target_cx;
                data.target_cy  = cmd->target_cy;
                memcpy(pack_data, &data, sizeof(data));
                pack_data += sizeof(data);
              }
              break;
            case AW_cmd_type_build_unit: {
                AW_build_unit_packet_t data;
                data.type               = AW_cmd_type_build_unit;
                data.id                 = cmd->id;
                data.turn               = cmd->turn;
                data.cmd_mask           = cmd->cmd_mask;
                data.unit_type          = cmd->unit_type;
                data.player_id          = cmd->player_id;
                data.target_player_id   = cmd->target_player_id;
                data.target_cx          = cmd->target_cx;
                data.target_cy          = cmd->target_cy;
                data.start_it           = cmd->start_it;
                data.cancel_it          = cmd->cancel_it;
                memcpy(pack_data, &data, sizeof(data));
                pack_data += sizeof(data);
              }
              break;
            case AW_cmd_type_generic: {
                AW_generic_packet_t data;
                data.type               = AW_cmd_type_generic;
                data.id                 = cmd->id;
                data.turn               = cmd->turn;
                data.cmd_mask           = cmd->cmd_mask;
                data.unit_type          = cmd->unit_type;
                data.player_id          = cmd->player_id;
                data.target_cx          = cmd->target_cx;
                data.target_cy          = cmd->target_cy;
                memcpy(pack_data, &data, sizeof(data));
                pack_data += sizeof(data);
              }
              break;
            case AW_cmd_type_end_of_turn: {
                AW_end_of_turn_packet_t data;
                data.type             = AW_cmd_type_end_of_turn;
                data.id               = cmd->id;
                data.turn             = cmd->turn;
                data.turn_frame_count = cmd->turn_frame_count;
                data.frame_time_step  = cmd->frame_time_step;
                data.hash             = cmd->hash;
                memcpy(pack_data, &data, sizeof(data));
                pack_data += sizeof(data);
              }
              break;
          }
        }
        cm = cmd->cmd_next;
      }
      *(AW_cmd_type_t*)pack_data = AW_cmd_type_none;
      z_stream strm;
      strm.zalloc     = Z_NULL;
      strm.zfree      = Z_NULL;
      strm.opaque     = Z_NULL;
      // we have cpu en masse
      AW_ZErr(deflateInit(&strm, 9));
      strm.avail_in   = len;
      strm.next_in    = input;
      strm.avail_out  = dyn_alloc ? len : ZLIB_CHUNK;
      strm.next_out   = output;
      AW_ZErr(deflate(&strm, Z_FINISH));
      assert(strm.avail_in == 0);
      int out_len = (dyn_alloc ? len : ZLIB_CHUNK) - strm.avail_out;
      deflateEnd(&strm);
      ENetPacket *packet = 
        enet_packet_create(
          null, 
          0, 
          ENET_PACKET_FLAG_RELIABLE);
      enet_packet_resize(packet, out_len);
      memcpy(packet->data, output, out_len);
      if(dyn_alloc) {
        free(input);
        free(output);
      }
      enet_peer_send(cl->peer, 0, packet);
      enet_host_flush(cl->host);
      //trace("" + i2a(len) + " " + i2a(out_len));
    }
  }
  ENetEvent e;
  while(enet_host_service(cl->host, &e, 0) != ENET_EVENT_TYPE_NONE) {
    switch(e.type) {
      case ENET_EVENT_TYPE_CONNECT:
        trace("CL: Connection to master server succeed.");
        assert(false);
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        trace("CL: cannot connect to master server.");
        //assert(false);
        break;
      case ENET_EVENT_TYPE_RECEIVE:
        trace("CL: a packet has been received.");
        enet_packet_destroy(e.packet);
        break;
    }
  }
}

void CL_Update(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  /* standart update for the human client */
  if(cl->type == HUMAN_CLIENT) {
    CL_UpdateInputs(gi, c);
    LI_Update(gi);
    SO_Update(gi, c);
    CL_RenderSmokes(gi, c);
    CL_RenderEnvMap(gi, c);
    CL_RenderUnits(gi, c);
    CL_RenderBlood(gi, c);
    CL_RenderFloatingText(gi, c);
    CL_RenderBloom(gi, c);
    CL_RenderBalls(gi, c);
    CL_RenderSelection(gi, c);
    CL_RenderCursor(gi, c);
    CL_RenderWinLose(gi, c);
    CL_RenderWindow(gi, c);
    CL_RenderFps(gi);
    CL_RenderPing(gi);
    CL_RenderStats(gi);
    CL_UpdateHUD(gi, c);
  }
  if(gi->update_client_cb)
    gi->update_client_cb(gi, c);
  /* finally seeds for everybody */
  CL_Seed(gi, c);
  /* option window */
  if(cl->type == HUMAN_CLIENT) {
    /* which possibly quit then must 
       be at the end of cl_update */
    CL_UpdateOptionWindow(gi, c);
    /* here so that escape does not escape also 
       with the option window opened */
    if(!cl->window_opened
    && GI_IsKeyReleased(gi, TCODK_ESCAPE))
      CL_OpenOptionWindow(gi, c);
  }
}

void CL_EndOfTurn(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl         = &client(c);
  AW_cmd_ptr r            = CMD_New(gi, cl->cmd_id++);
  AW_cmd_t *cmd           = &cmd(r);
  cmd->type               = AW_cmd_type_end_of_turn;
  cmd->turn               = gi->turn + CMD_TURN_SHIFT;
  cmd->frame_time_step    = gi->aver_frame_time;
  int target              = ceil((float)gi->max_ping / MAX(1, cmd->frame_time_step));
  cmd->turn_frame_count   = MIN(MAX_TURN_FRAME_COUNT, target);
  bool map[MAP_SIZE];
  DO_TIMES(MAP_SIZE)
    map[f] = gi->unit_map[f] != AW_null;
  cmd->hash = Aw_Hash((unsigned char*)map, MAP_SIZE*sizeof(bool));
  #if REPLAY
  AW_cmd_ptr r2 = cmd_store(cl->cs).cmd_head;
  while(r2 != AW_null) {
    AW_cmd_t *cmd2 = &cmd(r2);
    if(cmd->turn == cmd2->turn
    && cmd->type == cmd2->type) {
      cmd->frame_time_step = cmd2->frame_time_step;
      cmd->turn_frame_count = cmd2->turn_frame_count;
    }
    r2 = cmd2->cmd_next;
  }
  #endif
  CS_InsertCmd(gi, cl->cs, r);
  // seed immediately
  CL_Seed(gi, c);
}

void CL_UpdateInputs(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  AW_player_ptr cl_p = GI_GetPlayerPtr(gi, cl->player_id);
  AW_player_t *cl_pl = &player(cl_p);
  if(gi->lbtn_down
  && !cl->attack_here
  && !cl->selection_started
  && INSIDE_MINI(gi->mouse.cx, gi->mouse.cy))
    cl->pointing_minimap = true;
  if(gi->lbtn_down
  && !cl->attack_here
  && !cl->selection_started
  && cl->pointing_minimap) {
    cl->viewport_x = (gi->mouse.x-MINIMAP_START_X)*MINIMAP_INV_RATIO_X-CON_RES_X/2;
    cl->viewport_y = (gi->mouse.y-MINIMAP_START_Y)*MINIMAP_INV_RATIO_Y-CON_RES_Y/2;
  }
  if(gi->lbtn_up)
    cl->pointing_minimap = false;
  if(gi->mouse.cx <= 0
  || GI_IsKeyPressed2(gi, TCODK_LEFT))
    cl->viewport_x -= (short)((float)0.1f * gi->frame_time);
  if(gi->mouse.cx >= CON_RES_X-1
  || GI_IsKeyPressed2(gi, TCODK_RIGHT))
    cl->viewport_x += (short)((float)0.1f * gi->frame_time);
  if(gi->mouse.cy <= 0
  || GI_IsKeyPressed2(gi, TCODK_UP))
    cl->viewport_y -= (short)((float)0.1f * gi->frame_time);
  if(gi->mouse.cy >= CON_RES_Y-1
  || GI_IsKeyPressed2(gi, TCODK_DOWN))
    cl->viewport_y += (short)((float)0.1f * gi->frame_time);
  BOUND_VIEWPORT
  /* restore selection */
  if(/*GI_IsKeyReleased(gi, TCODK_CHAR, '&', 0)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '1', 0)
  || GI_IsKeyPressed(gi, TCODK_1, -1, 0))
    CL_RestoreGroupSelection(gi, c, 0);
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, -23, 0)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '2', 0)
  || GI_IsKeyPressed(gi, TCODK_2, -1, 0))
    CL_RestoreGroupSelection(gi, c, 1);
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, '\"', 0)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '3', 0)
  || GI_IsKeyPressed(gi, TCODK_3, -1, 0))
    CL_RestoreGroupSelection(gi, c, 2);
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, '\'', 0)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '4', 0)
  || GI_IsKeyPressed(gi, TCODK_4, -1, 0))
    CL_RestoreGroupSelection(gi, c, 3);
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, '(', 0)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '5', 0)
  || GI_IsKeyPressed(gi, TCODK_5, -1, 0))
    CL_RestoreGroupSelection(gi, c, 4);
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, '-', 0)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '6', 0)
  || GI_IsKeyPressed(gi, TCODK_6, -1, 0))
    CL_RestoreGroupSelection(gi, c, 5);
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, -15, 0)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '7', 0)
  || GI_IsKeyPressed(gi, TCODK_7, -1, 0))
    CL_RestoreGroupSelection(gi, c, 6);
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, '_', 0)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '8', 0)
  || GI_IsKeyPressed(gi, TCODK_8, -1, 0))
    CL_RestoreGroupSelection(gi, c, 7);
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, -25, 0)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '9', 0)
  || GI_IsKeyPressed(gi, TCODK_9, -1, 0))
    CL_RestoreGroupSelection(gi, c, 8);
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, -20, 0)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '0', 0)
  || GI_IsKeyPressed(gi, TCODK_0, -1, 0))
    CL_RestoreGroupSelection(gi, c, 9);
  /* copy selection to group */
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, '&', 1)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '1', 1)
  || GI_IsKeyPressed(gi, TCODK_1, -1, 1))
    CL_CopySelectionTo(gi, c, 0);
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, -23, 1)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '2', 1)
  || GI_IsKeyPressed(gi, TCODK_2, -1, 1))
    CL_CopySelectionTo(gi, c, 1);
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, '\"', 1)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '3', 1)
  || GI_IsKeyPressed(gi, TCODK_3, -1, 1))
    CL_CopySelectionTo(gi, c, 2);
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, '\'', 1)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '4', 1)
  || GI_IsKeyPressed(gi, TCODK_4, -1, 1))
    CL_CopySelectionTo(gi, c, 3);
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, '(', 1)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '5', 1)
  || GI_IsKeyPressed(gi, TCODK_5, -1, 1))
    CL_CopySelectionTo(gi, c, 4);
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, '-', 1)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '6', 1)
  || GI_IsKeyPressed(gi, TCODK_6, -1, 1))
    CL_CopySelectionTo(gi, c, 5);
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, -15, 1)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '7', 1)
  || GI_IsKeyPressed(gi, TCODK_7, -1, 1))
    CL_CopySelectionTo(gi, c, 6);
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, '_', 1)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '8', 1)
  || GI_IsKeyPressed(gi, TCODK_8, -1, 1))
    CL_CopySelectionTo(gi, c, 7);
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, -25, 1)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '9', 1)
  || GI_IsKeyPressed(gi, TCODK_9, -1, 1))
    CL_CopySelectionTo(gi, c, 8);
  if(/*GI_IsKeyPressed(gi, TCODK_CHAR, -20, 1)
  || */GI_IsKeyPressed(gi, TCODK_CHAR, '0', 1)
  || GI_IsKeyPressed(gi, TCODK_0, -1, 1))
    CL_CopySelectionTo(gi, c, 9);
  if(!cl_pl->lose && !cl_pl->win && !cl->window_opened) {
    if(gi->lbtn_down 
    && !cl->selection_started
    && !cl->pointing_minimap
    && INSIDE_CON(gi->mouse.cx, gi->mouse.cy)) {
      cl->mp_lb_x = gi->mouse.cx+cl->viewport_x;
      cl->mp_lb_y = gi->mouse.cy+cl->viewport_y;
      cl->selection_started = true;
    }
    if(gi->lbtn_up
    && cl->selection_started) {
      cl->mr_lb_x = gi->mouse.cx+cl->viewport_x;
      cl->mr_lb_y = gi->mouse.cy+cl->viewport_y;
      cl->selection_started = false;
      if(!cl->attack_here
      && CL_IsThereUnitsHere(gi, c, cl->mp_lb_x, cl->mp_lb_y, cl->mr_lb_x, cl->mr_lb_y)) {
        if(!GI_IsKeyPressed2(gi, TCODK_SHIFT))
          CL_FreeUnitSelection(gi, c);
        AW_unit_ptr u = AT(unit_map, cl->mp_lb_x, cl->mp_lb_y);
        if(GI_IsKeyPressed2(gi, TCODK_SHIFT)
        && !GI_IsKeyPressed2(gi, TCODK_CONTROL)
        && cl->mp_lb_x == cl->mr_lb_x
        && cl->mp_lb_y == cl->mr_lb_y
        && CL_IsUnitSelected(gi, c, u))
          CL_SubToSelection(gi, c, u);
        else {
          if(GI_IsKeyPressed2(gi, TCODK_CONTROL)
          && cl->mp_lb_x == cl->mr_lb_x
          && cl->mp_lb_y == cl->mr_lb_y)
            CL_SelectUnits(
              gi, 
              c, 
              cl->mp_lb_x-SAME_UNIT_TYPE_SIZE, 
              cl->mp_lb_y-SAME_UNIT_TYPE_SIZE, 
              cl->mr_lb_x+SAME_UNIT_TYPE_SIZE, 
              cl->mr_lb_y+SAME_UNIT_TYPE_SIZE,
              unit(u).unit_type);
          else
            CL_SelectUnits(
              gi, 
              c, 
              cl->mp_lb_x, 
              cl->mp_lb_y, 
              cl->mr_lb_x, 
              cl->mr_lb_y, 
              AW_unit_type_count);
        }
      }
    }
    bool can_spawn = PL_IsInFov(
                    gi, 
                    cl_p, 
                    (gi->mouse.cx+cl->viewport_x)>>RANGE_SHIFT,
                    (gi->mouse.cy+cl->viewport_y)>>RANGE_SHIFT);
    if(can_spawn
    && (GI_IsKeyReleased(gi, TCODK_CHAR, 'p')
    || GI_IsKeyReleased(gi, TCODK_CHAR, 'P'))
    && INSIDE_CON(gi->mouse.cx, gi->mouse.cy))
      DO_TIMES(100)
        CL_CmdSpawnUnit(
          gi, 
          c, 
          gi->mouse.cx+cl->viewport_x, 
          gi->mouse.cy+cl->viewport_y,
          AW_unit_type_g);
    if(can_spawn
    && (GI_IsKeyReleased(gi, TCODK_CHAR, 'l')
    || GI_IsKeyReleased(gi, TCODK_CHAR, 'L'))
    && INSIDE_CON(gi->mouse.cx, gi->mouse.cy))
      DO_TIMES(10)
        CL_CmdSpawnUnit(
          gi, 
          c, 
          gi->mouse.cx+cl->viewport_x, 
          gi->mouse.cy+cl->viewport_y,
          AW_unit_type_O);
    if(gi->lbtn_up
    && cl->attack_here) {
      if(INSIDE_MINI(gi->mouse.cx, gi->mouse.cy))
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          (gi->mouse.x-MINIMAP_START_X)*MINIMAP_INV_RATIO_X, 
          (gi->mouse.y-MINIMAP_START_Y)*MINIMAP_INV_RATIO_Y,
          GI_IsKeyPressed2(gi, TCODK_SHIFT));
      else
      if(INSIDE_CON(gi->mouse.cx, gi->mouse.cy))
        CL_CmdUnitOrderOnSelection(
          gi, 
          c, 
          gi->mouse.cx+cl->viewport_x, 
          gi->mouse.cy+cl->viewport_y,
          GI_IsKeyPressed2(gi, TCODK_SHIFT));
      cl->attack_here = false;
    }
    if(GI_IsKeyReleased(gi, TCODK_CHAR, 'a')
    || GI_IsKeyReleased(gi, TCODK_CHAR, 'q'))
      cl->attack_here = true;
    if(gi->rbtn_up) {
      if(cl->attack_here)
        cl->attack_here = false;
      else
        if(INSIDE_MINI(gi->mouse.cx, gi->mouse.cy))
          CL_CmdUnitOrderOnSelection(
            gi, 
            c, 
            (gi->mouse.x-MINIMAP_START_X)*MINIMAP_INV_RATIO_X, 
            (gi->mouse.y-MINIMAP_START_Y)*MINIMAP_INV_RATIO_Y,
            GI_IsKeyPressed2(gi, TCODK_SHIFT));
        else
        if(INSIDE_CON(gi->mouse.cx, gi->mouse.cy))
          CL_CmdUnitOrderOnSelection(
            gi, 
            c, 
            gi->mouse.cx+cl->viewport_x, 
            gi->mouse.cy+cl->viewport_y,
            GI_IsKeyPressed2(gi, TCODK_SHIFT));
    }
    if(GI_IsKeyReleased(gi, TCODK_TAB))
      CL_SelectNextSubGroup(gi, c);
  } else {
    if(cl->window_opened
    && gi->lbtn_down
    && INSIDE_CON(gi->mouse.cx, gi->mouse.cy)
    && (gi->mouse.cx < cl->window_start_x 
    || gi->mouse.cx > cl->window_end_x
    || gi->mouse.cy < cl->window_start_y 
    || gi->mouse.cy > cl->window_end_y))
      cl->window_opened = false;
  }
  #if !RELEASE
  if(GI_IsKeyReleased(gi, TCODK_F3))
    gi->show_all = !gi->show_all;
  if(GI_IsKeyReleased(gi, TCODK_F4))
    gi->show_lagometer = !gi->show_lagometer;
  #endif
  if(GI_IsKeyReleased(gi, TCODK_F6))
    gi->render_bloom = !gi->render_bloom;
}

void CL_UpdateHUD(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  /* HUD abilities */
  if(cl->selected_sub_group != -1
  && gi->get_abilities_count_cb) {
    int ability_count            = gi->get_abilities_count_cb(gi, c, cl->selected_sub_group);
    gi->abilities_btns.btn_count = MIN(ABILITIES_PER_LIST, ability_count);
    /* pos_x is the center of the string */
    gi->abilities_btns.pos_x     = START_ABILITIES_X;
    gi->abilities_btns.pos_y     = HUD_START_Y;
    DO_TIMES(gi->abilities_btns.btn_count) {
      gi->abilities_btns.btns[f].color             = TCOD_white;
      gi->abilities_btns.btns[f].s                 = gi->get_ability_name_cb(gi, c, cl->selected_sub_group, f);
      gi->abilities_btns.btns[f].highlight         = true;
      gi->abilities_btns.btns[f].entry             = false;
      gi->abilities_btns.btns[f].shortcut          = gi->get_ability_shortcut_cb(gi, c, cl->selected_sub_group, f);
      gi->abilities_btns.btns[f].shortcut_special  = (TCOD_keycode_t)-1;
      if(BTN_IsClicked(gi, &gi->abilities_btns.btns[f])) {
        AW_unit_ptr u = cl->selected_units_head;
        while(u != AW_null
           && unit(u).unit_type >= cl->selected_sub_group) {
          if(unit(u).unit_type == cl->selected_sub_group)
            gi->trigger_ability_cb(gi, c, u, f);
          u = unit(u).snext[c];
        }
      }
    }
    BTN_Render(gi, &gi->abilities_btns, false, 1); 
  }
  /* HUD main information */
  if(gi->hud_info_cb)
    gi->hud_info_cb(gi, c);
}

void CL_RenderPointer(AW_game_instance_t *gi, AW_client_ptr c, short x, short y, bool and_unit) {
  AW_client_t *cl = &client(c);
  AW_player_ptr cl_p = GI_GetPlayerPtr(gi, cl->player_id);
  AW_player_t *cl_pl = &player(cl_p);
  if(INSIDE_CON(x, y)) {
    AW_unit_ptr u = AW_null;
    bool vis = PL_IsInFov(gi, cl_p, (x+cl->viewport_x)>>RANGE_SHIFT, (y+cl->viewport_y)>>RANGE_SHIFT);
    if(INSIDE_MAP(x+cl->viewport_x, y+cl->viewport_y)
    && vis)
      u = AT(unit_map, x+cl->viewport_x, y+cl->viewport_y);
    if(u != AW_null) {
      if(and_unit) {
        AW_unit_t *un = &unit(u);
        AW_id_t team_id = TEAM_ID(un->player_id);
        TCOD_color_t c = 
          cl->attack_here ? TCOD_red : 
          un->player_id == game_desc.local_player_id ? TCOD_blue : 
          team_id == cl_pl->team_id ? TCOD_light_blue : TCOD_red;
        SOCLE(SIZE(un)) {
          short cx = un->pos_x+i-cl->viewport_x,
                cy = un->pos_y+j-cl->viewport_y;
          if(INSIDE_CON(cx, cy))
            TCOD_console_set_char_background(
              con, 
              cx, 
              cy, 
              c, 
              TCOD_BKGND_SET);
        }
      }
    } else {
      TCOD_console_set_char_background(
        con, 
        x, 
        y, 
        cl->attack_here ? TCOD_red : TCOD_blue, 
        TCOD_BKGND_SET);
    }
  }
}

void CL_RenderCursor(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  // cursor.
  CL_RenderPointer(gi, c, gi->mouse.cx, gi->mouse.cy, false);
}

void CL_RenderSelection(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  if(cl->selection_started) {
    int s_cx = cl->mp_lb_x, 
        s_cy = cl->mp_lb_y,
        e_cx = gi->mouse.cx+cl->viewport_x, 
        e_cy = gi->mouse.cy+cl->viewport_y,
        x1 = s_cx, y1 = e_cy,
        x2 = e_cx, y2 = s_cy;
    IN_ORDER(s_cx, s_cy, e_cx, e_cy)
    FOR_RECT(s_cx, e_cx, s_cy, e_cy)
      if(INSIDE_MAP(i, j)) 
        CL_RenderPointer(gi, c, i-cl->viewport_x, j-cl->viewport_y, true);
  }
}

void CL_RenderSmokes(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  FOR_RECT(
    cl->viewport_x, 
    cl->viewport_x+CON_RES_X,
    cl->viewport_y, 
    cl->viewport_y+CON_RES_Y) 
    AT(smoke_map, i, j) = 0.f;
  AW_smoke_ptr s = gi->smoke_head;
  while(s != AW_null) {
    AW_smoke_t *sm = &smoke(s);
    FOR_RECT(
      sm->pos_x-sm->d,
      sm->pos_x+sm->d-1,
      sm->pos_y-sm->d,
      sm->pos_y+sm->d-1) 
      AT(smoke_map, i, j) = MAX(AT(smoke_map, i, j),
                            SATURATE(1.f
                          - (float)((i-sm->pos_x) * (i-sm->pos_x)
                          + (j-sm->pos_y) * (j-sm->pos_y)) 
                          / (sm->d * sm->d))
                          * (1.f - (float)ABS(sm->span - SMOKE_LIFE_SPAN/2)/(SMOKE_LIFE_SPAN/2)));
    s = sm->next;
  }
}

void CL_RenderBlood(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  AW_player_ptr p = GI_GetPlayerPtr(gi, cl->player_id);
  AW_blood_ptr b = gi->blood_head;
  while(b != AW_null) {
    AW_blood_t *bl = &blood(b);
    int i = bl->pos_x-cl->viewport_x,
        j = bl->pos_y-cl->viewport_y;
    bool vis = PL_IsInFov(gi, p, bl->pos_x>>RANGE_SHIFT, bl->pos_y>>RANGE_SHIFT);
    if(INSIDE_CON(i, j)
    && AT(unit_map, bl->pos_x, bl->pos_y) == AW_null
    && vis) {
      int light = PL_GetLight(gi, p, bl->pos_x>>RANGE_SHIFT, bl->pos_y>>RANGE_SHIFT);
      TCOD_color_t c = TCOD_console_get_char_foreground(con, i, j);
      TCOD_console_set_char_foreground(
        con,
        i,
        j,
        TCOD_color_lerp(c, bl->c, (float)0.5f*pow((float)bl->span/BLOOD_LIFE_SPAN, 0.3)*light/255));
    }
    b = bl->next;
  }
}

void CL_RenderFloatingText(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  AW_player_ptr p = GI_GetPlayerPtr(gi, cl->player_id);
  AW_floating_text_ptr ft = gi->floating_text_head;
  while(ft != AW_null) {
    AW_floating_text_t *flt = &floating_text(ft);
    int i     = flt->pos_x-cl->viewport_x,
        j     = flt->pos_y-cl->viewport_y,
        half  = flt->s.size()/2;
    if(INSIDE_CON(i-half, j)
    && INSIDE_CON(i+half, j)) {
      DO_TIMES(flt->s.size())
        gi->post_render_mask[CON_RES_X*j+i+f-half] = gi->frame_mark;
      TCOD_color_t c = TCOD_console_get_char_background(con, i, j);
      TCOD_console_set_default_foreground(
        con, 
        TCOD_color_lerp(c, flt->c, (float)flt->span/FLOATING_TEXT_LIFE_SPAN));
      AW_PrintString(flt->s, i-half, j);
    }
    ft = flt->next;
  }
}

void CL_RenderEnvMap(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  AW_player_ptr p = GI_GetPlayerPtr(gi, cl->player_id);
  AW_player_t *pl = &player(p);
  gi->fog_scroll_x -= (float)gi->game_time_step * 0.00025;
  gi->fog_scroll_y += (float)gi->game_time_step * 0.00025;
  FOR_RECT(
    cl->viewport_x, 
    cl->viewport_x+CON_RES_X-1, 
    cl->viewport_y, 
    cl->viewport_y+CON_RES_Y-1)
    if(INSIDE_MAP(i, j)
    && INSIDE_CON(i-cl->viewport_x, j-cl->viewport_y)) {
      int light = PL_GetLight(gi, p, i>>RANGE_SHIFT, j>>RANGE_SHIFT);
      #if !RELEASE
      if(gi->show_all)
        light = 255;
      #endif
      if(AT(env_art_map, i, j) == AW_WATER
      || AT(env_art_map, i, j) == AW_WATER2) {
        int x = i,
            y = j;
        float v[4];
        FOR_RECT(0, 1, 0, 1) {
          float coords[2];
          coords[0] = (float)(x+i)*25/MAP_SIZE_X+gi->fog_scroll_x*2;
          coords[1] = 100000-(float)(y+j)*25/MAP_SIZE_Y+gi->fog_scroll_y*2;
          v[(j<<1)+i] = 0.5f + 0.5f * TCOD_noise_get(gi->bump_noise, coords);  
        }
        float N[3],
              L[3];
        L[0] = -1; L[1] = -1; L[2] = 1;
        N[0] = (v[0] - v[1])*6; N[1] = (v[0] - v[2])*6; N[2] = 1;
        float L_len = sqrt(L[0]*L[0] + L[1]*L[1] + L[2]*L[2]),
              N_len = sqrt(N[0]*N[0] + N[1]*N[1] + N[2]*N[2]);
        DO_TIMES(3) {
          L[f] /= L_len;
          N[f] /= N_len;
          AT(normal_map, x, y)[f] = N[f];
          gi->dir_light[f] = L[f];
        }
        float dot = N[0] * L[0] + N[1] * L[1] + N[2] * L[2];
        AT(static_light_map, x, y) = 
          TCOD_color_add(
            AMBIENT_LIGHTMAP, 
            TCOD_color_multiply_scalar(
              TCOD_color_multiply_scalar(
                DIR_LIGHTMAP_COLOR, 
                (float)AT(shadow_map, x, y)), 
              SATURATE(dot)));
      }
      TCOD_color_t l = 
        TCOD_color_add(AT(static_light_map, i, j), AT(dyn_light_map, i, j)),
                   c = AT(env_color, i, j);
      int r = ((c.r * l.r)>>8)&0xff, 
          g = ((c.g * l.g)>>8)&0xff,
          b = ((c.b * l.b)>>8)&0xff;
      /*r = MAX(TCOD_darker_grey.r, r);
      g = MAX(TCOD_darker_grey.g, g);
      b = MAX(TCOD_darker_grey.b, b);*/
      c = TCOD_color_RGB(r, g, b);
      float smoke = AT(smoke_map, i, j) * SMOKE_STRENGTH;
      TCOD_color_t fog_mask = 
        TCOD_color_add(
          TCOD_color_RGB(255-light, 255-light, 255-light),
          TCOD_color_RGB(255*smoke, 255*smoke, 255*smoke)); //TCOD_image_get_pixel(gi->fog_mask, i, j);
      int fog = 0;
      bool is_wall = AT_ENV(i>>RANGE_SHIFT, j>>RANGE_SHIFT) == AW_WALL;
      if(fog_mask.r > 0
      && !is_wall) {
        float coords[2];
        coords[0] = (float)i*50/MAP_SIZE_X+gi->fog_scroll_x;
        coords[1] = (float)j*50/MAP_SIZE_Y+gi->fog_scroll_y;
        fog = (float)(0.5f + 0.5f * TCOD_noise_get_fbm(gi->fog_noise, coords, 4))*0.16f*light*fog_mask.r/255;
      }
      /* spec */
      float H[3], V[3];
      V[0] = -(i - (cl->viewport_x + CON_RES_X/2));
      V[1] = -(j - (cl->viewport_y + CON_RES_Y/2));
      V[2] = 20;
      float V_len = sqrt(V[0]*V[0] + V[1]*V[1] + V[2]*V[2]);
      float *N = AT(normal_map, i, j);
      float H_len = 0;
      DO_TIMES(3) {
        V[f] /= V_len;
        H[f] = gi->dir_light[f] + V[f];
        H_len += H[f] * H[f];
      }
      H_len = sqrt(H_len);
      float LdotN = N[0] * gi->dir_light[0] + N[1] * gi->dir_light[1] + N[2] * gi->dir_light[2];
      float R[3],
            R_len = 0;
      DO_TIMES(3) {
        R[f] = 2.f*LdotN*N[f] - gi->dir_light[f];
        R_len += R[f] * R[f];
      }
      R_len = sqrt(R_len);
      DO_TIMES(3) {
        H[f] /= H_len;
        R[f] /= R_len;
      }
      float spec_f, spec_exp;
      short env_char = AT(env_art_map, i, j);
      if(env_char == AW_BUSH1
      || env_char == AW_BUSH2
      || env_char == AW_TREE1
      || env_char == AW_TREE2
      || env_char == AW_TREE3
      || env_char == AW_MUSH
      || env_char == AW_TORCH
      || env_char == AW_EMPTY) {
        spec_f    = 0.f;
        spec_exp  = 1.f;
      } else
      if(env_char == AW_WATER
      || env_char == AW_WATER2) {
        spec_f    = 1.f;
        spec_exp  = 10.f;
      } else
      if(env_char == AW_TILE2) {
        spec_f    = 0.75f;
        spec_exp  = 50.f;
      } else
      if(env_char == AW_PLANK
      || env_char == AW_PLANK2) {
        spec_f    = 0.65f;
        spec_exp  = 1.f;
      } else
      if(env_char == AW_WALL
      || env_char == AW_TILE
      || env_char == AW_GRASS) {
        spec_f    = 1.f;
        spec_exp  = 1.f;
      }
      float spec = SATURATE(V[0] * R[0] + V[1] * R[1] + V[2] * R[2]);
      spec = pow(spec, spec_exp) * (float)AT(shadow_map, i, j);
      /* colors */
      TCOD_color_t c1 = TCOD_color_add(
                          TCOD_color_multiply_scalar(c, (float)light/255), 
                          TCOD_color_multiply_scalar(TCOD_color_RGB(75, 75, 75), spec_f*spec*(float)light/255)),
                   c2 = TCOD_color_add(
                          TCOD_color_multiply_scalar(TCOD_color_RGB(10, 10, 10), (float)light/255),
                          TCOD_color_multiply(TCOD_color_RGB(fog, fog, fog), l));
      TCOD_console_put_char_ex(
        con, 
        i-cl->viewport_x, 
        j-cl->viewport_y, 
        AT(env_art_map, i, j), 
        is_wall ? c2 : c1, 
        is_wall ? c1 : c2);
  }
}

void CL_RenderUnits(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  // all units.
  AW_player_ptr p = GI_GetPlayerPtr(gi, cl->player_id);
  AW_player_t *pl = &player(p);
  AW_player_ptr p2 = gi->player_head;
  while(p2 != AW_null) {
    AW_player_t *pl2 = &player(p2);
    AW_unit_ptr u2 = pl2->unit_head;
    while(u2 != AW_null) {
      AW_unit_t *un2 = &unit(u2);
      AW_id_t team_id2 = TEAM_ID(un2->player_id);
      bool vis = PL_IsInFov(gi, p, u2);
      #if !RELEASE
      if(gi->show_all)
        vis = true;
      #endif
      if(vis 
      && (un2->blink_acc <= 0
      || (gi->frame_mark&15) < 8)) {
        SOCLE(SIZE(un2)) {
          if(INSIDE_CON(
              un2->pos_x+i-cl->viewport_x,
              un2->pos_y+j-cl->viewport_y)) {
            int light = PL_GetLight(gi, p, (un2->pos_x+i)>>RANGE_SHIFT, (un2->pos_y+j)>>RANGE_SHIFT);
            TCOD_color_t c = pl->team_id == team_id2 ? BLUE : RED,
                         l = TCOD_color_add(
                              AT(static_light_map, un2->pos_x+i, un2->pos_y+j), 
                              AT(dyn_light_map, un2->pos_x+i, un2->pos_y+j));
            int r = ((c.r * l.r)>>8)&0xff, 
                g = ((c.g * l.g)>>8)&0xff,
                b = ((c.b * l.b)>>8)&0xff;
            /*r = MAX(TCOD_darker_grey.r, r);
            g = MAX(TCOD_darker_grey.g, g);
            b = MAX(TCOD_darker_grey.b, b);*/
            c = TCOD_color_RGB(r, g, b);
            float smoke = AT(smoke_map, un2->pos_x+i, un2->pos_y+j) * SMOKE_STRENGTH;
            TCOD_color_t fog_mask = 
              TCOD_color_add(
                TCOD_color_RGB(255-light, 255-light, 255-light),
                TCOD_color_RGB(255*smoke, 255*smoke, 255*smoke)); //TCOD_image_get_pixel(gi->fog_mask, i, j);
            int fog = 0;
            if(fog_mask.r > 0) {
              float coords[2];
              coords[0] = (float)(un2->pos_x+i)*50/MAP_SIZE_X+gi->fog_scroll_x;
              coords[1] = (float)(un2->pos_y+j)*50/MAP_SIZE_Y+gi->fog_scroll_y;
              fog = (float)(0.5f + 0.5f * TCOD_noise_get_fbm(gi->fog_noise, coords, 4))*0.16f*light*fog_mask.r/255;
            }
            /* spec */
            float H[3], V[3];
            V[0] = -((un2->pos_x+i) - (cl->viewport_x + CON_RES_X/2));
            V[1] = -((un2->pos_y+j) - (cl->viewport_y + CON_RES_Y/2));
            V[2] = 20;
            float V_len = sqrt(V[0]*V[0] + V[1]*V[1] + V[2]*V[2]);
            float N[3] = {0, 0, 1};
            float H_len = 0;
            DO_TIMES(3) {
              V[f] /= V_len;
              H[f] = gi->dir_light[f] + V[f];
              H_len += H[f] * H[f];
            }
            H_len = sqrt(H_len);
            float LdotN = N[0] * gi->dir_light[0] + N[1] * gi->dir_light[1] + N[2] * gi->dir_light[2];
            float R[3],
                  R_len = 0;
            DO_TIMES(3) {
              R[f] = 2.f*LdotN*N[f] - gi->dir_light[f];
              R_len += R[f] * R[f];
            }
            R_len = sqrt(R_len);
            DO_TIMES(3) {
              H[f] /= H_len;
              R[f] /= R_len;
            }
            float spec = SATURATE(V[0] * R[0] + V[1] * R[1] + V[2] * R[2]);
            spec = pow(spec, 10) * (float)AT(shadow_map, un2->pos_x+i, un2->pos_y+j);
            TCOD_console_put_char_ex(
              con, 
              un2->pos_x+i-cl->viewport_x, 
              un2->pos_y+j-cl->viewport_y, 
              ASCII(un2)[SIZE(un2)*(j+_half)+(i+_half)], 
              TCOD_color_add(
                TCOD_color_multiply_scalar(c, (float)light/255), 
                TCOD_color_multiply_scalar(TCOD_color_RGB(75, 75, 75), 1.f*spec*(float)light/255)),
              TCOD_color_add(
                TCOD_color_multiply_scalar(TCOD_color_RGB(20, 20, 20), (float)light/255),
                TCOD_color_add(
                  TCOD_color_multiply(TCOD_color_RGB(fog, fog, fog), l),
                  TCOD_color_multiply_scalar(TCOD_color_RGB(75, 75, 75), 1.f*spec*(float)light/255))));
          }
        }
      }
      u2 = un2->pnext;
    }
    /* dir shadows */
    u2 = pl2->unit_head;
    while(u2 != AW_null) {
      AW_unit_t *un2 = &unit(u2);
      AW_id_t team_id2 = TEAM_ID(un2->player_id);
      bool vis = PL_IsInFov(gi, p, u2);
      #if !RELEASE
      if(gi->show_all)
        vis = true;
      #endif
      if(vis 
      && (un2->blink_acc <= 0
      || (gi->frame_mark&15) < 8)) {
        SOCLE(SIZE(un2)) {
          int x = un2->pos_x+i-cl->viewport_x,
              y = un2->pos_y+j-cl->viewport_y,
              /* dir light is diag at the moment */
              x1 = x + SIZE(un2),
              y1 = y + SIZE(un2);
          TCOD_line_init(x, y, x1, y1);
          do {
            int x_env = x+cl->viewport_x,
                y_env = y+cl->viewport_y;
            AW_unit_ptr u3 = AT(unit_map, x_env, y_env);
            if(INSIDE_MAP(x, y)
            && u3 == AW_null
            && AT_ENV(x_env>>RANGE_SHIFT, y_env>>RANGE_SHIFT) 
            != AW_WALL) {
              int light = PL_GetLight(gi, p, x_env>>RANGE_SHIFT, y_env>>RANGE_SHIFT);
              TCOD_color_t c = AT(env_color, x_env, y_env),
                           l = AMBIENT_LIGHTMAP;
              int r = ((c.r * l.r)>>8)&0xff, 
                  g = ((c.g * l.g)>>8)&0xff,
                  b = ((c.b * l.b)>>8)&0xff;
              /*r = MAX(TCOD_darker_grey.r, r);
              g = MAX(TCOD_darker_grey.g, g);
              b = MAX(TCOD_darker_grey.b, b);*/
              c = TCOD_color_RGB(r, g, b);
              if(INSIDE_CON(x,y))
                TCOD_console_set_char_foreground(con, x, y, TCOD_color_multiply_scalar(c, (float)light/255));
            }
          } while(!TCOD_line_step(&x, &y));
        }
        /* AO */
        DO_TIMES(SIZE(un2)+2) {
          int x_env = un2->pos_x-halfes[SIZE(un2)-1]-1+f,
              y_env = un2->pos_y-halfes[SIZE(un2)-1]-1,
              x = x_env-cl->viewport_x,
              y = y_env-cl->viewport_y;
          if(INSIDE_CON(x, y)
          && AT(unit_map, x_env, y_env) == AW_null
          && AT_ENV(x_env>>RANGE_SHIFT, y_env>>RANGE_SHIFT) 
          != AW_WALL) {
            int light = PL_GetLight(gi, p, x_env>>RANGE_SHIFT, y_env>>RANGE_SHIFT);
            TCOD_color_t c = TCOD_console_get_char_foreground(con, x, y);
            TCOD_console_set_char_foreground(con, x, y, TCOD_color_multiply_scalar(c, (float)UNIT_AO_STRENGTH));
          }
          x_env = un2->pos_x-halfes[SIZE(un2)-1]-1+f,
          y_env = un2->pos_y-halfes[SIZE(un2)-1]+SIZE(un2),
          x = x_env-cl->viewport_x,
          y = y_env-cl->viewport_y;
          if(INSIDE_CON(x, y)
          && AT(unit_map, x_env, y_env) == AW_null
          && AT_ENV(x_env>>RANGE_SHIFT, y_env>>RANGE_SHIFT) 
          != AW_WALL) {
            int light = PL_GetLight(gi, p, x_env>>RANGE_SHIFT, y_env>>RANGE_SHIFT);
            TCOD_color_t c = TCOD_console_get_char_foreground(con, x, y);
            TCOD_console_set_char_foreground(con, x, y, TCOD_color_multiply_scalar(c, (float)UNIT_AO_STRENGTH));
          }
        }
        DO_TIMES(SIZE(un2)) {
          int x_env = un2->pos_x-halfes[SIZE(un2)-1]-1,
              y_env = un2->pos_y-halfes[SIZE(un2)-1]+f,
              x = x_env-cl->viewport_x,
              y = y_env-cl->viewport_y;
          if(INSIDE_CON(x, y)
          && AT(unit_map, x_env, y_env) == AW_null
          && AT_ENV(x_env>>RANGE_SHIFT, y_env>>RANGE_SHIFT) 
          != AW_WALL) {
            int light = PL_GetLight(gi, p, x_env>>RANGE_SHIFT, y_env>>RANGE_SHIFT);
            TCOD_color_t c = TCOD_console_get_char_foreground(con, x, y);
            TCOD_console_set_char_foreground(con, x, y, TCOD_color_multiply_scalar(c, (float)UNIT_AO_STRENGTH));
          }
          x_env = un2->pos_x-halfes[SIZE(un2)-1]+SIZE(un2),
          y_env = un2->pos_y-halfes[SIZE(un2)-1]+f,
          x = x_env-cl->viewport_x,
          y = y_env-cl->viewport_y;
          if(INSIDE_CON(x, y)
          && AT(unit_map, x_env, y_env) == AW_null
          && AT_ENV(x_env>>RANGE_SHIFT, y_env>>RANGE_SHIFT) 
          != AW_WALL) {
            int light = PL_GetLight(gi, p, x_env>>RANGE_SHIFT, y_env>>RANGE_SHIFT);
            TCOD_color_t c = TCOD_console_get_char_foreground(con, x, y);
            TCOD_console_set_char_foreground(con, x, y, TCOD_color_multiply_scalar(c, (float)UNIT_AO_STRENGTH));
          }
        }
      }
      u2 = un2->pnext;
    }
    p2 = pl2->next;
  }
}

void CL_RenderBloom(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  if(gi->render_bloom) {
    int r, g, b, k;
    FOR_RECT(0, CON_RES_X-1, 0, CON_RES_Y-1) 
      if(INSIDE_CON(i, j)) {
        TCOD_color_t c = TCOD_console_get_char_foreground(con, i, j);
        float v = (float)(c.r+c.g+c.b)/(3*255);
        float bloom_f;
        short env_char = AT(env_art_map, i+cl->viewport_x, j+cl->viewport_y);
        if(env_char == AW_BUSH1
        || env_char == AW_BUSH2
        || env_char == AW_TREE1
        || env_char == AW_TREE2
        || env_char == AW_TREE3
        || env_char == AW_MUSH
        || env_char == AW_TORCH
        || env_char == AW_EMPTY
        || env_char == AW_GRASS)
          bloom_f    = 0.f;
        else
        if(env_char == AW_TILE)
          bloom_f    = 1.33f;
        else
        if(env_char == AW_TILE2)
          bloom_f    = 1.f;
        else
        if(env_char == AW_WATER
        || env_char == AW_WATER2
        || env_char == AW_WALL
        || env_char == AW_PLANK
        || env_char == AW_PLANK2)
          bloom_f    = 1.f;
        gi->glow_map[CON_RES_X*j+i] = v*bloom_f > 0.85f ? c : TCOD_black;
      }
    FOR_RECT(0, CON_RES_X-1, 0, CON_RES_Y-1) 
      if(INSIDE_CON(i, j)) {
        r = g = b = k = 0;
        DO_TIMES(5) 
          if(INSIDE_CON(i+f-2, j)) {
            TCOD_color_t c = gi->glow_map[CON_RES_X*j+(i+f-2)];
            r += c.r; g += c.g; b += c.b;
            k++;
          }
        r /= k; g /= k; b /= k;
        gi->glow_map2[CON_RES_X*j+i] = TCOD_color_RGB(r, g, b);
      }
    FOR_RECT(0, CON_RES_X-1, 0, CON_RES_Y-1) 
      if(INSIDE_CON(i, j)) {
        r = g = b = k = 0;
        DO_TIMES(5) 
          if(INSIDE_CON(i, j+f-2)) {
            TCOD_color_t c = gi->glow_map2[CON_RES_X*(j+f-2)+i];
            r += c.r; g += c.g; b += c.b;
            k++;
          }
        r /= k; g /= k; b /= k;
        gi->glow_map[CON_RES_X*j+i] = TCOD_color_RGB(r, g, b);
      }
    FOR_RECT(0, CON_RES_X-1, 0, CON_RES_Y-1)
      if(INSIDE_CON(i, j)) {
        TCOD_color_t c = TCOD_console_get_char_background(con, i, j);
        TCOD_console_set_char_background(
          con, 
          i, 
          j, 
          TCOD_color_add(
            c, 
            TCOD_color_multiply_scalar(gi->glow_map[CON_RES_X*j+i], 0.2f)),
          TCOD_BKGND_SET);
      }
  }
}

void CL_RenderBalls(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  AW_player_ptr p = GI_GetPlayerPtr(gi, cl->player_id);
  AW_player_t *pl = &player(p);
  AW_ball_ptr b   = gi->ball_head;
  while(b != AW_null) {
    AW_ball_t *ba = &ball(b);
    if(AT(unit_map, ba->pos_x, ba->pos_y) == AW_null) {
      bool vis = PL_IsInFov(gi, p, ba->pos_x>>RANGE_SHIFT, ba->pos_y>>RANGE_SHIFT);
      #if !RELEASE
      if(gi->show_all)
        vis = true;
      #endif
      if(vis
      && INSIDE_CON(
          ba->pos_x-cl->viewport_x,
          ba->pos_y-cl->viewport_y)) {
        int light = PL_GetLight(gi, p, ba->pos_x>>RANGE_SHIFT, ba->pos_y>>RANGE_SHIFT);
        TCOD_color_t c = pl->team_id == player(ba->p).team_id ? BLUE : RED,
                     l = TCOD_color_add(
                          AT(static_light_map, ba->pos_x, ba->pos_y), 
                          AT(dyn_light_map, ba->pos_x, ba->pos_y));
        float smoke = AT(smoke_map, ba->pos_x, ba->pos_y) * SMOKE_STRENGTH;
        TCOD_color_t fog_mask = 
          TCOD_color_add(
            TCOD_color_RGB(255-light, 255-light, 255-light),
            TCOD_color_RGB(255*smoke, 255*smoke, 255*smoke)); //TCOD_image_get_pixel(gi->fog_mask, i, j);
        int fog = 0;
        if(fog_mask.r > 0) {
          float coords[2];
          coords[0] = (float)ba->pos_x*50/MAP_SIZE_X+gi->fog_scroll_x;
          coords[1] = (float)ba->pos_y*50/MAP_SIZE_Y+gi->fog_scroll_y;
          fog = (float)(0.5f + 0.5f * TCOD_noise_get_fbm(gi->fog_noise, coords, 4))*0.16f*light*fog_mask.r/255;
        }
        /* spec */
        float H[3], V[3];
        V[0] = -((ba->pos_x) - (cl->viewport_x + CON_RES_X/2));
        V[1] = -((ba->pos_y) - (cl->viewport_y + CON_RES_Y/2));
        V[2] = 20;
        float V_len = sqrt(V[0]*V[0] + V[1]*V[1] + V[2]*V[2]);
        float N[3] = {0, 0, 1};
        float H_len = 0;
        DO_TIMES(3) {
          V[f] /= V_len;
          H[f] = gi->dir_light[f] + V[f];
          H_len += H[f] * H[f];
        }
        H_len = sqrt(H_len);
        float LdotN = N[0] * gi->dir_light[0] + N[1] * gi->dir_light[1] + N[2] * gi->dir_light[2];
        float R[3],
              R_len = 0;
        DO_TIMES(3) {
          R[f] = 2.f*LdotN*N[f] - gi->dir_light[f];
          R_len += R[f] * R[f];
        }
        R_len = sqrt(R_len);
        DO_TIMES(3) {
          H[f] /= H_len;
          R[f] /= R_len;
        }
        float spec = SATURATE(V[0] * R[0] + V[1] * R[1] + V[2] * R[2]);
        spec = pow(spec, 10) * (float)AT(shadow_map, ba->pos_x, ba->pos_y);
        TCOD_console_put_char_ex(
          con, 
          ba->pos_x-cl->viewport_x, 
          ba->pos_y-cl->viewport_y, 
          '0', 
          TCOD_color_add(
            TCOD_color_multiply_scalar(c, (float)light/255), 
            TCOD_color_multiply_scalar(TCOD_color_RGB(75, 75, 75), 1.f*spec*(float)light/255)),
          TCOD_color_add(
            TCOD_color_multiply_scalar(TCOD_color_RGB(20, 20, 20), (float)light/255),
            TCOD_color_add(
              TCOD_color_multiply(TCOD_color_RGB(fog, fog, fog), l),
              TCOD_color_multiply_scalar(TCOD_color_RGB(75, 75, 75), 1.f*spec*(float)light/255))));
      }
    }
    b = ba->next;
  }
}

void CL_RenderWinLose(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  AW_player_ptr p = GI_GetPlayerPtr(gi, cl->player_id);
  AW_player_t *pl = &player(p);
  if(pl->lose || pl->win) {
    str s;
    if(pl->win) {
      TCOD_console_set_default_foreground(con, TCOD_green);
      s = "You win!";
    } else
    if(pl->lose) {
      TCOD_console_set_default_foreground(con, TCOD_dark_red);
      s = "You lose...";
    }
    AW_PrintString(s, CON_RES_X/2-s.length()/2, CON_RES_Y/2);
  }
}

void CL_RenderWindow(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  if(cl->window_opened) {
    FOR_RECT(cl->window_start_x, cl->window_end_x, cl->window_start_y, cl->window_end_y) {
      TCOD_color_t c = TCOD_console_get_char_background(con, i, j);
      TCOD_console_set_char_background(con, i, j, TCOD_color_multiply_scalar(c, 0.25f), TCOD_BKGND_SET);
      c = TCOD_console_get_char_foreground(con, i, j);
      TCOD_console_set_char_foreground(con, i, j, TCOD_color_multiply_scalar(c, 0.25f));
      gi->post_render_mask[CON_RES_X*j+i] = gi->frame_mark;
      BTN_Render(gi, &gi->close_btn);
    }
  }
}

void CL_RenderFps(AW_game_instance_t *gi) {
  float fps = TCOD_sys_get_fps();
  TCOD_console_set_default_foreground(con, TCOD_white);
  AW_PrintString(i2a(fps), CON_RES_X-3, CON_RES_Y-1);
}

void CL_RenderPing(AW_game_instance_t *gi) {
  TCOD_console_set_default_foreground(con, TCOD_white);
  AW_PrintString(i2a(gi->max_ping), 0, CON_RES_Y-1);
}

void CL_RenderStats(AW_game_instance_t *gi) {
  TCOD_console_set_default_foreground(con, TCOD_white);
  #if !RELEASE
    AW_PrintString(f2a(gi->turn_frame_count), CON_RES_X-30, CON_RES_Y-1);
    AW_PrintString(f2a(gi->frame_time_step), CON_RES_X-45, CON_RES_Y-1);
    AW_PrintString(f2a(gi->aver_wait_time), CON_RES_X-55, CON_RES_Y-1);
    if(gi->desync)
      AW_PrintString("desynced", CON_RES_X-45, CON_RES_Y-3);
    #if REPLAY
    if(gi->turn > gi->max_turn)
      AW_PrintString("the end", CON_RES_X-45, CON_RES_Y-3);
    #endif
  #endif
}

bool CL_IsUnitSelected(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u, short gs) {
  AW_client_t *cl = &client(c);
  if(gs == -1) {
    AW_unit_ptr u2 = cl->selected_units_head;
    while(u2 != AW_null) {
      if(u == u2) 
        return true;
      u2 = unit(u2).snext[c];
    }
    return false;
  } 
  AW_unit_ptr u2 = cl->gs_head[gs];
  while(u2 != AW_null) {
    if(u == u2) 
      return true;
    u2 = unit(u2).gsnext[gs];
  }
  return false;
}

short CL_GetSelectedUnitsCount(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  AW_unit_ptr u = cl->selected_units_head;
  int i = 0;
  while(u != AW_null) {
    i++;
    u = unit(u).snext[c];
  }
  return i;
}

bool CL_IsThereUnitsHere(AW_game_instance_t *gi, AW_client_ptr c, int s_cx, int s_cy, int e_cx, int e_cy) {
  int x1 = s_cx, y1 = e_cy,
      x2 = e_cx, y2 = s_cy;
  IN_ORDER(s_cx, s_cy, e_cx, e_cy)
  FOR_RECT(s_cx, e_cx, s_cy, e_cy) 
    if(INSIDE_MAP(i, j)) {
      AW_unit_ptr u = AT(unit_map, i, j);
      if(u != AW_null 
      && TEAM_ID(unit(u).player_id) 
      == TEAM_ID(client(c).player_id))
        return true;
    }
  return false;
}

void CL_SelectUnits(AW_game_instance_t *gi, AW_client_ptr c, int s_cx, int s_cy, int e_cx, int e_cy, AW_unit_type_t type) {
  AW_client_t *cl = &client(c);
  int x1 = s_cx, y1 = e_cy,
      x2 = e_cx, y2 = s_cy;
  IN_ORDER(s_cx, s_cy, e_cx, e_cy)
  bool no_sub_group = cl->selected_sub_group == -1;
  FOR_RECT(s_cx, e_cx, s_cy, e_cy) 
    if(INSIDE_MAP(i, j)) {
      AW_unit_ptr u = AT(unit_map, i, j);
      if(u != AW_null 
      && TEAM_ID(unit(u).player_id) 
      == TEAM_ID(client(c).player_id)
      && (type == AW_unit_type_count
      || unit(u).unit_type == type))
        CL_AddToSelection(gi, c, u);
    }
  if(no_sub_group) {
    cl->selected_sub_group = 
      unit(cl->selected_units_head).unit_type;
    cl->hud_state = 0;
  }
}

void CL_AddToSelection(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u) {
  if(!CL_IsUnitSelected(gi, c, u)) {
    AW_client_t *cl = &client(c);
    AW_unit_t *un = &unit(u);
    if(cl->selected_sub_group == -1) {
      cl->selected_sub_group = un->unit_type;
      cl->hud_state = 0;
    }
    if(cl->selected_units_head == AW_null) {
      un->snext[c] = AW_null;
      un->sprevious[c] = AW_null;
      cl->selected_units_head = u;
    } else 
    if(unit(cl->selected_units_head).unit_type 
    <= un->unit_type) {
      un->snext[c] = cl->selected_units_head;
      unit(cl->selected_units_head).sprevious[c] = u;
      un->sprevious[c] = AW_null;
      cl->selected_units_head = u;
    } else {
      AW_unit_ptr u2 = cl->selected_units_head;
      while(unit(u2).snext[c] != AW_null
         && unit(unit(u2).snext[c]).unit_type > un->unit_type)
        u2 = unit(u2).snext[c];
      AW_unit_t *un2 = &unit(u2);
      if(un2->snext[c] == AW_null) {
        un->sprevious[c] = u2;
        un->snext[c] = AW_null;
        un2->snext[c] = u;
      } else {
        unit(un2->snext[c]).sprevious[c] = u;
        un->snext[c] = un2->snext[c];
        un2->snext[c] = u;
        un->sprevious[c] = u2;
      }
    }
  }
}

void CL_SubToSelection(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u, short gs) {
  AW_client_t     *cl = &client(c);
  if(gs == -1) {  
    AW_unit_ptr u2 = cl->selected_units_head;
    AW_unit_ptr previous = u2;
    bool done = false;
    while(!done 
       && u2 != AW_null) {
      if(u == u2) {
        if(previous == u2) {
          cl->selected_units_head =
            unit(cl->selected_units_head).snext[c];
        } else {
          unit(previous).snext[c] = unit(u2).snext[c];
          if(unit(u2).snext[c])
            unit(unit(u2).snext[c]).sprevious[c] = previous;
        }
        done = true;
      }
      previous = u2;
      u2 = unit(u2).snext[c];
    }
    assert(done);
    if(cl->selected_sub_group == unit(u).unit_type) {
      bool change = true;
      AW_unit_ptr u2 = cl->selected_units_head;
      while(u2 != AW_null) {
        if(unit(u2).unit_type 
        == cl->selected_sub_group)
          change = false;
        u2 = unit(u2).snext[c];
      }
      if(change) {
        if(cl->selected_units_head != AW_null)
          cl->selected_sub_group = 
            unit(cl->selected_units_head).unit_type;
        else
          cl->selected_sub_group = -1;
        cl->hud_state = 0;
      }
    }
  } else {
    AW_unit_ptr u2 = cl->gs_head[gs];
    AW_unit_ptr previous = u2;
    bool done = false;
    while(!done 
       && u2 != AW_null) {
      if(u == u2) {
        if(previous == u2) {
          cl->gs_head[gs] =
            unit(cl->gs_head[gs]).gsnext[gs];
        } else {
          unit(previous).gsnext[gs] = unit(u2).gsnext[gs];
          if(unit(u2).gsnext[gs])
            unit(unit(u2).gsnext[gs]).gsprevious[gs] = previous;
        }
        done = true;
      }
      previous = u2;
      u2 = unit(u2).gsnext[gs];
    }
    assert(done);
    if(cl->gs_sub_group[gs] == unit(u).unit_type) {
      bool change = true;
      AW_unit_ptr u2 = cl->gs_head[gs];
      while(u2 != AW_null) {
        if(unit(u2).unit_type 
        == cl->selected_sub_group)
          change = false;
        u2 = unit(u2).gsnext[gs];
      }
      if(change)
        if(cl->gs_head[gs] != AW_null)
          cl->gs_sub_group[gs] = 
            unit(cl->gs_head[gs]).unit_type;
        else
          cl->gs_sub_group[gs] = -1;
    }
  }
}

void CL_FreeUnitSelection(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  cl->selected_units_head = AW_null;
  cl->selected_sub_group  = -1;
  cl->hud_state           = 0;
}

void CL_AddToGroupSelection(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u, int gs) {
  AW_client_t *cl = &client(c);
  if(cl->gs_head[gs] != AW_null)
    unit(cl->gs_head[gs]).gsprevious[gs] = u;
  unit(u).gsprevious[gs] = AW_null;
  unit(u).gsnext[gs] = cl->gs_head[gs];
  cl->gs_head[gs] = u;
}

void CL_CopySelectionTo(AW_game_instance_t *gi, AW_client_ptr c, int gs) {
  AW_client_t *cl = &client(c);
  cl->gs_head[gs] = AW_null;
  AW_unit_ptr u = cl->selected_units_head;
  while(u != AW_null) {
    CL_AddToGroupSelection(gi, c, u, gs);
    u = unit(u).snext[c];
  }
  cl->gs_sub_group[gs] = cl->selected_sub_group;
}

void CL_RestoreGroupSelection(AW_game_instance_t *gi, AW_client_ptr c, int gs) {
  AW_client_t *cl = &client(c);
  if(cl->gs_head[gs] != AW_null) {
    CL_FreeUnitSelection(gi, c);
    AW_unit_ptr u = cl->gs_head[gs];
    while(u != AW_null) {
      CL_AddToSelection(gi, c, u);
      u = unit(u).gsnext[gs];
    }
    cl->selected_sub_group = cl->gs_sub_group[gs];
    cl->hud_state = 0;
  }
}

void CL_SelectNextSubGroup(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  AW_unit_ptr u = cl->selected_units_head;
  while(u != AW_null
     && unit(u).unit_type >= cl->selected_sub_group)
    u = unit(u).snext[c];
  if(u != AW_null)
    cl->selected_sub_group = unit(u).unit_type;
  else
  if(cl->selected_units_head != AW_null)
    cl->selected_sub_group = 
      unit(cl->selected_units_head).unit_type;
  else
    assert(cl->selected_sub_group == -1);
  cl->hud_state = 0;
}

void CL_CmdUnitOrderOnSelection(AW_game_instance_t *gi, AW_client_ptr c, int to_x, int to_y, bool push_back, bool ground_only, AW_id_t cmd_mask, int r_squared, AW_ptr_t user_data, AW_unit_order_cb_t unit_order_completed_cb, AW_unit_order_cb_t unit_order_failed_cb) {
  BOUND(MAP_SIZE_X-1, to_x);
  BOUND(MAP_SIZE_Y-1, to_y);
  AW_client_t *cl = &client(c);
  AW_player_ptr p = GI_GetPlayerPtr(gi, cl->player_id);
  AW_player_t *pl = &player(p);
  cl->cmd_id++;
  int min_x = MAP_SIZE_X-1, 
      min_y = MAP_SIZE_Y-1, 
      max_x = 0, 
      max_y = 0,
      area  = 0,
      count = 0;
  AW_unit_ptr u = cl->selected_units_head;
  while(u != AW_null) {
    AW_unit_t *un   = &unit(u);
    /* it is possible to select other players' units */
    if(un->player_id == cl->player_id) {
      if(un->pos_x < min_x) 
        min_x = un->pos_x;
      if(un->pos_y < min_y) 
        min_y = un->pos_y;
      if(un->pos_x > max_x) 
        max_x = un->pos_x;
      if(un->pos_y > max_y) 
        max_y = un->pos_y;
      area += SIZE(un)*SIZE(un);
      count+= SIZE(un);
    }
    u = un->snext[c];
  }
  int   a             = area / 3.14;
  float cx            = (min_x + max_x)/2;
  float cy            = (min_y + max_y)/2;
  int   m             = max(max_x-min_x, max_y-min_y);
  bool vis            = PL_IsInFov(gi, p, to_x>>RANGE_SHIFT, to_y>>RANGE_SHIFT);
  AW_unit_ptr target  = vis && !ground_only ? AT(unit_map, to_x, to_y) : AW_null;
  /* we can't attack a unit from the same team */
  target              = unit(target).team_id == pl->team_id && cl->attack_here ? AW_null : target;
  bool  in_formation  = 
             !cl->attack_here
          && (target == AW_null 
          || unit(target).team_id == pl->team_id)
          && m < count
          && m < 60
          && (cx - to_x) * (cx - to_x) 
           + (cy - to_y) * (cy - to_y) 
           < MAX_FORMATION_MOVE
          && (to_x < (min_x + (max_x-min_x)/4) || to_x > (max_x - (max_x-min_x)/4) 
          ||  to_y < (min_y + (max_y-min_y)/4) || to_y > (max_y - (max_y-min_y)/4));
  u = cl->selected_units_head;
  while(u != AW_null) {
    AW_unit_t *un   = &unit(u);
    /* it is possible to select other players' units */
    if(un->player_id == cl->player_id) {
      AW_cmd_ptr r    = CMD_New(gi, un->cmd_id);
      if(r != AW_null) {
        AW_cmd_t *cmd           = &cmd(r);
        cmd->turn               = gi->turn + CMD_TURN_SHIFT;
        cmd->cmd_mask           = cmd_mask == -1 ? cl->cmd_id : cmd_mask;
        cmd->type               = AW_cmd_type_unit_order;
        cmd->r_squared          = in_formation 
                               || cl->attack_here 
                               || (target != AW_null 
                               && unit(target).team_id != pl->team_id) ? 0 : 
                               r_squared == -1 ? a : r_squared;
        cmd->target_cx          = (float)to_x + (in_formation ? un->pos_x - cx : 0);
        cmd->target_cy          = (float)to_y + (in_formation ? un->pos_y - cy : 0);
        cmd->target             = target != AW_null ? unit(target).cmd_id : AW_null;
        cmd->target_player_id   = target != AW_null ? unit(target).player_id : AW_null;
        cmd->target_cmd_id      = target != AW_null ? unit(target).cmd_id : AW_null;
        cmd->attack_here        = cl->attack_here;
        cmd->time_stamp         = gi->game_time;
        cmd->push_back          = push_back;
        cmd->user_data          = user_data;
        cmd->unit_order_completed_cb  = unit_order_completed_cb;
        cmd->unit_order_failed_cb     = unit_order_failed_cb;
        CS_InsertCmd(gi, cl->cs, r);
      }
    }
    u = un->snext[c];
  }
}

void CL_CmdSpawnUnit(AW_game_instance_t *gi, AW_client_ptr c, int to_x, int to_y, AW_unit_type_t unit_type, AW_ptr_t user_data) {
  BOUND(MAP_SIZE_X-1, to_x);
  BOUND(MAP_SIZE_Y-1, to_y);
  AW_client_t *cl = &client(c);
  AW_cmd_ptr r    = CMD_New(gi, cl->cmd_id++);
  AW_cmd_t *cmd   = &cmd(r);
  cmd->turn       = gi->turn + CMD_TURN_SHIFT;
  cmd->type       = AW_cmd_type_spawn_unit;
  cmd->unit_type  = unit_type;
  cmd->target_cx  = to_x;
  cmd->target_cy  = to_y;
  cmd->user_data  = user_data;
  CS_InsertCmd(gi, cl->cs, r);
}

void CL_CmdBuildOrder(AW_game_instance_t *gi, AW_client_ptr c, AW_id_t builder_id, AW_id_t builder_player_id, AW_id_t unit_player_id, AW_unit_type_t unit_type, short target_cx, short target_cy, bool start_it) {
  AW_client_t *cl = &client(c);
  AW_cmd_ptr r           = CMD_New(gi, builder_id);
  AW_cmd_t *cmd          = &cmd(r);
  cmd->turn              = gi->turn + CMD_TURN_SHIFT;
  cmd->type              = AW_cmd_type_build_unit;
  cmd->cmd_mask          = cl->cmd_id++;
  cmd->unit_type         = unit_type;
  cmd->target_cx         = target_cx;
  cmd->target_cy         = target_cy;
  cmd->player_id         = builder_player_id;
  cmd->target_player_id  = unit_player_id;
  cmd->start_it          = start_it;
  cmd->cancel_it         = false;
  CS_InsertCmd(gi, cl->cs, r);
}

void CL_CmdCancelBuildOrder(AW_game_instance_t *gi, AW_client_ptr c, AW_id_t builder_id, AW_id_t builder_player_id, AW_id_t unit_player_id, AW_id_t unit_id) {
  AW_client_t *cl        = &client(c);
  AW_cmd_ptr r           = CMD_New(gi, builder_id);
  AW_cmd_t *cmd          = &cmd(r);
  cmd->turn              = gi->turn + CMD_TURN_SHIFT;
  cmd->type              = AW_cmd_type_build_unit;
  cmd->cmd_mask          = unit_id;
  cmd->player_id         = builder_player_id;
  cmd->target_player_id  = unit_player_id;
  cmd->cancel_it         = true;
  CS_InsertCmd(gi, cl->cs, r);
}

void CL_CmdGeneric(AW_game_instance_t *gi, AW_client_ptr c, AW_id_t id, AW_id_t player_id, AW_id_t cmd_mask, AW_unit_type_t unit_type, short target_cx, short target_cy) {
  AW_client_t *cl        = &client(c);
  AW_cmd_ptr r           = CMD_New(gi, id);
  AW_cmd_t *cmd          = &cmd(r);
  cmd->turn              = gi->turn + CMD_TURN_SHIFT;
  cmd->type              = AW_cmd_type_generic;
  cmd->cmd_mask          = cmd_mask;
  cmd->unit_type         = unit_type;
  cmd->target_cx         = target_cx;
  cmd->target_cy         = target_cy;
  cmd->player_id         = player_id;
  CS_InsertCmd(gi, cl->cs, r);
}

bool CL_HasOrder(AW_game_instance_t *gi, AW_client_ptr c, AW_unit_ptr u) {
  AW_client_t *cl = &client(c);
  AW_unit_t *un   = &unit(u);
  if(UO_GetFrontType(gi, u) 
  != AW_unit_order_none)
    return true;
  AW_cmd_ptr r = cmd_store(cl->cs).cmd_head;
  while(r != AW_null) {
    if(cmd(r).id == un->cmd_id)
      return true;
    r = cmd(r).cmd_next;
  }
  return false;
}

void CL_UpdateWindowCloseButton(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  gi->close_btn.pos_x             = cl->window_end_x;
  gi->close_btn.pos_y             = cl->window_start_y;
  gi->close_btn.color             = TCOD_dark_red;
  gi->close_btn.s                 = "X";
  gi->close_btn.highlight         = false;
  gi->close_btn.entry             = false;
  gi->close_btn.shortcut          = 'x';
  gi->close_btn.shortcut_special  = (TCOD_keycode_t)-1;
}

void CL_OpenOptionWindow(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  cl->window_opened     = true;
  cl->hud_state         = HUD_STATE_OPTION_WINDOW;
  cl->window_size_x     = 30;
  cl->window_size_y     = 15;
  cl->window_start_x    = (CON_RES_X-cl->window_size_x)/2;
  cl->window_start_y    = ((HUD_START_Y-cl->window_size_y)/2);
  cl->window_end_x      = (cl->window_start_x+cl->window_size_x);
  cl->window_end_y      = (cl->window_start_y+cl->window_size_y); 
  CL_UpdateWindowCloseButton(gi, c);
}

void CL_UpdateOptionWindow(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  if(cl->window_opened
  && cl->hud_state == HUD_STATE_OPTION_WINDOW) {
    gi->central_hud_btns.btn_count                  = 1;
    gi->central_hud_btns.pos_x                      = (cl->window_start_x+cl->window_end_x)/2;
    gi->central_hud_btns.pos_y                      = cl->window_start_y+8;
    gi->central_hud_btns.btns[0].color              = TCOD_white;
    gi->central_hud_btns.btns[0].s                  = "(Q)uit";
    gi->central_hud_btns.btns[0].highlight          = true;
    gi->central_hud_btns.btns[0].entry              = false;
    gi->central_hud_btns.btns[0].shortcut           = 'Q';
    gi->central_hud_btns.btns[0].shortcut_special   = (TCOD_keycode_t)-1;
    gi->central_hud_btns.btns[0].pos_x              = gi->central_hud_btns.pos_x;
    gi->central_hud_btns.btns[0].pos_y              = gi->central_hud_btns.pos_y;
    BTN_Render(gi, &gi->central_hud_btns, true, 1);
    if(BTN_IsClicked(gi, &gi->close_btn)
    || GI_IsKeyReleased(gi, TCODK_ESCAPE)) {
      cl->window_opened = false;
      cl->hud_state = HUD_STATE_NO_WINDOW;
      GI_StopPropagation(gi, TCODK_ESCAPE);
    }
    else
    if(BTN_IsClicked(gi, &gi->central_hud_btns.btns[0])) {
      GI_DisconnectFromMasterServer(gi);
      GI_Free(gi);
      gi->state = 0;
      AW_Quit();
      gi->flash_btn.s = "";
    }
  }
}

void CL_RenderRect(void *sdl_surface, int thickness, int color, int x, int y, int w, int h) {
  AW_game_instance_t *gi = renderer_gi;
  SDL_Surface *surface = (SDL_Surface*)sdl_surface;
  FOR_RECT(
    x, 
    x+w-1,
    y-thickness, 
    y-thickness) {
    ON_MINIMAP(i, j) = color;
  }
  FOR_RECT(
    x, 
    x+w-1,
    y+h-1+thickness, 
    y+h-1+thickness) {
    ON_MINIMAP(i, j) = color;
  }
  FOR_RECT(
    x-thickness,
    x-thickness,
    y,
    y+h-1) {
    ON_MINIMAP(i, j) = color;
  }
  FOR_RECT(
    x+w-1+thickness,
    x+w-1+thickness,
    y,
    y+h-1) {
    ON_MINIMAP(i, j) = color;
  }
}

void CL_PostRender(void *sdl_surface) {
  AW_game_instance_t *gi  = renderer_gi;
  SDL_Surface *surface    = (SDL_Surface*)sdl_surface;
  AW_client_ptr c         = GI_GetHumanClient(gi);
  AW_client_t *cl         = &client(c);
  AW_player_ptr local_p   = GI_GetPlayerPtr(gi, game_desc.local_player_id);
  AW_player_t *local_pl   = &player(local_p);
  AW_player_ptr p;
  if(GI_InState(gi, AW_state_game)) {
    /* game post render */
    int offx, offy;
    TCOD_sys_get_fullscreen_offsets(&offx, &offy);
    p = gi->player_head;
    while(p != AW_null) {
      AW_player_t *pl = &player(p);
      AW_unit_ptr u = pl->unit_head;
      while(u != AW_null) {
        AW_unit_t *un = &unit(u);
        if(PL_IsInFov(gi, local_p, u)) {
          short r = un->hp * (SCREEN_UNIT_SIZE(un)-4) / MAX_HP(un);
          int idx[]           = { 0, ((SCREEN_UNIT_SIZE(un)-4)/3)*2, (SCREEN_UNIT_SIZE(un)-4) };
          TCOD_color_t cols[] = {TCOD_red, TCOD_orange, TCOD_green};
          TCOD_color_t map[SCREEN_UNIT_SIZE(un)-4+1];
          TCOD_color_gen_map(map, 3, cols, idx);
          /* life bar */
          DO_TIMES(SCREEN_UNIT_SIZE(un)-4) {
            int x = SCREEN_UNIT_X(un)+2+f,
                y = SCREEN_UNIT_Y(un);
            if(x-(SCREEN_UNIT_X(un)+2) < r) {
              if(INSIDE_SCREEN(x, y)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y)))
                ON_SCREEN(x, y) = COLOR(map[r]);  
              if(INSIDE_SCREEN(x, y+1)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y)))
                ON_SCREEN(x, y+1) = COLOR(map[r]);  
              if(INSIDE_SCREEN(x, y+2)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y)))
                ON_SCREEN(x, y+2) = COLOR(map[r]);  
              /*if(INSIDE_SCREEN(x, y+3)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y)))
                ON_SCREEN(x, y+3) = COLOR(map[r]);  */
            } else {
              if(INSIDE_SCREEN(x, y)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y)))
                ON_SCREEN(x, y) = COLOR(TCOD_black);  
              if(INSIDE_SCREEN(x, y+1)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y)))
                ON_SCREEN(x, y+1) = COLOR(TCOD_black);  
              if(INSIDE_SCREEN(x, y+2)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y)))
                ON_SCREEN(x, y+2) = COLOR(TCOD_black);  
              /*if(INSIDE_SCREEN(x, y+3)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y)))
                ON_SCREEN(x, y+3) = COLOR(TCOD_black);  */
            }
          }
        }
        u = un->pnext;
      }
      p = pl->next;
    }
    /* selection squares */
    p = gi->player_head;
    while(p != AW_null) {
      AW_player_t *pl = &player(p);
      AW_unit_ptr u = pl->unit_head;
      while(u != AW_null) {
        AW_unit_t *un = &unit(u);
        TCOD_color_t color = pl->team_id == local_pl->team_id ? BLUE : RED;
        color = TCOD_color_multiply_scalar(color, 1.5f);
        short r = un->hp * SCREEN_UNIT_SIZE(un) / MAX_HP(un);
        int x, y;
        if(CL_IsUnitSelected(gi, c, u)) {
          DO_TIMES(SCREEN_UNIT_SIZE(un)>>2) {
            int i = f;
            DO_TIMES(3) {
              int j = f;
              x = SCREEN_UNIT_X(un)+i,
              y = SCREEN_UNIT_Y(un)+3+j;
              if(INSIDE_SCREEN(x, y)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y))) 
                ON_SCREEN(x, y) = COLOR(color);
              x = SCREEN_UNIT_X(un)+j,
              y = SCREEN_UNIT_Y(un)+3+i;
              if(INSIDE_SCREEN(x, y)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y))) 
                ON_SCREEN(x, y) = COLOR(color);
              x = SCREEN_UNIT_X(un)+SCREEN_UNIT_SIZE(un)-1-i,
              y = SCREEN_UNIT_Y(un)+3+j;
              if(INSIDE_SCREEN(x, y)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y))) 
                ON_SCREEN(x, y) = COLOR(color);
              x = SCREEN_UNIT_X(un)+SCREEN_UNIT_SIZE(un)-1-j,
              y = SCREEN_UNIT_Y(un)+3+i;
              if(INSIDE_SCREEN(x, y)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y))) 
                ON_SCREEN(x, y) = COLOR(color);

              x = SCREEN_UNIT_X(un)+i,
              y = SCREEN_UNIT_Y(un)+SCREEN_UNIT_SIZE(un)-1-j;
              if(INSIDE_SCREEN(x, y)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y))) 
                ON_SCREEN(x, y) = COLOR(color);
              x = SCREEN_UNIT_X(un)+j,
              y = SCREEN_UNIT_Y(un)+SCREEN_UNIT_SIZE(un)-1-i;
              if(INSIDE_SCREEN(x, y)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y))) 
                ON_SCREEN(x, y) = COLOR(color);

              x = SCREEN_UNIT_X(un)+SCREEN_UNIT_SIZE(un)-1-i,
              y = SCREEN_UNIT_Y(un)+SCREEN_UNIT_SIZE(un)-1-j;
              if(INSIDE_SCREEN(x, y)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y))) 
                ON_SCREEN(x, y) = COLOR(color);
              x = SCREEN_UNIT_X(un)+SCREEN_UNIT_SIZE(un)-1-j,
              y = SCREEN_UNIT_Y(un)+SCREEN_UNIT_SIZE(un)-1-i;
              if(INSIDE_SCREEN(x, y)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y))) 
                ON_SCREEN(x, y) = COLOR(color);
            }
          }
        }
        u = un->pnext;
      }
      p = pl->next;
    }
    /* cursor */
    if(INSIDE_CON(gi->mouse.cx, gi->mouse.cy)) {
      bool vis = PL_IsInFov(gi, local_p, (gi->mouse.cx+cl->viewport_x)>>RANGE_SHIFT, (gi->mouse.cy+cl->viewport_y)>>RANGE_SHIFT);
      if(vis
      && INSIDE_MAP(gi->mouse.cx+cl->viewport_x, gi->mouse.cy+cl->viewport_y)) {
        AW_unit_ptr u = AT(unit_map, gi->mouse.cx+cl->viewport_x, gi->mouse.cy+cl->viewport_y);
        if(u != AW_null) {
          AW_unit_t *un = &unit(u);
          AW_id_t team_id = TEAM_ID(un->player_id);
          TCOD_color_t c = 
            cl->attack_here ? RED : 
            un->player_id == game_desc.local_player_id ? BLUE : 
            team_id == local_pl->team_id ? BLUE : RED;
          c = TCOD_color_multiply_scalar(c, 1.5f);
          int x, y, size = SCREEN_UNIT_SIZE(un)>>2;
          DO_TIMES(SCREEN_UNIT_SIZE(un)-(size<<1)) {
            int i = f;
            DO_TIMES(3) {
              int j = f;
              x = SCREEN_UNIT_X(un)+i+size,
              y = SCREEN_UNIT_Y(un)+3+j;
              if(INSIDE_SCREEN(x, y)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y))) 
                ON_SCREEN(x, y) = COLOR(c);
              x = SCREEN_UNIT_X(un)+i+size,
              y = SCREEN_UNIT_Y(un)+SCREEN_UNIT_SIZE(un)-1-j;
              if(INSIDE_SCREEN(x, y)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y))) 
                ON_SCREEN(x, y) = COLOR(c);
            }
          }
          DO_TIMES(SCREEN_UNIT_SIZE(un)-(size<<1)-3) {
            int i = f;
            DO_TIMES(3) {
              int j = f;
              x = SCREEN_UNIT_X(un)+j,
              y = SCREEN_UNIT_Y(un)+size+3+i;
              if(INSIDE_SCREEN(x, y)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y))) 
                ON_SCREEN(x, y) = COLOR(c);
              x = SCREEN_UNIT_X(un)+SCREEN_UNIT_SIZE(un)-1-j,
              y = SCREEN_UNIT_Y(un)+size+3+i;
              if(INSIDE_SCREEN(x, y)
              && INSIDE_CON(ON_CON_X(x), ON_CON_Y(y))) 
                ON_SCREEN(x, y) = COLOR(c);
            }
          }
        }
      }
    }
    /* black borders */
    FOR_RECT(0, offx-1, 0, resy-1)
      ON_SCREEN(i, j) = 0;
    FOR_RECT(resx-1-offx, resx-1, 0, resy-1)
      ON_SCREEN(i, j) = 0;
    FOR_RECT(0, resx-1, 0, offy-1)
      ON_SCREEN(i, j) = 0;
    FOR_RECT(0, resx-1, resy-1-offy, resy-1)
      ON_SCREEN(i, j) = 0;
    /* dirty */
    p = gi->player_head;
    while(p != AW_null) {
      AW_player_t *pl = &player(p);
      AW_unit_ptr u = pl->unit_head;
      while(u != AW_null) {
        AW_unit_t *un = &unit(u);
        if(un->old_pos_x != -1) {
          SOCLE(SIZE(un))
            if(INSIDE_CON(un->old_pos_x+i, un->old_pos_y+j))
              TCOD_console_set_dirty(
                un->old_pos_x+i, 
                un->old_pos_y+j, 
                1, 
                1);
        }
        SOCLE(SIZE(un))
          if(INSIDE_CON(un->pos_x+i-cl->viewport_x, un->pos_y+j-cl->viewport_y))
            TCOD_console_set_dirty(
              un->pos_x+i-cl->viewport_x, 
              un->pos_y+j-cl->viewport_y, 
              1, 
              1); 
        un->old_pos_x = un->pos_x-cl->viewport_x;
        un->old_pos_y = un->pos_y-cl->viewport_y;
        u = un->pnext;
      }
      p = pl->next;
    }
    /* minimap render */
    /* env */
    FOR_RECT(0, MINIMAP_CON_SIZE_X*CHAR_SIZE-1, 0, MINIMAP_CON_SIZE_Y*CHAR_SIZE-1) {
      bool vis = PL_IsInFov(gi, local_p, (i*MINIMAP_INV_RATIO_X)>>RANGE_SHIFT, (j*MINIMAP_INV_RATIO_Y)>>RANGE_SHIFT);
      #if !RELEASE
      if(gi->show_all)
        vis = true;
      #endif
      TCOD_color_t c = AT(final_color, i*MINIMAP_INV_RATIO_X, j*MINIMAP_INV_RATIO_Y);
      c = vis ? c : TCOD_color_multiply_scalar(c, (float)AMBIENT_LIGHT/255);
      c = TCOD_color_multiply_scalar(c, 0.65f);
      ON_SCREEN(MINIMAP_START_X+i, MINIMAP_START_Y+j) = COLOR(c);
    }
    /* units */
    p = gi->player_head;
    while(p != AW_null) {
      AW_player_t *pl = &player(p);
      int c = pl->team_id == game_desc.local_team_id ? 
        COLOR(TCOD_light_blue) : COLOR(TCOD_red);
      AW_unit_ptr u = pl->unit_head;
      while(u != AW_null) {
        AW_unit_t *un = &unit(u);
        bool vis = PL_IsInFov(gi, local_p, u);
        #if !RELEASE
        if(gi->show_all)
          vis = true;
        #endif
        if(vis) {
          int w = MAX(SIZE(un)*MINIMAP_RATIO_X, 4),
              h = w,
              thickness = 1,
              x = (un->pos_x-floor((float)MAX(SIZE(un), 4*MINIMAP_INV_RATIO_X)*0.5f - 0.5f))*MINIMAP_RATIO_X,
              y = (un->pos_y-floor((float)MAX(SIZE(un), 4*MINIMAP_INV_RATIO_Y)*0.5f - 0.5f))*MINIMAP_RATIO_Y,
              color = COLOR(TCOD_white);
          FOR_RECT(x, x+w-1, y, y+h-1)
            ON_SCREEN(MINIMAP_START_X+i, MINIMAP_START_Y+j) = c;
          FOR_RECT(
            x, 
            x+w-1,
            y-thickness, 
            y-thickness)
            ON_SCREEN(MINIMAP_START_X+i, MINIMAP_START_Y+j) 
              = color;
          FOR_RECT(
            x, 
            x+w-1,
            y+h-1+thickness, 
            y+h-1+thickness)
            ON_SCREEN(MINIMAP_START_X+i, MINIMAP_START_Y+j) 
              = color;
          FOR_RECT(
            x-thickness,
            x-thickness,
            y,
            y+h-1)
            ON_SCREEN(MINIMAP_START_X+i, MINIMAP_START_Y+j) 
              = color;
          FOR_RECT(
            x+w-1+thickness,
            x+w-1+thickness,
            y,
            y+h-1)
            ON_SCREEN(MINIMAP_START_X+i, MINIMAP_START_Y+j) 
              = color;
        }
        u = un->pnext;
      }
      p = pl->next;
    }
    /* viewport */
    AW_client_t *cl = &client(GI_GetHumanClient(gi));
    int c = COLOR(TCOD_white);
    CL_RenderRect(
      sdl_surface,
      1,
      c,
      cl->viewport_x+1,
      cl->viewport_y+1,
      CON_RES_X-2,
      HUD_START_Y-2);
    /*TCOD_console_set_dirty(
      MINIMAP_START_X, 
      MINIMAP_START_Y, 
      MAP_SIZE_X*MINIMAP_RATIO_X, 
      MAP_SIZE_Y*MINIMAP_RATIO_Y);*/
    #if !RELEASE
      if(gi->show_lagometer) {
        FOR_RECT(0, LAGOMETER_SIZE-1, 0, LAGOMETER_SIZE-1)
          ON_LAGOMETER(i, j) = 0xffffffff;
        DO_TIMES(LAGOMETER_SIZE-1) {
          int i = gi->aver_frame_times_counter-MAX_AVER_FRAME_TIMES-1+f;
          if(i<0) i = MAX_AVER_FRAME_TIMES+i;
          int j = (i+1)%MAX_AVER_FRAME_TIMES;
          int x = f, 
              y = LAGOMETER_SIZE-1-(int)(1.f/(gi->aver_frame_times[i]/1000.f));
          TCOD_line_init(
            x, 
            y,
            f+1,
            LAGOMETER_SIZE-1-(int)(1.f/(gi->aver_frame_times[j]/1000.f)));
          do{
            if(!(x>=0 && x<LAGOMETER_SIZE))
              trace(x);
            assert(x>=0 && x<LAGOMETER_SIZE);
            if(y<0)y=0;
            if(y>=LAGOMETER_SIZE)y=LAGOMETER_SIZE-1;
            ON_LAGOMETER(x, y) = COLOR(TCOD_red);
          }while(!TCOD_line_step(&x,&y));
        }
        for(int f = 0 ; f < LAGOMETER_SIZE; f++)
          ON_LAGOMETER(f, LAGOMETER_SIZE-1-60) = COLOR(TCOD_blue);
        /*TCOD_console_set_dirty(
          LAGOMETER_START_X, 
          LAGOMETER_START_Y, 
          LAGOMETER_SIZE, 
          LAGOMETER_SIZE);*/
    } else {
      /*TCOD_console_set_dirty(
        LAGOMETER_START_X, 
        LAGOMETER_START_Y, 
        LAGOMETER_SIZE, 
        LAGOMETER_SIZE);*/
    }
    #endif
  } else {
    if(!gi->minimap_cleared) {
      FOR_RECT(0, MAP_SIZE_X-1, 0, MAP_SIZE_Y-1)
        ON_MINIMAP(i, j) = 0;
      TCOD_console_set_dirty(
        0, 
        0, 
        CON_RES_X, 
        CON_RES_Y);
      gi->minimap_cleared = true;
    }
  }
}

/*
 * player
 */

void PL_Init(AW_game_instance_t *gi) {
  for(int i = 0; i < MAX_PLAYER; i++) {
    player(i).tcod_map = TCOD_map_new(MAP_SIZE_X, MAP_SIZE_Y);
    if(i != MAX_PLAYER-1)
      player(i).fnext = i+1;
    else
      player(i).fnext = AW_null;
  }
  gi->free_player_head = 0;
  gi->player_head      = AW_null;
  trace("Players initiated.");
}

AW_player_ptr PL_New(AW_game_instance_t *gi, AW_ptr_t user_data) {
  if(gi->free_player_head != AW_null) {
    AW_player_ptr r = gi->free_player_head;
    gi->free_player_head = player(gi->free_player_head).fnext;
    AW_player_ptr p   = r;
    AW_player_t *pl   = &player(p);
    pl->win           = false;
    pl->lose          = false;
    pl->worker_count  = 0;
    pl->unit_head     = AW_null;
    pl->user_data     = user_data;
    pl->previous      = AW_null;
    pl->next          = gi->player_head;
    TCOD_map_clear(pl->tcod_map, true, true);
    FOR_RECT(0, MAP_SIZE_X-1, 0, MAP_SIZE_Y-1)
      if(AT_ENV(i>>RANGE_SHIFT, j>>RANGE_SHIFT) 
      == AW_WALL)
        TCOD_map_set_properties(pl->tcod_map, i, j, false, false);
    DO_TIMES(MAX_WORKER) {
      AW_worker_ptr w = f;
      AW_worker_t *wo = &worker(w);
      wo->free = true;
      path(r, w) = 
        TCOD_path_new_using_function(
          MAP_SIZE_X, 
          MAP_SIZE_Y, 
          PL_PathCallback, 
          (void*)&wo->callback_data, 
          1.3f);
      path2(r, w) = 
        TCOD_path_new_using_function(
          RANGE_MAP_SIZE_X, 
          RANGE_MAP_SIZE_Y, 
          PL_PathCallback2, 
          (void*)&wo->callback_data, 
          1.4f);
    }
    if(gi->player_head != AW_null)
      player(gi->player_head).previous = r;
    gi->player_head = r;
    return r;
  } 
  trace("No more player left.");
  return AW_null;
}

void PL_Free(AW_game_instance_t *gi, AW_player_ptr p) {
  DO_TIMES(MAX_WORKER) {
    TCOD_path_delete(path(p, f));
    TCOD_path_delete(path2(p, f));
    if(!worker(f).free)
      PL_CloseWorker(gi, p, f);
  }
  PL_KillAll(gi, p);
  if(gi->player_head == p)
    gi->player_head = player(p).next;
  if(player(p).previous != AW_null)
    player(player(p).previous).next = player(p).next;
  if(player(p).next != AW_null)
    player(player(p).next).previous = player(p).previous;
  player(p).fnext = gi->free_player_head;
  gi->free_player_head = p;
}

void PL_FreeAll(AW_game_instance_t *gi) {
  while(gi->player_head != AW_null)
    PL_Free(gi, gi->player_head);
  trace("Players freed.");
}

bool PL_NextTurn(AW_game_instance_t *gi, AW_player_ptr p) {
  AW_player_t *pl = &player(p);
  AW_cmd_ptr r = cmd_store(pl->cs).cmd_head;
  while(r != AW_null) {
    AW_cmd_t *cmd = &cmd(r);
    if(cmd->turn == gi->turn+1 
    && cmd->type == AW_cmd_type_end_of_turn)
      return true;
    r = cmd->cmd_next;
  }
  return false;
}

void PL_KillUnit(AW_game_instance_t *gi, AW_player_ptr killer_p, AW_player_ptr unit_p, AW_unit_ptr u) {
  AW_player_t *pl = &player(unit_p);
  AW_unit_t *un = &unit(u);
  if(gi->on_death_cb
  && killer_p != AW_null)
    gi->on_death_cb(gi, killer_p, unit_p, u);
  UN_RemoveFromMap(gi, u);
  pl->cmd_to_mem_unit_map.erase(un->cmd_id);
  if(pl->unit_head == u)
    pl->unit_head = un->pnext;
  if(un->pprevious != AW_null)
    unit(un->pprevious).pnext = un->pnext;
  if(un->pnext != AW_null)
    unit(un->pnext).pprevious = un->pprevious;
  AW_client_ptr c = GI_GetHumanClient(gi);
  DO_TIMES(MAX_GROUP_SELECTION)
    if(CL_IsUnitSelected(gi, c, u, f))
      CL_SubToSelection(gi, c, u, f);
  if(CL_IsUnitSelected(gi, c, u))
    CL_SubToSelection(gi, c, u);
  UN_Free(gi, u);
}

void PL_KillAll(AW_game_instance_t *gi, AW_player_ptr p) {
  while(player(p).unit_head != AW_null)
    PL_KillUnit(gi, AW_null, p, player(p).unit_head);
}

bool PL_IsUnitAlive(AW_game_instance_t *gi, AW_player_ptr p, AW_id_t cmd_id) {
  assert(p != AW_null);
  AW_player_t *pl = &player(p);
  AW_unit_ptr u2  = pl->unit_head;
  while(u2 != AW_null) {
    AW_unit_t *un2 = &unit(u2);
    if(cmd_id == un2->cmd_id)
      return true;
    u2 = un2->pnext;
  }
  return false;
}

int PL_IsInFov(AW_game_instance_t *gi, AW_player_ptr p, short pos_x, short pos_y) {
  AW_player_t *pl = &player(p);
  return ON_FOV_MAP(pos_x, pos_y);
}

bool PL_IsInFov(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u) {
  AW_player_t *pl = &player(p);
  AW_unit_t   *un = &unit(u);
  /* this fun is bottleneck */
  short size = SIZE(un);
  if(size==1) {
    if(PL_IsInFov(gi, p, un->pos_x>>RANGE_SHIFT, un->pos_y>>RANGE_SHIFT))
      return true;
  } else {
    SOCLE(size)
      if(PL_IsInFov(gi, p, (un->pos_x+i)>>RANGE_SHIFT, (un->pos_y+j)>>RANGE_SHIFT))
        return true;
  }
  return false;
}

int PL_GetLight(AW_game_instance_t *gi, AW_player_ptr p, short pos_x, short pos_y) {
  return MIN(255, AMBIENT_LIGHT + PL_IsInFov(gi, p, pos_x, pos_y));
}

void PL_ProcessCmdStore(AW_game_instance_t *gi, AW_player_ptr p) {
  AW_player_t       *pl = &player(p);
  AW_cmd_store_t    *cs = &cmd_store(pl->cs);
  AW_cmd_ptr r;
  #if !REPLAY && RECORD
  FILE *file = fopen(str(str("./replay/") + i2a(pl->id)).c_str(), "a+b");
  r = cs->cmd_head;
  while(r != AW_null) {
    AW_cmd_t *cmd = &cmd(r);
    if(cmd->turn == gi->turn) {
      switch(cmd->type) {
        case AW_cmd_type_unit_order: {
            AW_unit_order_packet_t data;
            data.type             = AW_cmd_type_unit_order;
            data.id               = cmd->id;
            data.order            = AW_unit_order_move;
            data.turn             = cmd->turn;
            data.cmd_mask         = cmd->cmd_mask;
            data.r_squared        = cmd->r_squared;
            data.target_cx        = cmd->target_cx;
            data.target_cy        = cmd->target_cy;
            data.target           = cmd->target;
            data.target_player_id = cmd->target_player_id;
            data.target_cmd_id    = cmd->target_cmd_id;
            data.attack_here      = cmd->attack_here;
            data.push_back        = cmd->push_back;
            data.time_stamp       = cmd->time_stamp;
            fwrite((unsigned char*)&data, sizeof(data), 1, file);
          }
          break;
        case AW_cmd_type_spawn_unit: {
            AW_spawn_unit_packet_t data;
            data.type       = AW_cmd_type_spawn_unit;
            data.id         = cmd->id;
            data.unit_type  = cmd->unit_type;
            data.turn       = cmd->turn;
            data.target_cx  = cmd->target_cx;
            data.target_cy  = cmd->target_cy;
            fwrite((unsigned char*)&data, sizeof(data), 1, file);
          }
          break;
        case AW_cmd_type_build_unit: {
            AW_build_unit_packet_t data;
            data.type               = AW_cmd_type_build_unit;
            data.id                 = cmd->id;
            data.turn               = cmd->turn;
            data.cmd_mask           = cmd->cmd_mask;
            data.unit_type          = cmd->unit_type;
            data.target_cx          = cmd->target_cx;
            data.target_cy          = cmd->target_cy;
            data.player_id          = cmd->player_id;
            data.target_player_id   = cmd->target_player_id;
            data.start_it           = cmd->start_it;
            data.cancel_it          = cmd->cancel_it;
            fwrite((unsigned char*)&data, sizeof(data), 1, file);
          }
          break;
        case AW_cmd_type_generic: {
            AW_generic_packet_t data;
            data.type               = AW_cmd_type_generic;
            data.id                 = cmd->id;
            data.turn               = cmd->turn;
            data.cmd_mask           = cmd->cmd_mask;
            data.unit_type          = cmd->unit_type;
            data.target_cx          = cmd->target_cx;
            data.target_cy          = cmd->target_cy;
            data.player_id          = cmd->player_id;
            fwrite((unsigned char*)&data, sizeof(data), 1, file);
          }
          break;
        case AW_cmd_type_end_of_turn: {
            AW_end_of_turn_packet_t data;
            data.type             = AW_cmd_type_end_of_turn;
            data.id               = cmd->id;
            data.turn             = cmd->turn;
            data.turn_frame_count = cmd->turn_frame_count;
            data.frame_time_step  = cmd->frame_time_step;
            data.hash             = cmd->hash;
            fwrite((unsigned char*)&data, sizeof(data), 1, file);
          }
          break;
      }
    }
    r = cmd->cmd_next;
  }
  fclose(file);
  #endif
  /* spawns */
  r = cs->cmd_head;
  while(r != AW_null) {
    AW_cmd_t *cmd = &cmd(r);
    if(cmd->turn == gi->turn) {
      switch(cmd->type) {
        case AW_cmd_type_spawn_unit:
          AW_unit_ptr u = 
            PL_SpawnUnit(
              gi, 
              p, 
              cmd->id, 
              cmd->target_cx, 
              cmd->target_cy, 
              cmd->unit_type, 
              cmd->user_data);
          if(gi->on_spawn_unit_cb)
            gi->on_spawn_unit_cb(gi, u);
          break;
      }
    }
    r = cmd->cmd_next;
  }
  r = cs->cmd_head;
  while(r != AW_null) {
    AW_cmd_t *cmd = &cmd(r);
    if(cmd->turn == gi->turn) {
      switch(cmd->type) {
        case AW_cmd_type_unit_order: {
          /* the unit may be dead */
          if(pl->cmd_to_mem_unit_map.find(cmd->id) 
          != pl->cmd_to_mem_unit_map.end()) {
            BOUND(MAP_SIZE_X-1, cmd->target_cx);
            BOUND(MAP_SIZE_Y-1, cmd->target_cy);
            AW_unit_ptr u           = pl->cmd_to_mem_unit_map[cmd->id];
            AW_unit_t *un           = &unit(u);
            AW_unit_order_ptr o;
            if(cmd->push_back)
              o = UO_PushBack(gi, u);
            else {
              UO_FreeAll(gi, u);
              o = UO_PushFront(gi, u);
            }
            AW_unit_order_t *uo         = &order(o);
            uo->user_data               = cmd->user_data;
            uo->time_stamp              = cmd->time_stamp;
            uo->unit_order_completed_cb = cmd->unit_order_completed_cb;
            uo->unit_order_failed_cb    = cmd->unit_order_failed_cb;
            if(cmd->target == AW_null) {
              uo->order             = AW_unit_order_move;
              uo->target            = AW_null;
              uo->target_cx         = cmd->target_cx;
              uo->target_cy         = cmd->target_cy;
              uo->cmd_mask          = cmd->cmd_mask;
              uo->r_squared         = cmd->r_squared;
              uo->d_squared         = 0;
              uo->attack_here       = cmd->attack_here;
              uo->precise_target    = !cmd->attack_here; 
            } else {
              /* there's a precise target */
              AW_player_ptr target_p = GI_GetPlayerPtr(gi, cmd->target_player_id);
              assert(target_p != AW_null);
              AW_player_t *target_pl = &player(target_p);
              if(PL_IsUnitAlive(gi, target_p, cmd->target_cmd_id)) {
                uo->order             = AW_unit_order_follow_target;
                assert(target_pl->cmd_to_mem_unit_map.find(cmd->target)
                    != target_pl->cmd_to_mem_unit_map.end());
                uo->target            = 
                  target_pl->cmd_to_mem_unit_map[cmd->target];
                uo->target_player_id  = cmd->target_player_id;
                uo->target_cmd_id     = cmd->target_cmd_id;
                uo->target_cx         = cmd->target_cx;
                uo->target_cy         = cmd->target_cy;
                uo->cmd_mask          = uo->target;
                uo->r_squared         = cmd->r_squared;
                uo->d_squared         = 0;
                uo->last_target_cx    = -1;
                uo->attack_here       = cmd->attack_here;
                uo->precise_target    = true;
              }
            }
          }
        } break;
      case AW_cmd_type_build_unit: {
          AW_player_ptr p2 = GI_GetPlayerPtr(gi, cmd->player_id);
          if(player(p2).cmd_to_mem_unit_map.find(cmd->id) 
          != player(p2).cmd_to_mem_unit_map.end()) {
            /* cancel */
            AW_unit_ptr u           = player(p2).cmd_to_mem_unit_map[cmd->id];
            AW_unit_t *un           = &unit(u);
            if(cmd->cancel_it) {
              if(un->build_order_head[cmd->target_player_id] != AW_null) {
                AW_build_order_ptr b = un->build_order_head[cmd->target_player_id];
                while(b != AW_null) {
                  AW_build_order_t *bo = &build_order(b);
                  if(bo->unit_id == cmd->cmd_mask) {
                    if(gi->on_cancel_build_cb)
                      gi->on_cancel_build_cb(gi, b);
                    BO_Free(gi, u, b);
                    break;
                  }
                  b = bo->next;
                }
              }
            } else {
              /* build */
              AW_build_order_ptr b    = BO_New(gi);
              AW_build_order_t *bo    = &build_order(b);
              bo->unit_type           = cmd->unit_type;
              bo->unit_id             = cmd->cmd_mask;
              bo->unit_player_id      = cmd->target_player_id;
              bo->target_cx           = cmd->target_cx;
              bo->target_cy           = cmd->target_cy;
              bo->started             = cmd->start_it;
              BO_PushBackBuildOrder(gi, u, b);
            }
          }
        } break;
      case AW_cmd_type_generic: {
          if(gi->on_generic_cmd_cb)
            gi->on_generic_cmd_cb(
              gi, 
              cmd->id, 
              cmd->player_id, 
              cmd->cmd_mask, 
              cmd->unit_type, 
              cmd->target_cx, 
              cmd->target_cy);
        } break;
      }
    }
    r = cmd->cmd_next;
  }
}

void PL_FindSpawnPoint(AW_game_instance_t *gi, AW_player_ptr p, short size, short *to_x, short *to_y) {
  AW_player_t *pl       = &player(p);
  short       off         = 0;
  bool        found       = false;
  int         min_dist    = 0x7fffffff, dist;
  short       x, y, i, j, endi, endj,
              dx = *to_x, dy = *to_y;
  while(!found 
     && off < MAP_SIZE_X) {
    endi = dx+off;
    for(i = dx-off; i <= endi; i++) {
      j = dy-off;
      if(UN_CanBeHere(gi, AW_null, size, i, j)) {
        dist = (i - dx) * (i - dx) + (j - dy) * (j - dy);
        if(dist < min_dist) {
          min_dist = dist;
          x = i;
          y = j;
          found = true;
        }
      }
    }
    endi = dx+off;
    for(i = dx-off; i <= endi; i++) {
      j = dy+off;
      if(UN_CanBeHere(gi, AW_null, size, i, j)) {
        dist = (i - dx) * (i - dx) + (j - dy) * (j - dy);
        if(dist < min_dist) {
          min_dist = dist;
          x = i;
          y = j;
          found = true;
        }
      }
    }
    endj = dy+off;
    for(j = dy-off; j <= endj; j++) {
      i = dx-off;
      if(UN_CanBeHere(gi, AW_null, size, i, j)) {
        dist = (i - dx) * (i - dx) + (j - dy) * (j - dy);
        if(dist < min_dist) {
          min_dist = dist;
          x = i;
          y = j;
          found = true;
        }
      }
    }
    endj = dy+off;
    for(j = dy-off; j <= endj; j++) {
      i = dx+off;
      if(UN_CanBeHere(gi, AW_null, size, i, j)) {
        dist = (i - dx) * (i - dx) + (j - dy) * (j - dy);
        if(dist < min_dist) {
          min_dist = dist;
          x = i;
          y = j;
          found = true;
        }
      }
    }
    if(found) {
      *to_x = x;
      *to_y = y;
    }
    off++;
  }
}

AW_unit_ptr PL_SpawnUnit(AW_game_instance_t *gi, AW_player_ptr p, AW_id_t cmd_id, short x, short y, AW_unit_type_t unit_type, AW_ptr_t user_data) {
  AW_player_t *pl = &player(p);
  AW_unit_ptr u   = UN_New(gi);
  if(u != AW_null) {
    AW_unit_t *un                       = &unit(u);
    pl->cmd_to_mem_unit_map[cmd_id]     = u;
    un->user_data                       = user_data;
    un->unit_type                       = unit_type;
    un->player_id                       = pl->id;
    un->team_id                         = TEAM_ID(un->player_id);
    un->cmd_id                          = cmd_id;
    PL_FindSpawnPoint(gi, p, SIZE(un), &x, &y);
    un->pos_x                           = x;
    un->pos_y                           = y;
    un->hp = un->virtual_hp             = MAX_HP(un);
    un->blink_acc                       = 0;
    UN_PutInMap(gi, u);
    if(pl->unit_head != AW_null)
      unit(pl->unit_head).pprevious = u;
    un->pnext = pl->unit_head;
    pl->unit_head = u;
    return u;
  }
  return AW_null;
}

void PL_UpdateUnits(AW_game_instance_t *gi, AW_player_ptr p) {
  AW_player_t *pl = &player(p);
  AW_unit_ptr u = pl->unit_head;
  while(u != AW_null) {
    AW_unit_t *un = &unit(u);
    if(un->blink_acc > 0)
      un->blink_acc -= gi->game_time_step;
    u = un->pnext;
  }
  /* workers */
  DO_TIMES(MAX_WORKER) {
    AW_worker_ptr w = f;
    AW_worker_t *wo = &worker(w);
    if(!wo->free) {
      wo->frame_counter++;
      if(wo->frame_counter == wo->frame_count)
        PL_CloseWorker(gi, p, w);
    }
  }
  PL_InitFreeWorker(gi, p);
  /* unit order */
  u = pl->unit_head;
  while(u != AW_null) {
    AW_unit_t *un = &unit(u);
    AW_unit_ptr u2;
    bool done = true;
    switch(UO_GetFrontType(gi, u)) {
      case AW_unit_order_follow_target: {
          AW_unit_order_t *uo = UO_GetFront(gi, u);
          /* some units don't move this is why this 
             check is mandatory here:
             'un' is following an enemy by aggro  
             - it's not a precise target - 
             so 'un' might found another enemy 
             in it's attack range in which case
             it does attack it */
          if(!uo->precise_target
          && (u2=PL_GetFirstEnemyInRange(gi, p, u, ATTACK_RANGE(un)))
          != AW_null
          && PL_IsInFov(gi, u, u2)) {
            UO_FreeFront(gi, u);
            if(UO_GetFrontType(gi, u) 
            == AW_unit_order_follow_target)
              UO_GetFront(gi, u)->last_target_cx = -1;
            PL_GenAttackEnemy(gi, p, u, uo->cmd_mask, uo->time_stamp, false, uo->following_friend);
            done = false;
          } else 
            done = PL_UpdateFollowTarget(gi, p, u);
        } break;
      case AW_unit_order_move:
#if THREADS        
        {
          /* while 'un' is moving, it can happen things... */
          AW_unit_order_t *uo = UO_GetFront(gi, u);
          /* towers and co don't move */
          if(MOVE_SPEED(un) == 0)
            UO_FreeFront(gi, u);
          else
          /* if 'un' is following a friendly unit 
             which has stopped - 'un' might be now 
             following an enemy - it sometimes 
             checks for the original friendly unit 
             to move */
          if(uo->following_friend
          && PL_CheckFollowFriend(gi, p, u, 2)) {
            UO_FreeFront(gi, u);
            UO_FreeFront(gi, u);
            assert(UO_GetFrontType(gi, u) 
                == AW_unit_order_follow_target);
            done = false;
          } else
          /* 'un' is attacking a precise target
             and don't wait the end of the counter
             to attack it if it can */
          if(uo->target != AW_null
          && unit(uo->target).team_id != pl->team_id
          && UN_GetDist(gi, u, uo->target) <= ATTACK_RANGE(un)) {
            UO_FreeFront(gi, u);
            assert(UO_GetFrontType(gi, u) 
                == AW_unit_order_follow_target);
            UO_GetFront(gi, u)->last_target_cx = -1;
            PL_UpdateFollowTarget(gi, p, u);
            done = false;
          } else
          /* regular 'attack here' check for enemy */
          if(uo->attack_here
          && PL_GenFollowEnemy(gi, p, u, uo->cmd_mask, uo->time_stamp, uo->following_friend)) {
            /* a pathfinding may have started, 
               so it must be invalidated 
               - notably, the started_proxy
               may be true and
               may not have been consumed 
               and never must anymore - */
            uo->started_proxy = false;
            uo->task_id = gi->task_mark++;
            uo->starting = false;
            uo->started = false;
            done = false;
          } else
          /* 'un' is following an enemy by aggro  
             - it's not a precise target - 
             so 'un' might found another enemy 
             in it's attack range in which case
             it does attack it */
          if(!uo->precise_target
          && (u2=PL_GetFirstEnemyInRange(gi, p, u, ATTACK_RANGE(un)))
          != AW_null
          && PL_IsInFov(gi, u, u2)) {
            UO_FreeFront(gi, u);
            assert(UO_GetFrontType(gi, u) 
                == AW_unit_order_follow_target);
            UO_GetFront(gi, u)->last_target_cx = -1;
            PL_GenAttackEnemy(gi, p, u, uo->cmd_mask, uo->time_stamp, false, uo->following_friend);
            done = false;
          } else {
            /* start move */
            if(!uo->started) {
              if(!uo->starting) {
                if(PL_StartMoveOrder(gi, p, u))
                  uo->starting = true;
              } else {
                if(uo->started_proxy) {
                  uo->started       = true;
                  uo->starting      = false;
                  uo->started_proxy = false;
                  un->chase_cx      = uo->chase_cx_proxy;
                  un->chase_cy      = uo->chase_cy_proxy;
                  un->path_len      = uo->path_len_proxy;
                  assert(un->path_len >= 0);
                  assert(un->path_index == 0);
                }
              }
            } else {
              /* go, move */
              un->move += gi->game_time_step * MOVE_SPEED(un);
              un->blocked_time += gi->game_time_step;
              un->attack          = 0;
              if(uo->with_counter) {
                uo->counter -= gi->game_time_step;
                if(uo->counter <= 0) {
                  UO_FreeFront(gi, u);
                  assert(UO_GetFrontType(gi, u) 
                      == AW_unit_order_follow_target);
                  UO_GetFront(gi, u)->last_target_cx = -1;
                  done = false;
                }
              }
              /* the move is cancelled if a 
                 friendly unit is blocking 'un' 
                 because it gives a better look
                 and tend to disperse units in a 
                 way which is cool for combat */
              if(done) {
                int px, py;
                if(un->path_len != 0 
                && un->path_index < un->path_len) {
                  short *pt = path_point_ptr(u, un->path_index);
                  px = pt[0];
                  py = pt[1];
                }
                if(!UN_CanBeHere(gi, u, SIZE(un), px, py)) {
                  AW_get_unit_here_t data;
                  UN_GetUnitHere(&data, gi, u, SIZE(un), px, py);
                  bool done = false;
                  for(int f = 0; !done && f < data.count; f++) {
                    AW_unit_ptr u2 = data.result[f];
                    AW_unit_t *un2 = &unit(u2);
                    AW_unit_order_t *uo2 = UO_GetFront(gi, u2);
                    if((UO_GetFrontType(gi, u2) == AW_unit_order_move 
                    && TEAM_ID(un2->player_id) == TEAM_ID(un->player_id)
                    && UN_HasOrder(gi, u, UO_GetFront(gi, u2)->cmd_mask)
                    && UN_HasOrder(gi, u2, uo->cmd_mask))
                    && un->blocked_time < MAX_SPECIAL_BLOCKED_TIME) {
                      /* I've hesitated with move = 0 but found
                         that this gives a better result but
                         it was hard to say */
                      un->move -= gi->game_time_step * MOVE_SPEED(un);
                      done = true;
                    }
                  }
                }
              }
            }
          }
        } 
#endif        
        break;
      case AW_unit_order_attack: {
          un->move = 0;
          AW_unit_order_t *uo = UO_GetFront(gi, u);
          uo->started = true;
          un->attack += gi->game_time_step * ATTACK_SPEED(un);
          /* if 'un' is following a friendly unit 
             which has stopped - 'un' might be now 
             following an enemy - it sometimes 
             checks for the original friendly unit 
             to move */
          if(uo->following_friend
          && PL_CheckFollowFriend(gi, p, u, 2)) {
            UO_FreeFront(gi, u);
            UO_FreeFront(gi, u);
            assert(UO_GetFrontType(gi, u) 
              == AW_unit_order_follow_target);
            done = false;
          } else
          /* 'un' is following an enemy by aggro  
             - it's not a precise target - 
             so 'un' might found another enemy 
             in it's attack range in which case
             it does attack it */
          if(!uo->precise_target) {
            UO_FreeFront(gi, u);
            if(PL_GenAttackEnemy(gi, p, u, uo->cmd_mask, uo->time_stamp, false, uo->following_friend))
              UO_GetFront(gi, u)->started = true;
          }
        }
        break;
      case AW_unit_order_none: {
          un->move = 0;
          un->attack = 0;
          /* regular aggro check */
          if(PL_GenFollowEnemy(gi, p, u, 0, 0, false))
            done = false;
        }
        break;
      default:
        assert(false);
    }
    if(done)
      u = un->pnext;
  }
  /* workers */
  if(pl->free_worker != AW_null)
    PL_StartWorker(gi, p);
  /* build unit */
  u = pl->unit_head;
  while(u != AW_null) {
    AW_unit_t *un = &unit(u);
    DO_TIMES(MAX_PLAYER)
      if(un->build_order_head[f] != AW_null) {
        build_order(un->build_order_head[f]).started = true;
        AW_build_order_ptr b = un->build_order_head[f];
        while(b != AW_null) {
          AW_build_order_t *bo = &build_order(b);
          if(bo->started) {
            bo->progress += gi->game_time_step;
            if(bo->progress >= unit_dic[bo->unit_type].build_time) {
              AW_unit_ptr u2 = 
                PL_SpawnUnit(
                  gi, 
                  GI_GetPlayerPtr(gi, bo->unit_player_id), 
                  bo->unit_id, 
                  bo->target_cx, 
                  bo->target_cy, 
                  bo->unit_type);
              if(gi->on_spawn_unit_cb)
                gi->on_spawn_unit_cb(gi, u2);
              BO_Free(gi, u, b);
            }
          }
          b = bo->next;
        }
      }
    u = un->pnext;
  }
}

bool PL_UpdateFollowTarget(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u) {
  AW_player_t *pl             = &player(p);
  AW_unit_t *un               = &unit(u);
  AW_unit_order_t *follow_uo  = UO_GetFront(gi, u);
  assert(follow_uo->target != AW_null);
  AW_unit_t *target_un        = &unit(follow_uo->target);
  bool result                 = true;
  /* 'un' might be following an enemy 
     while its initial friendly target 
     has stopped but it may be moving 
     again at any moment */
  if(follow_uo->following_friend
  && PL_CheckFollowFriend(gi, p, u, 1)) {
    UO_FreeFront(gi, u);
    result = false;
  } else
  /* 'un' is not its own target and   
      target is alive (else it stops) */
  if(u != follow_uo->target
  && PL_IsUnitAlive(
      gi, 
      GI_GetPlayerPtr(gi, follow_uo->target_player_id), 
      follow_uo->target_cmd_id)) {
    follow_uo->target_cx = target_un->pos_x;
    follow_uo->target_cy = target_un->pos_y;
    bool is_enemy = pl->team_id != TEAM_ID(follow_uo->target_player_id)
                 || follow_uo->attack_here;
    int dist = UN_GetDist(gi, u, follow_uo->target);
    bool attack_it = is_enemy 
                  && dist <= ATTACK_RANGE(un)
                  && PL_IsInFov(gi, p, follow_uo->target);
    if(follow_uo->last_target_cx != follow_uo->target_cx
    || follow_uo->last_target_cy != follow_uo->target_cy
    || attack_it) {
      AW_unit_order_ptr o         = UO_PushFront(gi, u);
      AW_unit_order_t  *uo        = &order(o);
      uo->order                   = attack_it ? AW_unit_order_attack : AW_unit_order_move;
      uo->target                  = follow_uo->target;
      uo->target_player_id        = follow_uo->target_player_id;
      uo->target_cmd_id           = follow_uo->target_cmd_id;
      uo->target_cx               = follow_uo->target_cx;
      uo->target_cy               = follow_uo->target_cy;
      uo->cmd_mask                = follow_uo->cmd_mask;
      uo->time_stamp              = follow_uo->time_stamp;
      uo->r_squared               = 
        MAX(follow_uo->r_squared, is_enemy ? 0 : FOLLOW_R_SQUARED);
      uo->d_squared               = attack_it ? 
        MAX(0, ATTACK_RANGE(un) - MOVE_SPEED(target_un) / ATTACK_SPEED(un) 
          * MOVE_SPEED(target_un) / ATTACK_SPEED(un)) : 0;
      uo->counter                 = FOLLOW_COUNTER;
      uo->with_counter            = true;
      uo->precise_target          = follow_uo->precise_target;
      uo->following_friend        = follow_uo->following_friend;
      follow_uo->done             = false;
      follow_uo->last_target_cx   = follow_uo->target_cx;
      follow_uo->last_target_cy   = follow_uo->target_cy;
      result = false; // done?
    } else {
    /* if 'un' has reached it's friendly target, it
       might now found an enemy target to shoot at */
    if(follow_uo->done
    && !is_enemy)
      result = 
        !PL_GenFollowEnemy(
          gi, 
          p, 
          u, 
          follow_uo->cmd_mask, 
          follow_uo->time_stamp,
          true);
    }
  } else {
    /* 'un' is following itself
       or its target is dead */
    UO_FreeFront(gi, u);
    result = false; // done?
  }
  return result; // done?
}

bool PL_StartMoveOrder(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u) {
  AW_player_t *pl = &player(p);
  if(pl->free_worker != AW_null) {
    AW_worker_ptr w         = pl->free_worker;
    AW_worker_t *wo         = &worker(w);
    if(wo->task_count < MAX_WORKER_TASK) {
      AW_unit_order_t *uo   = UO_GetFront(gi, u);
      AW_unit_t *un         = &unit(u);
      un->blocked_time      = 0;
      un->path_len          = 0;
      un->path_index        = 0;
      wo->task_cost         += sqrt((un->pos_x - uo->target_cx) * (un->pos_x - uo->target_cx)
                              + (un->pos_y - uo->target_cy) * (un->pos_y - uo->target_cy));
      AW_worker_task_t *ta  = &gi->worker_tasks[p][w][wo->task_count++];
      ta->u                 = u;
      ta->pos_x             = un->pos_x;
      ta->pos_y             = un->pos_y;
      ta->target_cx         = uo->target_cx;
      ta->target_cy         = uo->target_cy;
      ta->size              = SIZE(un);
      ta->chase_cx_proxy    = &uo->chase_cx_proxy;
      ta->chase_cy_proxy    = &uo->chase_cy_proxy;
      ta->path_len_proxy    = &uo->path_len_proxy;
      ta->task_id_proxy     = &uo->task_id;
      ta->started_proxy     = &uo->started_proxy;
      ta->id = uo->task_id  = gi->task_mark++;
      assert(!uo->started_proxy);
      return true;
    }
  }
  return false;
}

bool PL_MoveAndAttack(AW_game_instance_t *gi, AW_player_ptr p) {
  AW_player_t *pl = &player(p);
  bool done = true;
  AW_unit_ptr u = pl->unit_head;
  while(u != AW_null) {
    AW_unit_t *un = &unit(u);
    if(UO_GetFront(gi, u) 
    && UO_GetFront(gi, u)->started) {
      switch(UO_GetFrontType(gi, u)) {
        case AW_unit_order_move:
          done = PL_MoveUnit(gi, p, u) && done;
          break;
        case AW_unit_order_attack:
          done = PL_UnitAttack(gi, p, u) && done;
          break;
      }
    }
    u = un->pnext;
  }
  return done;
}

float PL_PathCallback(int xFrom, int yFrom, int xTo, int yTo, void *user_data) {
  AW_path_callback_t *callback_data 
    = (AW_path_callback_t*)user_data;
  AW_game_instance_t *gi  = callback_data->gi;
  AW_player_ptr p         = callback_data->p;
  AW_worker_ptr w         = callback_data->w;
  AW_worker_t *wo         = &worker(w);
  AW_player_t *pl         = &player(p);
  short size              = callback_data->size;
  bool result;
  float value = 1.f;
  int *close_map = gi->close_map3[p][w][size-1];
  if(callback_data->follow_mode) 
    result = ON_CLOSE_MAP(xFrom, yFrom) >= callback_data->start_mark;
  else {
    value = !callback_data->do_first_pass ? 1.f :
      gi->close_map4[p][w][RANGE_MAP_SIZE_X*(yFrom>>RANGE_SHIFT)+(xFrom>>RANGE_SHIFT)]
      == gi->close_map_mark4[p][w] ? 1.f : 5.f;
    callback_data->follow_mode = 
         callback_data->can_follow
      && ON_CLOSE_MAP(xFrom, yFrom) 
      == callback_data->mark;
    result = PL_Walkable(
              gi, 
              p,
              w, 
              size, 
              (short)xTo, 
              (short)yTo);
    /* this fun is bottleneck */
    if(size==1) {
      if(ON_HEAVY_PATH_MAP(xTo, yTo))
        value = HEAVY_PATH_COST;
    } else {
      SOCLE(size)
        if(ON_HEAVY_PATH_MAP(xTo+i, yTo+j)) {
          value = HEAVY_PATH_COST;
          _finished = 1;
        }
    }
  }
  return result ? value : 0;
}

float PL_PathCallback2(int xFrom, int yFrom, int xTo, int yTo, void *user_data) {
  AW_path_callback_t *callback_data 
    = (AW_path_callback_t*)user_data;
  AW_game_instance_t *gi = callback_data->gi;
  return AT_ENV(xFrom, yFrom) != AW_WALL ? 1.f : 0.f;
}

bool PL_Walkable(AW_game_instance_t *gi, AW_player_ptr p, AW_worker_ptr w, short size, short pos_x, short pos_y) {
  AW_player_t *pl = &player(p);
  /* this function is bottleneck */
  if(size==1) {
    if(!TCOD_map_is_walkable(pl->tcod_map, pos_x, pos_y))
      return false;
  } else {
    SOCLE(size)
      if(!TCOD_map_is_walkable(pl->tcod_map, pos_x+i, pos_y+j))
        return false;
  }
  return true;
}

bool PL_LightWalkable(AW_game_instance_t *gi, AW_player_ptr p, AW_worker_ptr w, short size, short pos_x, short pos_y) {
  AW_player_t *pl = &player(p);
  AW_worker_t *wo = &worker(w);
  /* this function is bottleneck */
  if(size==1) {
    if(!TCOD_map_is_walkable(pl->tcod_map, pos_x, pos_y)
    || ON_HEAVY_PATH_MAP(pos_x, pos_y))
      return false;
  } else {
    SOCLE(size)
      if(!TCOD_map_is_walkable(pl->tcod_map, pos_x+i, pos_y+j)
      || ON_HEAVY_PATH_MAP(pos_x+i, pos_y+j))
        return false;
  }
  return true;
}

void PL_FindNearestWalkablePoint(AW_game_instance_t *gi, AW_player_ptr p, AW_worker_ptr w, short size, short pos_x, short pos_y, short *to_x, short *to_y) {
  AW_player_t *pl         = &player(p);
  short       off         = 0;
  bool        found       = false;
  int         min_dist    = 0x7fffffff, dist;
  short       x, y, i, j, endi, endj,
              dx = *to_x, dy = *to_y;
  int *close_map = gi->close_map[size-1];
  while(!found 
     && off < MAP_SIZE_X) {
    endi = dx+off;
    for(i = dx-off; i <= endi; i++) {
      j = dy-off;
      bool is_unit = pos_x == i && pos_y == j;
      if(is_unit 
      || (INSIDE_MAP(i, j)
      && PL_LightWalkable(gi, p, w, size, i, j)
      && ON_CLOSE_MAP(i, j) == ON_CLOSE_MAP(pos_x, pos_y)
      && (i - dx) * (i - dx) + (j - dy) * (j - dy) < min_dist)) {
        min_dist = dist;
        x = i;
        y = j;
        found = true;
      }
    }
    endi = dx+off;
    for(i = dx-off; i <= endi; i++) {
      j = dy+off;
      bool is_unit = pos_x == i && pos_y == j;
      if(is_unit 
      || (INSIDE_MAP(i, j)
      && PL_LightWalkable(gi, p, w, size, i, j)
      && ON_CLOSE_MAP(i, j) == ON_CLOSE_MAP(pos_x, pos_y)
      && (i - dx) * (i - dx) + (j - dy) * (j - dy) < min_dist)) {
        min_dist = dist;
        x = i;
        y = j;
        found = true;
      }
    }
    endj = dy+off;
    for(j = dy-off; j <= endj; j++) {
      i = dx-off;
      bool is_unit = pos_x == i && pos_y == j;
      if(is_unit 
      || (INSIDE_MAP(i, j)
      && PL_LightWalkable(gi, p, w, size, i, j)
      && ON_CLOSE_MAP(i, j) == ON_CLOSE_MAP(pos_x, pos_y)
      && (i - dx) * (i - dx) + (j - dy) * (j - dy) < min_dist)) {
        min_dist = dist;
        x = i;
        y = j;
        found = true;
      }
    }
    endj = dy+off;
    for(j = dy-off; j <= endj; j++) {
      i = dx+off;
      bool is_unit = pos_x == i && pos_y == j;
      if(is_unit 
      || (INSIDE_MAP(i, j)
      && PL_LightWalkable(gi, p, w, size, i, j)
      && ON_CLOSE_MAP(i, j) == ON_CLOSE_MAP(pos_x, pos_y)
      && (i - dx) * (i - dx) + (j - dy) * (j - dy) < min_dist)) {
        min_dist = dist;
        x = i;
        y = j;
        found = true;
      }
    }
    if(found) {
      *to_x = x;
      *to_y = y;
    }
    off++;
  }
}

void PL_PathfinderScanMap(AW_game_instance_t *gi, short size, short pos_x, short pos_y, int *close_mark) {
  int *close_map = gi->close_map[size-1];
  if(INSIDE_MAP(pos_x, pos_y)
  && AT_ENV(pos_x>>RANGE_SHIFT, pos_y>>RANGE_SHIFT) != AW_WALL
  && ON_CLOSE_MAP(pos_x, pos_y) == 0xffffffff) {
    static short scan_map_x[MAP_SIZE];
    static short scan_map_y[MAP_SIZE];
    static int scan_map_counter;     
    *close_mark = *close_mark+1;
    int mark = *close_mark;
    scan_map_counter = 0;
    ON_CLOSE_MAP(pos_x, pos_y) = mark;
    scan_map_x[scan_map_counter] = pos_x;
    scan_map_y[scan_map_counter] = pos_y;
    scan_map_counter++;
    while(scan_map_counter > 0) {
      pos_x = scan_map_x[scan_map_counter-1];
      pos_y = scan_map_y[scan_map_counter-1];
      scan_map_counter--;
      FOR_RECT(-1, 1, -1, 1) 
        if(INSIDE_MAP(pos_x+i, pos_y+j)
        && ON_CLOSE_MAP(pos_x+i, pos_y+j) != mark
        /* todo: take size into account */
        && AT_ENV((pos_x+i)>>RANGE_SHIFT, (pos_y+j)>>RANGE_SHIFT)
        != AW_WALL) {
          ON_CLOSE_MAP(pos_x+i, pos_y+j) = mark;
          scan_map_x[scan_map_counter] = pos_x+i;
          scan_map_y[scan_map_counter] = pos_y+j;
          scan_map_counter++;
        }
    }
  }
}

bool PL_MoveUnit(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u) {
  AW_player_t *pl     = &player(p);
  AW_unit_t *un       = &unit(u);
  AW_unit_order_t *uo = UO_GetFront(gi, u);
  int px, py;
  if(un->path_len != 0 
  && un->path_index < un->path_len) {
    short *pt = path_point_ptr(u, un->path_index);
    px = pt[0];
    py = pt[1];
    if(((px == un->pos_x || py == un->pos_y) && un->move >= 1000)
    || ((px != un->pos_x && py != un->pos_y) && un->move >= 1410)) {
      un->move = 0;
      if(!UN_CanBeHere(gi, u, SIZE(un), px, py)) {
        int dist;
        if(uo->target != AW_null)
          dist = UN_GetDist(gi, u, uo->target);
        else
          dist = UN_GetDist(gi, u, un->chase_cx, un->chase_cy);
        if(dist <= uo->r_squared) {
          /* dist can be 0 here because 
             'un''s socle might be on the spot
             but not it's pos and another unit
             avoid 'un' to have its pos on the spot */
          /* goal reached */
          UO_FreeFront(gi, u, true);
          if(UO_GetFrontType(gi, u) 
          == AW_unit_order_follow_target)
            UO_GetFront(gi, u)->done = true;
        } else
        if(PL_PushUnit(gi, p, u, uo->time_stamp, px, py, un->pos_x, un->pos_y)) {
          un->blocked_time = 0;
          UN_RemoveFromMap(gi, u);
          un->pos_x = px;
          un->pos_y = py;
          UN_PutInMap(gi, u);
          un->path_index++;
        } else {
          AW_get_unit_here_t data;
          UN_GetUnitHere(&data, gi, u, SIZE(un), px, py);
          bool done = false;
          for(int f = 0; !done && f < data.count; f++) {
            AW_unit_ptr u2 = data.result[f];
            AW_unit_t *un2 = &unit(u2);
            AW_unit_order_t *uo2 = UO_GetFront(gi, u2);
            if(uo2 != null
            && uo2->target == u) {
              if(un->blocked_time >= MAX_SPECIAL_BLOCKED_TIME)
                un->push_power = MAX(un->push_power, SIZE(un2));
            } else
            if(UO_GetFrontType(gi, u2) == AW_unit_order_none
            && un->blocked_time < MAX_SPECIAL_BLOCKED_TIME) {
              un->push_power = MAX(un->push_power, SIZE(un2));
            } else
            if((/*UO_GetFrontType(gi, u2) == AW_unit_order_none
                this one is supposed to be for the case where there is no
                room any more really for 'un'. but it causes nasty bugs.
            ||*/ (UO_GetFrontType(gi, u2) == AW_unit_order_follow_target
            && UO_GetFront(gi, u2)->done == true
            && uo->cmd_mask == UO_GetFront(gi, u2)->cmd_mask)
            || (UO_GetFrontType(gi, u2) == AW_unit_order_move 
            && !UN_HasOrder(gi, u, UO_GetFront(gi, u2)->cmd_mask)
            && !UN_HasOrder(gi, u2, uo->cmd_mask)))
            /* this time threshold must be lesser
               than the follow-target counter */
            && un2->player_id == un->player_id
            && un->blocked_time >= MAX_SPECIAL_BLOCKED_TIME) {
              /* WARN: if 'un' is chasing an enemy, it 
                 will stop on this condition 
                 and I don't know if it's ok */
              do {
                /* it's blocked */
                UO_FreeFront(gi, u);
                /* in the case of push_back orders
                   we don't want the unit to go for
                   the next order since it has failed
                   this one at the moment */
              } while(UO_GetFrontType(gi, u)
                   == AW_unit_order_move);
              /* WARN: this has never been trully tested */
              if(UO_GetFrontType(gi, u) 
              == AW_unit_order_follow_target)
                UO_GetFront(gi, u)->done = true;
              done = true;
            } else 
            /* deadlock */
            if(UO_GetFrontType(gi, u2) == AW_unit_order_move
            && UO_GetFront(gi, u2)->started
            && un2->team_id == un->team_id
            && un2->path_len != 0 
            && un2->path_index < un2->path_len
            && un->blocked_time >= MAX_SPECIAL_BLOCKED_TIME) {
              short *pt2  = path_point_ptr(u2, un2->path_index),
                    p2x   = pt2[0],
                    p2y   = pt2[1];
              AW_get_unit_here_t data2;
              UN_GetUnitHere(&data2, gi, u2, SIZE(un2), p2x, p2y);
              for(int g = 0; !done && g < data2.count; g++) {
                AW_unit_ptr u3 = data2.result[g];
                if(u3 == u) {
                  short i = un->path_index+1;
                  short *pt3, p3x = -1, p3y = -1;
                  while(i < un->path_len
                     && !UN_CanBeHere(gi, u, SIZE(un), p3x, p3y)) {
                    pt3 = path_point_ptr(u, i);
                    p3x = pt3[0];
                    p3y = pt3[1];
                    i++;
                  }
                  if(i - un->path_index 
                  <= MAX_DEADLOCK_TELEPORT
                  && UN_CanBeHere(gi, u, SIZE(un), p3x, p3y)) {
                    un->blocked_time = 0;
                    UN_RemoveFromMap(gi, u);
                    un->pos_x = p3x;
                    un->pos_y = p3y;
                    UN_PutInMap(gi, u);
                    un->path_index = i;
                  } else 
                  if(un->path_len - un->path_index 
                  <= MAX_DEADLOCK_TELEPORT) {
                    assert(un->path_len 
                      - un->path_index > 0);
                    /* goal reached */ 
                    UO_FreeFront(gi, u, true);
                    if(UO_GetFrontType(gi, u) 
                    == AW_unit_order_follow_target) {
                      /* if 'un' is following an enemy and drop here
                         it won't chase the enemy anymore, it's not ok
                         because ultimately we want a unit always chasing
                         an enemy but for good-looking reasons - it makes units 
                         to become crazy turning and chasing, updatefollowtarget
                         doesn't always chase an enemy if it hasn't moved 
                         so on this particular condition, which actually happen
                         often in a fight, we want to unit to continue to chase
                         the enemy */
                      if(un->team_id
                      != TEAM_ID(UO_GetFront(gi, u)->target_player_id))
                        UO_GetFront(gi, u)->last_target_cx = -1;
                      UO_GetFront(gi, u)->done = true;
                    }
                  }
                  done = true;
                }
              }
            }
          }
        }
      } else {
        if(un->path_index < un->path_len) {
          un->blocked_time = 0;
          UN_RemoveFromMap(gi, u);
          un->pos_x = px;
          un->pos_y = py;
          UN_PutInMap(gi, u);
          un->path_index++;
        }
      } 
      if(UO_GetFront(gi, u) == uo
      && UO_GetFrontType(gi, u) == AW_unit_order_move
      && UN_GetDist(gi, u, un->chase_cx, un->chase_cy) 
      <= uo->d_squared)
        /* goal reached */
        UO_FreeFront(gi, u, true);
    }
  }
  /* done? */
  return UO_GetFront(gi, u) != uo ||
      !(((px == un->pos_x || py == un->pos_y) && un->move >= 1000)
      || ((px != un->pos_x && py != un->pos_y) && un->move >= 1410));
}

bool PL_GenFollowEnemy(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u, AW_id_t cmd_mask, AW_time_t time_stamp, bool following_friend) {
  AW_player_t *pl    = &player(p);
  AW_unit_t *un      = &unit(u);
  AW_unit_ptr target = 
    PL_GetFirstEnemyInRange(gi, p, u, AGGRO_RANGE(un));
  if(target != AW_null) {
    AW_unit_t *target_un          = &unit(target);
    AW_unit_order_ptr o           = UO_PushFront(gi, u);
    AW_unit_order_t *follow_uo    = &order(o);
    follow_uo->order              = AW_unit_order_follow_target;
    follow_uo->target             = target;
    follow_uo->target_player_id   = target_un->player_id;
    follow_uo->target_cmd_id      = target_un->cmd_id;
    follow_uo->target_cx          = target_un->pos_x;
    follow_uo->target_cy          = target_un->pos_y;
    follow_uo->cmd_mask           = cmd_mask;
    follow_uo->time_stamp         = time_stamp;
    follow_uo->r_squared          = 0;
    follow_uo->d_squared          = 0;
    follow_uo->last_target_cx     = -1;
    follow_uo->precise_target       = false;
    follow_uo->following_friend   = following_friend;
    return true;
  }
  return false;
}

bool PL_GenAttackEnemy(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u, AW_id_t cmd_mask, AW_time_t time_stamp, bool precise_target, bool following_friend) {
  AW_player_t *pl    = &player(p);
  AW_unit_t *un      = &unit(u);
  AW_unit_ptr target = 
    PL_GetFirstEnemyInRange(gi, p, u, ATTACK_RANGE(un));
  if(target != AW_null) {
    AW_unit_t *target_un          = &unit(target);
    AW_unit_order_ptr o           = UO_PushFront(gi, u);
    AW_unit_order_t *attack_uo    = &order(o);
    attack_uo->order              = AW_unit_order_attack;
    attack_uo->target             = target;
    attack_uo->target_player_id   = target_un->player_id;
    attack_uo->target_cmd_id      = target_un->cmd_id;
    attack_uo->target_cx          = target_un->pos_x;
    attack_uo->target_cy          = target_un->pos_y;
    attack_uo->cmd_mask           = cmd_mask;
    attack_uo->time_stamp         = time_stamp;
    attack_uo->r_squared          = 0;
    attack_uo->d_squared          = 
      MAX(0, ATTACK_RANGE(un) - 
      (MOVE_SPEED(target_un) / ATTACK_SPEED(un)) * 
      (MOVE_SPEED(target_un) / ATTACK_SPEED(un)));
    attack_uo->last_target_cx     = -1;
    attack_uo->precise_target       = precise_target;
    attack_uo->following_friend   = following_friend;
    return true;
  }
  return false;
}

bool PL_CheckFollowFriend(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u, short level) {
  AW_unit_order_ptr o = unit(u).order_head;
  while(level--) o = order(o).next;
  assert(o != AW_null);
  AW_unit_order_t *follow_uo = &order(o);
  assert(follow_uo->order == AW_unit_order_follow_target);
  AW_unit_t *target_un       = &unit(follow_uo->target);
  return follow_uo->last_target_cx != target_un->pos_x
      || follow_uo->last_target_cy != target_un->pos_y;
}

AW_unit_ptr PL_GetFirstEnemyInRange(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u, int range) {
  AW_player_t *pl         = &player(p);
  AW_unit_t   *un         = &unit(u);
  if((SIZE(un)&1) == 0) {
    AW_unit_ptr result      = AW_null;
    int         min_dist    = 0x7fffffff;
    /* 2x2 points gives a good center for even socle size */
    SOCLE(2) {
      AW_unit_ptr u2 = PL_GetFirstEnemyInRange(gi, p, u, range, un->pos_x+i, un->pos_y+j);
      if(u2 != AW_null) {
        int dist = UN_GetDist(gi, u, u2);
        if(dist < min_dist) {
          result = u2;
          min_dist = dist;
        }
      }
    }
    return result;
  }
  return PL_GetFirstEnemyInRange(gi, p, u, range, un->pos_x, un->pos_y);
}

AW_unit_ptr PL_GetFirstEnemyInRange(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u, int range, short pos_x, short pos_y) {
  AW_player_t *pl         = &player(p);
  AW_unit_t *un           = &unit(u);
  int         off         = 0;
  int         min_dist    = 0x7fff;
  int         i, j, endi, endj,
              dx = pos_x, 
              dy = pos_y;
  bool        found = false;
  AW_unit_ptr result = AW_null;
  while(!found
     && off < MAP_SIZE_X
     && off*off <= range) {
    endi = dx+off;
    for(i = dx-off; i <= endi; i++) {
      j = dy-off;
      if(INSIDE_MAP(i, j)) {
        AW_unit_ptr u2 = AT(unit_map, i, j);
        if(u2 != AW_null) {
          AW_unit_t *un2 = &unit(u2);
          if(un2->team_id != pl->team_id
          && un2->virtual_hp > 0
          && PL_IsInFov(gi, p, u2)) {
            int dist = (pos_x - i) * (pos_x - i)
                     + (pos_y - j) * (pos_y - j);
            if(dist < min_dist
            && dist <= range) {
              result    = AT(unit_map, i, j);
              min_dist  = dist;
              found     = true;
            }
          }
        }
      }
    }
    endi = dx+off;
    for(i = dx-off; i <= endi; i++) {
      j = dy+off;
      if(INSIDE_MAP(i, j)) {
        AW_unit_ptr u2 = AT(unit_map, i, j);
        if(u2 != AW_null) {
          AW_unit_t *un2 = &unit(u2);
          if(un2->team_id != pl->team_id
          && un2->virtual_hp > 0
          && PL_IsInFov(gi, p, u2)) {
            int dist = (pos_x - i) * (pos_x - i)
                     + (pos_y - j) * (pos_y - j);
            if(dist < min_dist
            && dist <= range) {
              result    = AT(unit_map, i, j);
              min_dist  = dist;
              found     = true;
            }
          }
        }
      }
    }
    endj = dy+off;
    for(j = dy-off; j <= endj; j++) {
      i = dx-off;
      if(INSIDE_MAP(i, j)) {
        AW_unit_ptr u2 = AT(unit_map, i, j);
        if(u2 != AW_null) {
          AW_unit_t *un2 = &unit(u2);
          if(un2->team_id != pl->team_id
          && un2->virtual_hp > 0
          && PL_IsInFov(gi, p, u2)) {
            int dist = (pos_x - i) * (pos_x - i)
                     + (pos_y - j) * (pos_y - j);
            if(dist < min_dist
            && dist <= range) {
              result    = AT(unit_map, i, j);
              min_dist  = dist;
              found     = true;
            }
          }
        }
      }
    }
    endj = dy+off;
    for(j = dy-off; j <= endj; j++) {
      i = dx+off;
      if(INSIDE_MAP(i, j)) {
        AW_unit_ptr u2 = AT(unit_map, i, j);
        if(u2 != AW_null) {
          AW_unit_t *un2 = &unit(u2);
          if(un2->team_id != pl->team_id
          && un2->virtual_hp > 0
          && PL_IsInFov(gi, p, u2)) {
            int dist = (pos_x - i) * (pos_x - i)
                     + (pos_y - j) * (pos_y - j);
            if(dist < min_dist
            && dist <= range) {
              result    = AT(unit_map, i, j);
              min_dist  = dist;
              found     = true;
            }
          }
        }
      }
    }
    off++;
  }
  return result;
}

bool PL_PushUnit(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u, AW_time_t time_stamp, short x, short y, short from_x, short from_y) {
  AW_unit_t *un = &unit(u);
  gi->close_map_mark++;
  SOCLE(SIZE(un))
    if(INSIDE_MAP(from_x+i, from_y+j))
      AT(close_map2, from_x+i, from_y+j) = gi->close_map_mark;
  bool result = PL_PushUnitRecursive(gi, p, u, u, time_stamp, un->push_power, x, y);
  if(!result) {
    gi->close_map_mark++;
    SOCLE(SIZE(un))
      if(INSIDE_MAP(from_x+i, from_y+j))
        AT(close_map2, from_x+i, from_y+j) = gi->close_map_mark;
    result = PL_PushUnitRecursive(gi, p, u, u, time_stamp, SIZE(un), x, y);
  }
  un->push_power = 1;
  return result;
}

bool PL_PushUnitRecursive(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u, AW_unit_ptr target, AW_time_t time_stamp, short size, short x, short y) {
  // fixme.
  AW_player_t *pl = &player(p);
  AW_unit_t *un = &unit(u);
  AW_id_t team_id = TEAM_ID(player(p).id);
  SOCLE(SIZE(un))
    if(INSIDE_MAP(x+i, y+j))
      AT(close_map2, x+i, y+j) = gi->close_map_mark;
  AW_get_unit_here_t data;
  bool can_be_here = UN_CanBeHere(gi, u, SIZE(un), x, y);
  if(!can_be_here) {
    UN_GetUnitHere(&data, gi, u, SIZE(un), x, y);
    for(int f = 0; f < data.count; f++) {
      AW_unit_ptr u2 = data.result[f];
      AW_unit_t *un2 = &unit(u2);
      if(UO_GetFrontType(gi, u2) != AW_unit_order_none 
      && un2->player_id == un->player_id
      && !UO_IsTarget(gi, u2, target)
      && UO_GetFront(gi, u2)->time_stamp < time_stamp) {
        assert(UO_GetFront(gi, u2)->time_stamp != -1);
        assert(time_stamp != -1);
        do {
          UO_FreeFront(gi, u2);
        } while(UO_GetFrontType(gi, u2)
             != AW_unit_order_none);
      }
      if(CAN_BE_PUSHED(un2)
      && UO_GetFrontType(gi, u2) != AW_unit_order_move 
      && UO_GetFrontType(gi, u2) != AW_unit_order_attack 
      && un2->team_id == team_id
      && (!UO_GetFront(gi, u) 
      || UO_GetFront(gi, u)->target != u2)) {
        unsigned int seed = MAP_SIZE_X*y+x + MAP_SIZE*un2->cmd_id;
        int used_r[8];
        for(int g = 0; g < 8; g++)
          used_r[g] = 0xffffffff;
        int h = 0;
        bool done = false;
        while(!done
           && h < 8) {
          int r = AW_Rand(&seed)&7;
          bool good = true;
          for(int g = 0; good && g < h; g++) 
            good = r != used_r[g];
          if(good) {
            used_r[h] = r;
            short ii = gi->square_x[r],
                  jj = gi->square_y[r],
                  xi = un2->pos_x+ii*size,
                  yj = un2->pos_y+jj*size;
            bool open = true;
            SOCLE(SIZE(un2))
              if(!INSIDE_MAP(xi+i, yj+j)
              || AT(close_map2, xi+i, yj+j) == gi->close_map_mark)
                open = false;
            if(open
            && PL_PushUnitRecursive(gi, p, u2, target, time_stamp, size, xi, yj)) {
              UN_RemoveFromMap(gi, u2);
              un2->pos_x = xi;
              un2->pos_y = yj;
              UN_PutInMap(gi, u2);
              done = true;
            }
            h++;
          }
        }
        if(!done) {
          can_be_here = false;
          break;
        }
      } else {
        can_be_here = false;
        break;
      }
    }
  }
  if(!can_be_here)
    can_be_here = UN_CanBeHere(gi, u, SIZE(un), x, y);
  return can_be_here;
}

bool PL_UnitAttack(AW_game_instance_t *gi, AW_player_ptr p, AW_unit_ptr u) {
  AW_player_t *pl         = &player(p);
  AW_unit_t   *un         = &unit(u);
  AW_unit_order_t *uo     = UO_GetFront(gi, u);
  assert(uo->target != AW_null);
  AW_unit_t *target_un    = &unit(uo->target);
  AW_player_ptr target_p  = GI_GetPlayerPtr(gi, uo->target_player_id);
  if(PL_IsUnitAlive(gi, target_p, uo->target_cmd_id)
  && target_un->virtual_hp > 0) {
    if(un->attack >= 1000) {
      un->attack -= 1000;
      short damage          = MIN(target_un->virtual_hp, ATTACK_DAMAGE(un));
      target_un->virtual_hp -= damage;
      AW_ball_ptr b         = BA_New(gi);
      AW_ball_t *ba         = &ball(b);
      ba->p                 = GI_GetPlayerPtr(gi, un->player_id);
      ba->move              = 0;
      ba->pos_x             = un->pos_x;
      ba->pos_y             = un->pos_y;
      ba->damage            = damage;
      ba->target            = uo->target;
      ba->target_player_id  = uo->target_player_id;
      ba->target_cmd_id     = uo->target_cmd_id;
      if(target_un->virtual_hp <= 0)
        /* target eliminated */
        UO_FreeFront(gi, u, true);
    } else {
      /* the 'un' checks if the target 
       is still visible and at reach 
       if not it stop attacking */
      int dist = UN_GetDist(gi, u, uo->target);
      if(dist > ATTACK_RANGE(un)
      || !PL_IsInFov(gi, p, uo->target))
        UO_FreeFront(gi, u);
    }
  } else
    UO_FreeFront(gi, u); 
  return UO_GetFront(gi, u) != uo || un->attack < 1000; // done?
}

void PL_InitFreeWorker(AW_game_instance_t *gi, AW_player_ptr p) {
  AW_player_t *pl = &player(p);
  pl->free_worker = AW_null;
#if THREADS  
  DO_TIMES(MAX_WORKER)
    if(gi->workers[p][f].free) {
      pl->free_worker = f;
      AW_worker_t *wo = &worker(pl->free_worker);
      wo->gi          = gi;
      wo->p           = p;
      wo->w           = f;
      wo->task_count  = 0;
      wo->task_cost   = 0;
      wo->free        = false;
      break;
    }
#endif    
}

void PL_StartWorker(AW_game_instance_t *gi, AW_player_ptr p) {
#if THREADS  
  AW_player_t *pl   = &player(p);
  AW_worker_ptr w   = pl->free_worker;
  AW_worker_t *wo   = &worker(w);
  if(wo->task_count > 0) {
    memcpy(wo->heavy_path_map, gi->heavy_path_map, sizeof(gi->heavy_path_map));
    wo->frame_count   = 1 + wo->task_cost / TASK_COST_PER_FRAME;
    wo->frame_counter = 0;
    wo->handle        = CreateThread(null, 0, PL_WorkerFun, (void*)wo, 0, &wo->id); 
    assert(wo->handle);
    //START_BENCH
    if(wo->frame_count == 1)
      PL_CloseWorker(gi, p, w);
    //END_BENCH
  } else
    wo->free = true;
#endif    
}

void PL_CloseWorker(AW_game_instance_t *gi, AW_player_ptr p, AW_worker_ptr w) {
#if THREADS
  AW_player_t *pl = &player(p);
  AW_worker_t *wo = &worker(w);
  DWORD r = WaitForSingleObject(wo->handle, 10000);
  assert(r!=WAIT_FAILED);
  CloseHandle(wo->handle);
  DO_TIMES(wo->task_count) {
    AW_worker_task_t *ta = &gi->worker_tasks[p][w][f];
    /* meanwhile, the task may have been cancelled */
    if(ta->id == *ta->task_id_proxy) {
      if(!(UO_GetFrontType(gi, ta->u) 
      == AW_unit_order_move
      && UO_GetFront(gi, ta->u)->starting)) {
        trace(ta->id);
        trace(UO_GetFrontType(gi, ta->u));
        if(UO_GetFront(gi, ta->u) != null)
          trace(UO_GetFront(gi, ta->u)->starting);
      }
      assert(UO_GetFrontType(gi, ta->u) 
          == AW_unit_order_move
          && UO_GetFront(gi, ta->u)->starting);
      *ta->chase_cx_proxy = ta->chase_cx;
      *ta->chase_cy_proxy = ta->chase_cy;
      *ta->path_len_proxy = ta->path_len;
      *ta->started_proxy  = true;
      for(int i = 0; i < ta->path_len; i++) {
        short *path_pt_worker_ptr = path_point_worker_ptr(i);
        short *path_pt_ptr = path_point_ptr(ta->u, i);
        path_pt_ptr[0] = path_pt_worker_ptr[0];
        path_pt_ptr[1] = path_pt_worker_ptr[1];
      }
    }
  }
  wo->free = true;
#endif  
}

DWORD WINAPI PL_WorkerFun(void *params) {
#if THREADS  
  AW_worker_t *wo               = (AW_worker_t*)params;
  AW_worker_ptr w               = wo->w;
  AW_game_instance_t *gi        = wo->gi;
  AW_player_ptr p               = wo->p;
  wo->callback_data.start_mark  = gi->close_map_mark3[p][w];
  gi->close_map_mark4[p][w]++;
  int i = 0,
      close_mark = 0;
  do {
    AW_worker_task_t *ta = &gi->worker_tasks[p][w][i++];
    short dx = ta->target_cx, 
          dy = ta->target_cy;
    PL_FindNearestWalkablePoint(gi, p, w, ta->size, ta->pos_x, ta->pos_y, &dx, &dy);
    bool pathfind = false;
    ta->path_len = 0;
    int x0 = ta->pos_x, y0 = ta->pos_y, x1 = dx, y1 = dy;
    int ddx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int ddy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
    int err = (ddx>ddy ? ddx : -ddy)/2, e2;
    while(!pathfind && (x0 != x1 || y0 != y1)) {
      e2 = err;
      if (e2 >-ddx) { err -= ddy; x0 += sx; }
      if (e2 < ddy) { err += ddx; y0 += sy; }
      short *path_pt_ptr = path_point_worker_ptr(ta->path_len);
      path_pt_ptr[0] = x0;
      path_pt_ptr[1] = y0;
      pathfind = !PL_LightWalkable(gi, p, w, ta->size, x0, y0);
      ta->path_len++;
    }
    if(pathfind) {
      wo->callback_data.gi          = gi;
      wo->callback_data.p           = p;
      wo->callback_data.follow_mode = false;
      int dist = (ta->pos_x - dx) * (ta->pos_x - dx)
               + (ta->pos_y - dy) * (ta->pos_y - dy);
      /* this is crucial for combat because they have short path */
      wo->callback_data.can_follow  = dist > MAX_FORMATION_MOVE;
      wo->callback_data.w           = w;
      wo->callback_data.size        = ta->size;
      TCOD_path_t *path             = &path(p, w);
      TCOD_path_t *path2            = &path2(p, w);
      /* first pass */
      /* this is crucial not to do a first pass for combat */
      wo->callback_data.do_first_pass = dist >= MAX_FORMATION_MOVE;
      if(wo->callback_data.do_first_pass
      && gi->close_map4[p][w][RANGE_MAP_SIZE_X*(dy>>RANGE_SHIFT)+(dx>>RANGE_SHIFT)] 
      != gi->close_map_mark4[p][w]) {
        gi->close_map_mark4[p][w]++;
        gi->close_map4[p][w][RANGE_MAP_SIZE_X*(ta->pos_y>>RANGE_SHIFT)+(ta->pos_x>>RANGE_SHIFT)] 
          = gi->close_map_mark4[p][w];
        gi->close_map4[p][w][RANGE_MAP_SIZE_X*(dy>>RANGE_SHIFT)+(dx>>RANGE_SHIFT)] 
          = gi->close_map_mark4[p][w];
        TCOD_path_compute(
          *path2, 
          ta->pos_x>>RANGE_SHIFT, 
          ta->pos_y>>RANGE_SHIFT, 
          dx>>RANGE_SHIFT, 
          dy>>RANGE_SHIFT);
        int len = TCOD_path_size(*path2);
        for(int k = 0; k < len; k++) {
          int x, y;
          TCOD_path_get(*path2, k, &x, &y);
          gi->close_map4[p][w][RANGE_MAP_SIZE_X*y+x] = 
            gi->close_map_mark4[p][w];
        }
      }
      /* second pass */
      int *close_map = gi->close_map3[p][w][ta->size-1];
      if(ON_CLOSE_MAP(dx, dy) < wo->callback_data.start_mark)
        ON_CLOSE_MAP(dx, dy) = gi->close_map_mark3[p][w]++;
      wo->callback_data.mark = ON_CLOSE_MAP(dx, dy);
      bool found_a_path = TCOD_path_compute(*path, ta->pos_x, ta->pos_y, dx, dy);
      assert(found_a_path);
      ta->path_len = TCOD_path_size(*path);
      assert(ta->path_len >= 0);
      if(ta->path_len != 0) {
        int *close_map = gi->close_map3[p][w][ta->size-1];
        ON_CLOSE_MAP(ta->pos_x, ta->pos_y) = wo->callback_data.mark;
        ON_CLOSE_MAP(dx, dy) = wo->callback_data.mark;
        for(int k = 0; k < ta->path_len; k++) {
          short *path_pt_ptr = path_point_worker_ptr(k);
          int x, y;
          TCOD_path_get(*path, k, &x, &y);
          path_pt_ptr[0] = x;
          path_pt_ptr[1] = y;
          ON_CLOSE_MAP(x, y) = wo->callback_data.mark;
        }
      }
    }
    ta->chase_cx = dx;
    ta->chase_cy = dy;
  } while(i < wo->task_count);
#endif  
}

/*
 * game instance
 */

void GI_Init(AW_game_instance_t *gi, int _argc, char **_argv) {
  /* audio */
  assert(SDL_Init(SDL_INIT_AUDIO)>=0);
  int audio_rate = 22050;
  unsigned short audio_format = AUDIO_S16SYS;
  int audio_channels = 2;
  int audio_buffers = 256;
  Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);
  gi->main_music = Mix_LoadMUS(MAIN_MUSIC);
  Mix_PlayMusic(gi->main_music, -1);
  Mix_VolumeMusic(25);
  gi->coin_sound    = OPEN_SOUND("./data/coin.ogg");
  gi->torch_sound   = OPEN_SOUND("./data/campfire.ogg");
  gi->foot_sound    = OPEN_SOUND("./data/steps.ogg");
  Mix_AllocateChannels(1024);

  /* rest */
  renderer_gi = gi;
  TCOD_sys_register_SDL_renderer(CL_PostRender);
  enet_initialize();
  enet_address_set_host(&gi->master_server_addr, game_desc.master_server.c_str());
  gi->glow_map  = new TCOD_color_t[CON_RES_X*CON_RES_Y];
  gi->glow_map2 = new TCOD_color_t[CON_RES_X*CON_RES_Y];
  gi->post_render_mask = new int[CON_RES_X*CON_RES_Y];
  #if !RELEASE
  gi->show_all = false;
  gi->show_lagometer = false;
  #endif
  gi->render_bloom = true;
  gi->host = null;
  gi->peer = null;
  gi->master_server_addr.port = MASTER_SERVER_PORT;
  gi->state = 0;
  gi->add_state_count = 0;
  gi->remove_state_count = 0;
  gi->minimap_cleared = true;
  gi->frame_time = 0;
  gi->frame_mark = 0;
  gi->aver_frame_times_counter = 0;
  gi->aver_wait_times_counter = 0;
  DO_TIMES(MAX_AVER_FRAME_TIMES)
    gi->aver_frame_times[f] = NORMAL_TIME_STEP;
  DO_TIMES(MAX_AVER_WAIT_TIMES)
    gi->aver_wait_times[f] = 0;
  DO_TIMES(MAX_MAX_PING_TIMES)
    gi->pings[f] = 100;
  gi->square_x[0] = +1; gi->square_y[0] = +1;
  gi->square_x[1] = +1; gi->square_y[1] = -1;
  gi->square_x[2] = +1; gi->square_y[2] = +0;
  gi->square_x[3] = -1; gi->square_y[3] = +1;
  gi->square_x[4] = -1; gi->square_y[4] = -1;
  gi->square_x[5] = -1; gi->square_y[5] = +0;
  gi->square_x[6] = +0; gi->square_y[6] = +1;
  gi->square_x[7] = +0; gi->square_y[7] = -1;
  gi->task_mark         = 0;
  gi->close_map_mark    = 0;
  DO_TIMES(MAP_SIZE)
    gi->close_map2[f] = 0xffffffff;
  DO_TIMES(MAX_UNIT_SIZE)
    halfes[f] = floor((float)(f+1)*0.5f - 0.5f);
  CMD_Init(gi, argc, argv);
  CS_Init(gi, argc, argv);
  RS_Init(gi, argc, argv);
  UO_Init(gi, argc, argv);
  BO_Init(gi, argc, argv);
  BA_Init(gi, argc, argv);
  SMOKE_Init(gi, argc, argv);
  BLOOD_Init(gi, argc, argv);
  FT_Init(gi, argc, argv);
  LI_Init(gi, argc, argv);
  SO_Init(gi, argc, argv);
  UN_Init(gi, argc, argv);
  PL_Init(gi);
  CL_Init(gi, argc, argv);
  GI_InitEnvMap(gi); 
  DO_TIMES(MAX_UNIT_SIZE) {
    int g = f;
    int *close_map  = gi->close_map[g];
    DO_TIMES(MAP_SIZE)
      close_map[f]  = 0xffffffff;
    int close_mark = 0;
    FOR_RECT(0, MAP_SIZE_X-1, 0, MAP_SIZE_Y-1)
      PL_PathfinderScanMap(gi, g+1, i, j, &close_mark);
  }
  DO_TIMES(MAX_PLAYER) {
    int p = f;
    DO_TIMES(MAX_WORKER) {
      int w = f;
      gi->close_map_mark3[p][w] = 0;
      gi->close_map_mark4[p][w] = 0;
      DO_TIMES(RANGE_MAP_SIZE)
        gi->close_map4[p][w][f] = 0xffffffff;
      DO_TIMES(MAX_UNIT_SIZE) {
        int g = f;
        int *close_map3 = gi->close_map3[p][w][g];
        DO_TIMES(MAP_SIZE)
          close_map3[f] = 0xffffffff;
      }
    }
  }
  gi->fog_noise = TCOD_noise_new(2, 0.5f, 2.0, null);
  gi->fog_scroll_x = gi->fog_scroll_y = 0;
  gi->fog_mask = TCOD_image_load("./data/fog_mask.png");
  gi->on_death_cb = null;
  #include "ui.cpp"
  trace("game instance initiated");
}

void GI_InitGame(AW_game_instance_t *gi, game_desc_t *gd, AW_init_game_cb_t init_game_cb) {
  /* plugued-in init */
  (*init_game_cb)(gi, gd);
  /* connection */
  AW_client_ptr c = gi->client_head;
  while(c != AW_null) {
    AW_client_t *cl = &client(c);
    CL_Connect(
      gi, 
      c, 
      gd->game_name);
    c = cl->next;
  }
  AW_remote_cmd_store_ptr r = gi->remote_cmd_store_head;
  while(r != AW_null) {
    AW_remote_cmd_store_t *rs = &remote_cmd_store(r);
    RS_Connect(
      gi, 
      r, 
      gd->game_name); 
    r = rs->next;
  }
  /* misc */
  DO_TIMES(MAX_TEAM) {
    int i = f;
    DO_TIMES(RANGE_MAP_SIZE)
      gi->fov_maps[i][f] = 0;
  }
  trace("The game is initiated.");
}

void GI_StartGame(AW_game_instance_t *gi, AW_start_game_cb_t start_game_cb) {
  gi->desync            = false;
  gi->minimap_cleared   = false;
  gi->flash_btn.s       = "";
  gi->game_time_step    = 0;
  gi->frame_time_step   = NORMAL_TIME_STEP;
  gi->game_time_acc     = 0;
  gi->turn              = -2;
  gi->turn_frame_count  = STARTING_TURN_FRAME_COUNT;
  gi->turn_acc          = gi->turn_frame_count;
  gi->game_time         = gi->last_t = AW_GetTime();
  gi->wait_time         = 0;
  GI_EndOfTurn(gi);
  gi->turn_acc          = gi->turn_frame_count;
  trace("The game has started.");
  GI_StartHumanClient(gi, GI_GetHumanClient(gi));
  (*start_game_cb)(gi);
}

void GI_StartHumanClient(AW_game_instance_t *gi, AW_client_ptr c) {
  AW_client_t *cl = &client(c);
  cl->viewport_x  = game_desc.local_team_id == 0 ? 0 : MAP_SIZE_X-1;
  cl->viewport_y  = game_desc.local_team_id == 0 ? MAP_SIZE_Y-1 : 0;
  BOUND_VIEWPORT
}

void GI_Free(AW_game_instance_t *gi) {
  if(gi->free_all_cb)
    gi->free_all_cb(gi);
  GI_KillHost(gi);
  CL_FreeAll(gi);
  PL_FreeAll(gi);
  UN_FreeAll(gi);
  SO_FreeAll(gi);
  LI_FreeAll(gi);
  BA_FreeAll(gi);
  SMOKE_FreeAll(gi);
  BLOOD_FreeAll(gi);
  FT_FreeAll(gi);
  RS_FreeAll(gi);
  CS_FreeAll(gi);
  CMD_FreeAll(gi);
  trace("Game instance freed.");
}

void GI_Close(AW_game_instance_t *gi) {
  GI_Free(gi);
  delete [] gi->glow_map;
  delete [] gi->glow_map2;
  delete [] gi->post_render_mask;
  Mix_FreeChunk(gi->coin_sound);
  Mix_FreeChunk(gi->torch_sound);
  Mix_FreeChunk(gi->foot_sound);
  Mix_FreeMusic(gi->main_music);
  Mix_CloseAudio();
}

void GI_InitEnvMap(AW_game_instance_t *gi) {
  TCOD_noise_t noise        = TCOD_noise_new(2, 0.5f, 2.0, null);
  TCOD_random_t random      = TCOD_random_new_from_seed(TCOD_RNG_MT, 0);
  gi->bump_noise            = TCOD_noise_new(2, 0.5f, 2.0, random);
  TCOD_image_t map          = TCOD_image_load("./data/map copy.png");
  TCOD_image_t walls        = TCOD_image_load("./data/map walls.png");
  FOR_RECT(0, RANGE_MAP_SIZE_X-1, 0, RANGE_MAP_SIZE_Y-1) {
    TCOD_color_t c = TCOD_image_get_pixel(walls, i, j);
    if(IS_COLOR(255, 255, 255)) 
      AT_ENV(i, j) = AW_WALL;
    else
      AT_ENV(i, j) = AW_EMPTY;
  }
  DO_TIMES(MAP_SIZE)
    gi->static_light_map[f] = AMBIENT_LIGHTMAP;
  FOR_RECT(0, MAP_SIZE_X-1, 0, MAP_SIZE_Y-1) {
    TCOD_color_t c = TCOD_image_get_pixel(map, i, j);
    int x = i,
        y = j;
    /* env art map */
    TCOD_color_t c2;
    float /* color noise */
          f1, 
          /* grey noise */
          f2, 
          /* freq color/grey noise */
          f3 = 256, 
          /* 'hills' */
          f4;
    if(IS_COLOR(255, 255, 255)) {
      AT(env_art_map, x, y) = AW_WALL;
      c2 = WALL_COLOR;
      f1 = 0.0f;
      f2 = 0.0f;
      f4 = 0.f;
    }
    else
    if(IS_COLOR(0, 0, 255)) {
      AT(env_art_map, x, y) = AW_WATER;
      c2 = WATER_COLOR;
      f1 = 0.0f;
      f2 = 0.25f;
      f3 = 50;
      f4 = 1.f;
    }
    else
    if(IS_COLOR(0, 0, 100)) {
      AT(env_art_map, x, y) = AW_WATER2;
      c2 = WATER2_COLOR;
      f1 = 0.0f;
      f2 = 0.25f;
      f3 = 50;
      f4 = 1.f;
    }
    else
    if(IS_COLOR(0, 255, 0)) {
      AT(env_art_map, x, y) = AW_GRASS;
      c2 = GRASS_COLOR;
      f1 = 0.3f;
      f2 = 0.5f;
      f4 = 1.f;
    }
    else
    if(IS_COLOR(0, 200, 0)) {
      AT(env_art_map, x, y) = AW_BUSH1;
      c2 = BUSH1_COLOR;
      f1 = 0.1f;
      f2 = 0.1f;
      f4 = 1.f;
    }
    else
    if(IS_COLOR(0, 150, 0)) {
      AT(env_art_map, x, y) = AW_BUSH2;
      c2 = BUSH2_COLOR;
      f1 = 0.1f;
      f2 = 0.1f;
      f4 = 1.f;
    }
    else
    if(IS_COLOR(0, 100, 0)) {
      AT(env_art_map, x, y) = AW_TREE1;
      c2 = TREE1_COLOR;
      f1 = 0.1f;
      f2 = 0.1f;
      f4 = 1.f;
    }
    else
    if(IS_COLOR(0, 75, 0)) {
      AT(env_art_map, x, y) = AW_TREE2;
      c2 = TREE2_COLOR;
      f1 = 0.1f;
      f2 = 0.1f;
      f4 = 1.f;
    }
    else
    if(IS_COLOR(0, 50, 0)) {
      AT(env_art_map, x, y) = AW_TREE3;
      c2 = TREE3_COLOR;
      f1 = 0.1f;
      f2 = 0.1f;
      f4 = 1.f;
    }
    else
    if(IS_COLOR(100, 0, 0)) {
      AT(env_art_map, x, y) = AW_MUSH;
      c2 = MUSH_COLOR;
      f1 = 0.1f;
      f2 = 0.1f;
      f4 = 1.f;
    }
    else
    if(IS_COLOR(100, 50, 0)) {
      AT(env_art_map, x, y) = AW_PLANK;
      c2 = PLANK_COLOR;
      f1 = 0.05f;
      f2 = 0.05f;
      f4 = 0.5f;
    }
    else
    if(IS_COLOR(75, 50, 0)) {
      AT(env_art_map, x, y) = AW_PLANK2;
      c2 = PLANK2_COLOR;
      f1 = 0.05f;
      f2 = 0.05f;
      f4 = 0.5f;
    }
    else
    if(IS_COLOR(128, 128, 128)) {
      AT(env_art_map, x, y) = AW_TILE;
      c2 = TILE_COLOR;
      f1 = 0.020f;
      f2 = 0.05f;
      f4 = 0.5f;
    }
    else
    if(IS_COLOR(200, 200, 200)) {
      AT(env_art_map, x, y) = AW_TILE2;
      c2 = TILE2_COLOR;
      f1 = 0.0075f;
      f2 = 0.05f;
      f4 = 0.5f;
    }
    else
    if(IS_COLOR(255, 255, 0)) {
      AT(env_art_map, x, y) = AW_TORCH;
      c2 = TORCH_COLOR;
      f1 = 0.1f;
      f2 = 0.1f;
      f4 = 1.f;
    }
    else {
      AT(env_art_map, x, y) = AW_EMPTY;
      c2 = EMPTY_COLOR;
      f1 = 0.05f;
      f2 = 0.05f;
      f4 = 1.f;
    }
    /* dir light */
    AT(shadow_map, x, y) = true;
    int x1  = i,
        y1  = j,
        x2  = i - DIR_LIGHTMAP_X,
        y2  = j - DIR_LIGHTMAP_Y;
    TCOD_color_t c3 = DIR_LIGHTMAP_COLOR;
    TCOD_line_init(x1, y1, x2, y2);
    do {
      if(INSIDE_MAP(x1, y1)
      && AT_ENV(x1>>RANGE_SHIFT, y1>>RANGE_SHIFT) 
      == AW_WALL) {
        AT(shadow_map, x, y) = false;
        c3 = TCOD_black;
        break;
      }
    } while(!TCOD_line_step(&x1, &y1));
    float v[4];
    FOR_RECT(0, 1, 0, 1) {
      float coords[2];
      coords[0] = (float)(x+i)*25 / MAP_SIZE_X;
      coords[1] = (float)(y+j)*25 / MAP_SIZE_Y;
      v[(j<<1)+i] = 0.5f + 0.5f * TCOD_noise_get(gi->bump_noise, coords);  
    }
    float N[3],
          L[3];
    L[0] = -1; L[1] = -1; L[2] = 1;
    N[0] = (v[0] - v[1])*6*f4; N[1] = (v[0] - v[2])*6*f4; N[2] = 1;
    float L_len = sqrt(L[0]*L[0] + L[1]*L[1] + L[2]*L[2]),
          N_len = sqrt(N[0]*N[0] + N[1]*N[1] + N[2]*N[2]);
    DO_TIMES(3) {
      L[f] /= L_len;
      N[f] /= N_len;
      AT(normal_map, x, y)[f] = N[f];
      gi->dir_light[f] = L[f];
    }
    float dot = N[0] * L[0] + N[1] * L[1] + N[2] * L[2];
    c3 = TCOD_color_multiply_scalar(c3, SATURATE(dot));
    AT(static_light_map, x, y) = TCOD_color_add(AT(static_light_map, x, y), c3);
    /* ambient occlusion */
    int s = 8;
    float count = s*s;
    FOR_RECT(-s/2, +s/2-1, -s/2, +s/2-1) {
      if(!INSIDE_MAP(x+i, y+j)
      || AT_ENV((x+i)>>RANGE_SHIFT, (y+j)>>RANGE_SHIFT) 
      == AW_WALL)
        count-=1.25;
    }
    count = MAX(0, count);
    AT(static_light_map, x, y) = 
      TCOD_color_multiply_scalar(AT(static_light_map, x, y), MAX(0.f, (float)count/(s*s)));
    /* walls: no AO, no dir shadows */
    if(AT_ENV(x>>RANGE_SHIFT, y>>RANGE_SHIFT) 
    == AW_WALL)
      AT(static_light_map, x, y) = TCOD_color_add(AMBIENT_LIGHTMAP, DIR_LIGHTMAP_COLOR);
    /* final color */
    TCOD_color_t l = AT(static_light_map, x, y);
    float coords[2];
    coords[0] = (float)x*f3/MAP_SIZE_X;
    coords[1] = (float)y*f3/MAP_SIZE_Y;
    int noise_r = (float)(TCOD_noise_get(noise, coords))*f1*255;
    coords[0] = (float)x*f3/MAP_SIZE_X+1000;
    coords[1] = (float)y*f3/MAP_SIZE_Y+1000;
    int noise_g = (float)(TCOD_noise_get(noise, coords))*f1*255;
    coords[0] = (float)x*f3/MAP_SIZE_X+2000;
    coords[1] = (float)y*f3/MAP_SIZE_Y+2000;
    int noise_b = (float)(TCOD_noise_get(noise, coords))*f1*255;
    coords[0] = (float)x*f3/MAP_SIZE_X+3000;
    coords[1] = (float)y*f3/MAP_SIZE_Y+3000;
    int noise_rgb = (float)(TCOD_noise_get(noise, coords))*f2*255;
    int r = MAX(0, MIN(255, ((c2.r + noise_r + noise_rgb))))&0xff, 
        g = MAX(0, MIN(255, ((c2.g + noise_g + noise_rgb))))&0xff,
        b = MAX(0, MIN(255, ((c2.b + noise_b + noise_rgb))))&0xff;
    AT(env_color, x, y) = TCOD_color_RGB(r, g, b);
    r = ((c2.r * l.r)>>8)&0xff, 
    g = ((c2.g * l.g)>>8)&0xff,
    b = ((c2.b * l.b)>>8)&0xff;
    /*r = MAX(TCOD_darker_grey.r, r);
    g = MAX(TCOD_darker_grey.g, g);
    b = MAX(TCOD_darker_grey.b, b);*/
    AT(final_color, x, y) = TCOD_color_RGB(r, g, b);
    /* dyn lights*/
    if(AT(env_art_map, x, y) == AW_TORCH) {
      AW_light_ptr l = LI_New(gi);
      AW_light_t *li = &light(l);
      li->pos_x = x;
      li->pos_y = y;
      li->r     = 16;
      li->c     = TORCH_COLOR;
      SLOW_START_SOUND(torch_sound, -1)
      so->volume = 8;
      so->pos_x = li->pos_x;
      so->pos_y = li->pos_y;
    }
  }
  /* tcod map for dyn lights */ 
  TCOD_map_clear(gi->tcod_light_map, true, true);
    FOR_RECT(0, RANGE_MAP_SIZE_X-1, 0, RANGE_MAP_SIZE_Y-1)
      if(AT_ENV(i, j) == AW_WALL)
        TCOD_map_set_properties(gi->tcod_light_map, i, j, false, false);
  TCOD_random_delete(random);
  trace("env_map initiated.");
}

void GI_UpdateTime(AW_game_instance_t *gi) {
  GI_UpdateAverFrameTime(gi, gi->frame_time);
  gi->game_time_acc     += gi->frame_time;
  if(!gi->desync
  #if REPLAY
  //&& gi->turn <= gi->max_turn
  #endif
  ) {
    if(gi->turn_acc < gi->turn_frame_count) {
      if(gi->game_time_acc >= gi->frame_time_step) {
        gi->game_time_acc -= gi->frame_time_step;
        gi->game_time_step = gi->frame_time_step;
        gi->turn_acc++;
      } else
        gi->game_time_step = 0;  
    } else {
      gi->game_time_step = 0;
      gi->wait_time += gi->frame_time_step;
    }
  } else
    gi->game_time_step = 0;

  gi->game_time                 += gi->game_time_step;
  gi->turn_frame_count_dec_acc  += gi->game_time_step;
}

void GI_UpdateTurn(AW_game_instance_t *gi) {
  if(gi->turn_acc >= gi->turn_frame_count) {
    bool change_turn    = true;
    AW_player_ptr p     = gi->player_head;
    while(p != AW_null) {
      change_turn = PL_NextTurn(gi, p) && change_turn;
      p = player(p).next;
    }
    if(change_turn)
      GI_EndOfTurn(gi);
  }
}

void GI_Update(AW_game_instance_t *gi) {
  // basic time
  AW_time_t t           = AW_GetTime(),
            frame_time  = t - gi->last_t;
  if(frame_time < 0 || gi->game_time_acc < 0) 
      trace(str("bug ") + f2a(frame_time) + " " + f2a(gi->game_time_acc) + " " + f2a(gi->game_time_step));
  if(frame_time < 0) frame_time = 0; // the bug (tm)
  gi->frame_time = MIN(frame_time, MAX_FRAME_TIME);
  gi->last_t     = t;
  // clear
  TCOD_console_clear(con);
  BTN_CenterX(gi, &gi->flash_btn);
  BTN_Render(gi, &gi->flash_btn);
  // general inputs
  GI_UpdateInputs(gi);
  // states
  #include "state.cpp"
  GI_UpdateState(gi);
  // end frame
  TCOD_console_blit(con, 0, 0, CON_RES_X, CON_RES_Y, null, 0, 0, 1, 1);
  TCOD_console_flush();
  gi->frame_mark++;
}

void GI_UpdatePlayers(AW_game_instance_t *gi) {
  if(gi->game_time_step != 0) {
    AW_player_ptr p = gi->player_head;
    while(p != AW_null) {
      AW_player_t *pl = &player(p);
      PL_ProcessCmdStore(gi, p);
      CS_ClearCmdStore(gi, pl->cs);
      PL_UpdateUnits(gi, p);
      p = pl->next;
    }
    bool done;
    do {
      done = true;
      p = gi->player_head;
      while(p != AW_null) {
        done = PL_MoveAndAttack(gi, p) && done;
        p = player(p).next;
      }
    } while(!done);
    GI_UpdateMaps(gi);
  }
}

void GI_UpdateMiscs(AW_game_instance_t *gi) {
  if(gi->game_time_step != 0) {
    AW_ball_ptr b = gi->ball_head;
    while(b != AW_null) {
      AW_ball_ptr b2 = b;
      b = ball(b).next;
      if(BA_Update(gi, b2))
        BA_Free(gi, b2);
    }
    AW_smoke_ptr s = gi->smoke_head;
    while(s != AW_null) {
      AW_smoke_ptr s2 = s;
      s = smoke(s).next;
      if(SMOKE_Update(gi, s2))
        SMOKE_Free(gi, s2);
    }
    AW_blood_ptr o = gi->blood_head;
    while(o != AW_null) {
      AW_blood_ptr o2 = o;
      o = blood(o).next;
      if(BLOOD_Update(gi, o2))
        BLOOD_Free(gi, o2);
    }
    AW_floating_text_ptr ft = 
      gi->floating_text_head;
    while(ft != AW_null) {
      AW_floating_text_ptr ft2 = ft;
      ft = floating_text(ft).next;
      if(FT_Update(gi, ft2))
        FT_Free(gi, ft2);
    }
  }
}

void GI_UpdateMaps(AW_game_instance_t *gi) {
  /* heavy path map */
  DO_TIMES(MAP_SIZE) {
    AW_unit_ptr u = gi->unit_map[f];
    gi->heavy_path_map[f] = 
        u != AW_null
     && (UO_GetFrontType(gi, u) 
     == AW_unit_order_attack
     || !CAN_BE_PUSHED(&unit(u)));
  }
  /* fov map: find something to 
     optimize that (x4 would be 
     perfect) */
  DO_TIMES(game_desc.team_count) {
    AW_id_t team_id = f;
    int *fov_map = gi->fov_maps[team_id];
    DO_TIMES(RANGE_MAP_SIZE)
      fov_map[f] = 0;
    AW_player_ptr p = gi->player_head;
    while(p != AW_null) {
      AW_player_t *pl = &player(p);
      if(pl->team_id == team_id) {
        AW_unit_ptr u = pl->unit_head;
        while(u != AW_null) {
          AW_unit_t *un = &unit(u);
          int x   = un->pos_x>>RANGE_SHIFT,
              y   = un->pos_y>>RANGE_SHIFT,
              vis = VISIBILITY_RANGE(un)>>RANGE_SHIFT;
          int *fov_mapj = fov_map + (y-vis)*RANGE_MAP_SIZE_X;
          for(int j = y-vis, endj = y+vis; j <= endj; j++) {
            if(j >= 0 && j < RANGE_MAP_SIZE_Y) {
              int *fov_mapi = fov_mapj + x-vis;
              for(int i = x-vis, endi = x+vis; i <= endi; i++) {
                if(i >= 0 && i < RANGE_MAP_SIZE_X)
                  *fov_mapi = MAX(*fov_mapi, UN_GetLight(gi, u, i, j));
                fov_mapi++;
              }
            }
            fov_mapj += RANGE_MAP_SIZE_X;
          }
          u = un->pnext;
        }
      }
      p = pl->next;
    }
  }
}

void GI_UpdateClients(AW_game_instance_t *gi) {
  if(gi->client_head != AW_null) {
    AW_client_ptr c = gi->client_head;
    while(c != AW_null) {
      AW_client_t *cl = &client(c);
      CL_Update(gi, c);
      c = cl->next;
    }
  }
}

void GI_UpdateRemoteStores(AW_game_instance_t *gi) {
  AW_remote_cmd_store_ptr r = gi->remote_cmd_store_head;
  while(r != AW_null) {
    RS_Update(gi, r);
    r = remote_cmd_store(r).next;
  }
}

void GI_EndOfTurn(AW_game_instance_t *gi) {
  GI_UpdateAverWaitTime(gi, gi->wait_time);
  AW_client_ptr c = gi->client_head;
  while(c != AW_null) {
    CL_EndOfTurn(gi, c);
    c = client(c).next;
  }
  gi->frame_time_step = NORMAL_TIME_STEP;
  gi->turn_frame_count = STARTING_TURN_FRAME_COUNT;
  unsigned int hash = 0;
  AW_player_ptr p = gi->player_head;
  while(p != AW_null) {
    AW_cmd_ptr r = cmd_store(player(p).cs).cmd_head;
    while(r != AW_null) {
      AW_cmd_t *cmd = &cmd(r);
      if(cmd->type == AW_cmd_type_end_of_turn
      && cmd->turn == gi->turn+1) {
        gi->turn_frame_count  = MAX(gi->turn_frame_count, cmd->turn_frame_count);
        gi->frame_time_step   = MAX(gi->frame_time_step, cmd->frame_time_step);
        if(hash != 0 
        /* in case of the replay there is 
           two EOT, one with hash == 0 */
        && cmd->hash != 0 
        && hash != cmd->hash) {
          gi->desync = true;
          gi->game_time_step = 0;
          trace("desynced");
        }
        hash = cmd->hash;
      }
      r = cmd->cmd_next;
    }
    p = player(p).next;
  }
  gi->turn++;
  gi->turn_acc        = 0;
  gi->wait_time       = 0;
}

void GI_UpdateAverFrameTime(AW_game_instance_t *gi, AW_time_t frame_time) {
  gi->aver_frame_times[gi->aver_frame_times_counter++] = frame_time;
  if(gi->aver_frame_times_counter >= MAX_AVER_FRAME_TIMES)
    gi->aver_frame_times_counter = 0;
  gi->aver_frame_time = 0;
  DO_TIMES(MAX_AVER_FRAME_TIMES)
    gi->aver_frame_time += gi->aver_frame_times[f];
  gi->aver_frame_time /= MAX_AVER_FRAME_TIMES;
  /*DO_TIMES(MAX_AVER_FRAME_TIMES)
    gi->aver_frame_time = 
      MAX(gi->aver_frame_time, gi->aver_frame_times[f]);
  gi->aver_frame_time = MIN(gi->aver_frame_time, MAX_TIME_STEP);*/
}

void GI_UpdateAverWaitTime(AW_game_instance_t *gi, AW_time_t wait_time) {
  gi->aver_wait_times[gi->aver_wait_times_counter++] = wait_time;
  if(gi->aver_wait_times_counter >= MAX_AVER_WAIT_TIMES)
    gi->aver_wait_times_counter = 0;
  gi->aver_wait_time = 0;
  DO_TIMES(MAX_AVER_WAIT_TIMES)
    gi->aver_wait_time += gi->aver_wait_times[f];
  gi->aver_wait_time /= MAX_AVER_WAIT_TIMES;
}

void GI_UpdateMaxPing(AW_game_instance_t *gi) {
  gi->pings[gi->max_ping_times_counter++] = gi->ping;
  if(gi->max_ping_times_counter >= MAX_MAX_PING_TIMES)
    gi->max_ping_times_counter = 0;
  gi->max_ping = 0;
  DO_TIMES(MAX_MAX_PING_TIMES)
    gi->max_ping = MAX(gi->max_ping, gi->pings[f]);
}

void GI_UpdateInputs(AW_game_instance_t *gi) {
  gi->key_counter = 0;
  TCOD_event_t e;
  e = TCOD_sys_check_for_event(TCOD_EVENT_MOUSE|TCOD_EVENT_KEY, &gi->keys[gi->key_counter++], &gi->mouse);
  if(gi->key_counter >= SMALL_SIZE)
    gi->key_counter = SMALL_SIZE-1;
  gi->lbtn_up = gi->lbtn_down && !gi->mouse.lbutton;
  gi->lbtn_down = gi->mouse.lbutton;
  gi->rbtn_up = gi->rbtn_down && !gi->mouse.rbutton;
  gi->rbtn_down = gi->mouse.rbutton;
  while(e != TCOD_EVENT_NONE) {
    e = TCOD_sys_check_for_event(TCOD_EVENT_MOUSE|TCOD_EVENT_KEY, &gi->keys[gi->key_counter++], &gi->mouse);
    if(gi->key_counter >= SMALL_SIZE)
      gi->key_counter = SMALL_SIZE-1;
  }
}

bool GI_IsKeyReleased(AW_game_instance_t *gi, TCOD_keycode_t k, char c, int ctrl) {
  for(int f = 0; f < gi->key_counter; f++) {
    if(k != TCODK_CHAR
    && gi->keys[f].vk == k
    && !gi->keys[f].pressed)
      return ctrl==-1 
          || (ctrl == 1 && (gi->keys[f].lctrl || gi->keys[f].rctrl))
          || (ctrl == 0 && !gi->keys[f].lctrl && !gi->keys[f].rctrl);
    if(/*gi->keys[f].vk == TCODK_CHAR 
    &&*/k == TCODK_CHAR
    && (gi->keys[f].c == c 
    ||  gi->keys[f].c+32 == c
    ||  gi->keys[f].c-32 == c)
    && !gi->keys[f].pressed)
      return ctrl==-1 
          || (ctrl == 1 && (gi->keys[f].lctrl || gi->keys[f].rctrl))
          || (ctrl == 0 && !gi->keys[f].lctrl && !gi->keys[f].rctrl);
  }
  return false;
}

bool GI_IsKeyPressed(AW_game_instance_t *gi, TCOD_keycode_t k, char c, int ctrl) {
  for(int f = 0; f < gi->key_counter; f++) {
    if(k != TCODK_CHAR
    && gi->keys[f].vk == k
    && gi->keys[f].pressed)
      return ctrl==-1 
          || (ctrl == 1 && (gi->keys[f].lctrl || gi->keys[f].rctrl))
          || (ctrl == 0 && !gi->keys[f].lctrl && !gi->keys[f].rctrl);
    if(/*gi->keys[f].vk == TCODK_CHAR 
    &&*/k == TCODK_CHAR
    && (gi->keys[f].c == c 
    ||  gi->keys[f].c+32 == c
    ||  gi->keys[f].c-32 == c)
    && gi->keys[f].pressed)
      return ctrl==-1 
          || (ctrl == 1 && (gi->keys[f].lctrl || gi->keys[f].rctrl))
          || (ctrl == 0 && !gi->keys[f].lctrl && !gi->keys[f].rctrl);
  }
  return false;
}

bool GI_IsKeyPressed2(AW_game_instance_t *gi, TCOD_keycode_t k) {
  return TCOD_console_is_key_pressed(k);
}

void GI_StopPropagation(AW_game_instance_t *gi, TCOD_keycode_t k, char c) {
  for(int f = 0; f < gi->key_counter; f++) {
    if(k != TCODK_CHAR
    && gi->keys[f].vk == k)
      gi->keys[f].vk = (TCOD_keycode_t)0;
    if(/*gi->keys[f].vk == TCODK_CHAR 
    &&*/k == TCODK_CHAR
    && (gi->keys[f].c == c 
    ||  gi->keys[f].c+32 == c
    ||  gi->keys[f].c-32 == c))
      gi->keys[f].c = 0;
  }
}

bool GI_InState(AW_game_instance_t *gi, AW_state_t st) {
  return (gi->state & st) != 0;
}

void GI_AddState(AW_game_instance_t *gi, AW_state_t st) {
  gi->add_state[gi->add_state_count++] = st;
}

void GI_RemoveState(AW_game_instance_t *gi, AW_state_t st) {
  gi->remove_state[gi->remove_state_count++] = st;
}

void GI_UpdateState(AW_game_instance_t *gi) {
  for(int f = 0; f < gi->remove_state_count; f++)
    gi->state &= ~gi->remove_state[f];
  for(int f = 0; f < gi->add_state_count; f++)
    gi->state |= gi->add_state[f];
  gi->remove_state_count = 0;
  gi->add_state_count = 0;
}

void GI_ClearConsole(AW_game_instance_t *gi) {
  FOR_RECT(0, CON_RES_X-1, 0, CON_RES_Y-1)
    TCOD_console_put_char_ex(
      con, 
      i, 
      j, 
      0, 
      TCOD_black, 
      TCOD_black);
}

void GI_ConnectToMasterServer(AW_game_instance_t *gi) {
  GI_KillHost(gi);
  gi->address.host = ENET_HOST_ANY;
  gi->address.port = ENET_PORT_ANY;
  gi->host = enet_host_create(&gi->address, MAX_PEER, 1, 0, 0);
  assert(gi->host != null);
  gi->peer = enet_host_connect(gi->host, &gi->master_server_addr, 1, 0);
  gi->connection_acc = 0;
  gi->frame_time = 0;
  gi->connected = false;
  enet_host_flush(gi->host);
  assert(gi->peer != null);
}

void GI_DisconnectFromMasterServer(AW_game_instance_t *gi) {
  if(gi->peer) {
    enet_peer_disconnect(gi->peer, 0);
    ENetEvent e;
    enet_host_service(gi->host, &e, 500);
    enet_peer_reset(gi->peer);
    gi->peer = null;
  }
}

void GI_KillHost(AW_game_instance_t *gi) {
  GI_DisconnectFromMasterServer(gi);
  if(gi->host) {
    enet_host_destroy(gi->host);
    gi->host = null;
  }
}

bool GI_ListenMasterServer(AW_game_instance_t *gi, ENetEvent *e) {
  enet_host_flush(gi->host);
  enet_host_service(gi->host, e, 0);
}

bool GI_CheckConnectionToMasterServer(AW_game_instance_t *gi, ENetEvent *e, AW_state_t current_state) {
  switch(e->type) {
    case ENET_EVENT_TYPE_RECEIVE:
      enet_packet_destroy(e->packet);
      return true;
    case ENET_EVENT_TYPE_CONNECT:
      gi->connected = true;
      trace("GI_CheckConnectionToMasterServer: Connection to master server succeed.");
      GI_SuccessFlash(gi, "Connection to master server succeed.");
      return true;
    case ENET_EVENT_TYPE_DISCONNECT:
      gi->connected = false;
      gi->connection_acc = 0;
      trace("GI_CheckConnectionToMasterServer: disconnected from master server.");
      GI_Free(gi);
      GI_FailureFlash(gi, "Disconnected from master server.");
      GI_RemoveState(gi, current_state);
      GI_AddState(gi, AW_state_main_menu);
      return false;
    default:
      if(!gi->connected) {
        gi->connection_acc += gi->frame_time;
        if(gi->connection_acc > SHORT_CONNECTION_TIMEOUT) {
          gi->connection_acc = 0;
          trace("GI_CheckConnectionToMasterServer: cannot connect to master server.");
          GI_FailureFlash(gi, "Cannot connect to master server.");
          GI_RemoveState(gi, current_state);
          GI_AddState(gi, AW_state_main_menu);
          return false;
        }
      } 
  }
  return true;
}

bool GI_JoinGameOnMasterServer(AW_game_instance_t *gi) {
  if(gi->connected) {
    trace("Joining game: " + game_desc.game_name);
    GI_SuccessFlash(gi, "Game created successfully.");
    ENetPacket *packet =
      enet_packet_create(null, 0, ENET_PACKET_FLAG_RELIABLE);
    AW_join_game_packet_t data;
    data.type                   = AW_cmd_type_join_game;
    data.player_count_per_team  = game_desc.player_count_per_team;
    data.team_count             = game_desc.team_count;
    strcpy(data.game_name, game_desc.game_name.c_str());
    enet_packet_resize(packet, sizeof(data));
    memcpy(packet->data, &data, sizeof(data));
    enet_peer_send(gi->peer, 0, packet);
    enet_host_flush(gi->host);
  }
  return gi->connected;
}

bool GI_GameReady(AW_game_instance_t *gi) {
  ENetEvent e;
  if(enet_host_service(gi->host, &e, 0) != ENET_EVENT_TYPE_NONE) {
    switch(e.type)
      case ENET_EVENT_TYPE_RECEIVE: {
          trace("Game ready.");
          const char *data = (const char*)e.packet->data;
          int player_id = *(int*)data;
          data += sizeof(int);
          int team_id = *(int*)data;
          data += sizeof(int);
          game_desc.player_count_per_team = *(int*)data;
          data += sizeof(int);
          game_desc.team_count   = *(int*)data;
          data += sizeof(int);
          bool result = str(data) == "_READY_";
          assert(result);
          game_desc.local_player_id = player_id;
          game_desc.local_team_id   = team_id;
          return result;
        }
    GI_CheckConnectionToMasterServer(gi, &e, AW_state_waiting_players);
  }
  return false;
}

bool GI_PlayersConnected(AW_game_instance_t *gi) {
  AW_client_ptr c;
  bool  done = true,
        seeders_connected = true;
  c = gi->client_head;
  int i =0;
  while(seeders_connected
     && c != AW_null) {
    AW_client_t *cl = &client(c);
    seeders_connected = cl->host;
    i++;
    c = cl->next;
  }
  trace(i);
  if(!seeders_connected) {
    ENetEvent e;
    if(enet_host_service(gi->host, &e, 50) != ENET_EVENT_TYPE_NONE) {
      switch(e.type) {
        case ENET_EVENT_TYPE_RECEIVE: {
          trace("CL: port received.");
          int port      = ((int*)e.packet->data)[0],
              player_id = ((int*)e.packet->data)[1];
          AW_client_t *cl;
          c = gi->client_head;
          while(c != AW_null) {
            cl = &client(c);
            if(cl->player_id == player_id)
              break;
            c = cl->next;
          }
          cl->address.host          = ENET_HOST_ANY;
          cl->address.port          = 0;
          cl->host                  = enet_host_create(&cl->address, MAX_PEER, 1, 0, 0);
          enet_address_set_host(&cl->seeding_address, game_desc.master_server.c_str());
          cl->seeding_address.port  = port;
          trace("Port is " + i2a(cl->seeding_address.port));
          cl->peer                  = enet_host_connect(cl->host, &cl->seeding_address, 1, 0);
          cl->connected             = false;
          assert(cl->peer);
          enet_host_flush(cl->host);
        }
        break;
      }
      GI_CheckConnectionToMasterServer(gi, &e, AW_state_ready);
    }
  }
  c = gi->client_head;
  while(c != AW_null) {
    AW_client_t *cl = &client(c);
    CL_WaitForConnection(gi, c);
    done = cl->host 
        && cl->connected 
        && done;
    c = cl->next;
  }
  AW_remote_cmd_store_ptr r = gi->remote_cmd_store_head;
  while(r != AW_null) {
    AW_remote_cmd_store_t *rs = &remote_cmd_store(r);
    RS_WaitForConnection(gi, r); 
    done = rs->connected && done;
    r = rs->next;
  }
  if(done) {
    ENetEvent e;
    if(enet_host_service(gi->host, &e, 50) != ENET_EVENT_TYPE_NONE) {
      switch(e.type)
        case ENET_EVENT_TYPE_RECEIVE: {
            trace("Players connected.");
            trace((const char*)e.packet->data);
            bool result = str((const char*)e.packet->data) == "_PLAYERS_CONNECTED_";
            assert(result);
            return result;
          }
      GI_CheckConnectionToMasterServer(gi, &e, AW_state_ready);
    }
  }
  return false;
}

void GI_Ping(AW_game_instance_t *gi, ENetEvent *e) {
  gi->ping_acc += gi->frame_time;
  if(gi->ping_acc >= MASTER_SERVER_PING_INTERVAL) {
    gi->ping_acc = 0;
    ENetPacket *packet = 
      enet_packet_create(
        null, 
        0, 
        ENET_PACKET_FLAG_UNSEQUENCED);
    AW_ping_packet_t data;
    data.type         = AW_cmd_type_ping;
    data.time         = AW_GetTime();
    enet_packet_resize(packet, sizeof(data));
    memcpy(packet->data, &data, sizeof(data));
    enet_peer_send(gi->peer, 0, packet);
    enet_host_flush(gi->host);
  }
  switch(e->type) 
    case ENET_EVENT_TYPE_RECEIVE: {
        AW_cmd_type_t type = ((AW_cmd_type_t*)e->packet->data)[0];
        switch(type) {
          case AW_cmd_type_ping:
            AW_ping_packet_t *data = 
              (AW_ping_packet_t*)e->packet->data;
            gi->ping = AW_GetTime() - data->time;
            GI_UpdateMaxPing(gi);
            break;
        }
      }
}

void GI_SuccessFlash(AW_game_instance_t *gi, const str &msg) {
  gi->flash_btn.s = msg;
  gi->flash_btn.color = TCOD_green;
}

void GI_FailureFlash(AW_game_instance_t *gi, const str &msg) {
  gi->flash_btn.s = msg;
  gi->flash_btn.color = TCOD_red;
}

AW_player_ptr GI_GetPlayerPtr(AW_game_instance_t *gi, AW_id_t player_id) {
  AW_player_ptr p = gi->player_head;
  while(p != AW_null) {
    AW_player_t *pl = &player(p);
    if(pl->id == player_id)
      return p;
    p = pl->next;
  }
  return AW_null;
}

AW_client_ptr GI_GetHumanClient(AW_game_instance_t *gi) {
  AW_client_ptr c = gi->client_head;
  while(c != AW_null) {
    AW_client_t *cl = &client(c);
    if(cl->type == HUMAN_CLIENT)
      return c;
    c = cl->next;
  }
  return AW_null;
}

void GI_CheckListIntegrity(AW_game_instance_t *gi) {
  int i, j, k;
  AW_client_ptr c = gi->client_head;
  k = 0;
  while(c != AW_null) {
    assert((k++) <= MAX_CLIENT);
    c = client(c).next;
  }
  AW_player_ptr p = gi->player_head;
  k = 0;
  while(p != AW_null) {
    AW_cmd_ptr r = cmd_store(player(p).cs).cmd_head;
    i = 0;
    while(r != AW_null) {
      AW_cmd_t *cmd = &cmd(r);
      assert((i++) <= MAX_CMD);
      r = cmd->cmd_next;
    }
    AW_unit_ptr u = player(p).unit_head;
    i = 0;
    while(u != AW_null) {
      AW_unit_t *un = &unit(u);
      AW_unit_order_ptr o = un->order_head;
      j = 0;
      while(o != AW_null) {
        assert((j++) <= MAX_UNIT_ORDER);
        o = order(o).next;
      }
      assert((i++) <= MAX_UNIT);
      u = un->pnext;
    }
    assert((k++) <= MAX_PLAYER);
    p = player(p).next;
  }
  AW_ball_ptr b = gi->ball_head;
  k = 0;
  while(b != AW_null) {
    assert((k++) <= MAX_BALL);
    b = ball(b).next;
  }
  {
    AW_unit_ptr u = gi->free_unit_head;
    k = 0;
    while(u != AW_null) {
      assert((k++) <= MAX_UNIT);
      u = unit(u).fnext;
    }
  }
  {
    AW_player_ptr u = gi->free_player_head;
    k = 0;
    while(u != AW_null) {
      assert((k++) <= MAX_PLAYER);
      u = player(u).fnext;
    }
  }
  {
    AW_client_ptr u = gi->free_client_head;
    k = 0;
    while(u != AW_null) {
      assert((k++) <= MAX_CLIENT);
      u = client(u).fnext;
    }
  }
  {
    AW_unit_order_ptr u = gi->free_unit_order_head;
    k = 0;
    while(u != AW_null) {
      assert((k++) <= MAX_UNIT_ORDER);
      u = order(u).fnext;
    }
  }
  {
    AW_cmd_ptr u = gi->free_cmd_head;
    k = 0;
    while(u != AW_null) {
      assert((k++) <= MAX_CMD);
      u = cmd(u).fnext;
    }
  }
  {
    AW_ball_ptr u = gi->free_ball_head;
    k = 0;
    while(u != AW_null) {
      assert((k++) <= MAX_BALL);
      u = ball(u).fnext;
    }
  }
  {
    AW_cmd_store_ptr u = gi->free_cmd_store_head;
    k = 0;
    while(u != AW_null) {
      assert((k++) <= MAX_PLAYER);
      u = cmd_store(u).fnext;
    }
  }
  {
    AW_remote_cmd_store_ptr u = gi->free_remote_cmd_store_head;
    k = 0;
    while(u != AW_null) {
      assert((k++) <= MAX_PLAYER);
      u = remote_cmd_store(u).fnext;
    }
  }
}

/*
 * 
 */ 

void AW_Quit() {
  run = false;
}

void AW_SetBinDir() {
  bin_dir = str(argv[0]);
  int len = bin_dir.length();
  while(len
      && bin_dir.c_str()[len-1] != '/' 
      && bin_dir.c_str()[len-1] != '\\')
      len--;
  bin_dir.resize(len);
}

AW_time_t AW_GetTime() {
  #ifdef WIN32
    LARGE_INTEGER c;
    QueryPerformanceFrequency(&pfrequency);
    QueryPerformanceCounter(&c);
    return (unsigned int)(c.QuadPart*1000/pfrequency.QuadPart);
  #else
    return 0;
  #endif
}

int AW_Rand(unsigned int *seedp) {
  *seedp = *seedp * 1103515245 + 12345;
  return((unsigned)(*seedp/(1<<22)) % (1<<21));
}

void AW_ZErr(int ret) {
  switch(ret) {
    case Z_BUF_ERROR:
      trace("buffer error");
      break;
    case Z_NEED_DICT:
      trace("need dict");
      break;
    case Z_ERRNO:
      trace("error reading stdin");
      break;
    case Z_STREAM_ERROR:
      trace("invalid compression level");
      break;
    case Z_DATA_ERROR:
      trace("invalid or incomplete deflate data");
      break;
    case Z_MEM_ERROR:
      trace("out of memory");
      break;
    case Z_VERSION_ERROR:
      trace("zlib version mismatch!");
      break;
  }
}

unsigned int Aw_Hash(unsigned char *data, int len) {
  unsigned int hash = len, tmp;
  int rem;
  if(len > 0 && data != NULL) {
    rem = len & 3;
    len >>= 2;
    for(;len > 0; len--) {
      hash  += get16bits (data);
      tmp    = (get16bits (data+2) << 11) ^ hash;
      hash   = (hash << 16) ^ tmp;
      data  += 2*sizeof (unsigned short);
      hash  += hash >> 11;
    }
    switch(rem) {
      case 3: 
        hash += get16bits (data);
        hash ^= hash << 16;
        hash ^= ((signed char)data[sizeof (unsigned short)]) << 18;
        hash += hash >> 11;
        break;
      case 2: 
        hash += get16bits (data);
        hash ^= hash << 11;
        hash += hash >> 17;
        break;
      case 1: 
        hash += (signed char)*data;
          hash ^= hash << 10;
          hash += hash >> 1;
    }
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;
    return hash;
  }
  return 0;
}

void AW_PrintString(const str &s, int x, int y) {
  TCOD_console_print(con, x, y, s.c_str());
}

void AW_PrintString(int i, int x, int y) {
  AW_PrintString(i2a(i), x, y);
}

void AW_PrintString(float f, int x, int y) {
  AW_PrintString(f2a(f), x, y);
}

void AW_PrintDebug(const str &s) { trace(s); AW_PrintString(s, 0, CON_RES_Y-1);}
void AW_PrintDebug(int i)        { trace(i); AW_PrintString(i, 0, CON_RES_Y-1);}
void AW_PrintDebug(float f)      { trace(f); AW_PrintString(f, 0, CON_RES_Y-1);}

str f2a(double f) {
  std::ostringstream ss;
  ss << f;
  return ss.str();
}

str i2a(int i) {
  std::ostringstream ss;
  ss << i;
  return ss.str();
}

str get_date() {
  time_t      rawtime;
  struct tm  *timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  str date(asctime(timeinfo));
  date.resize(date.length()-1);
  return date.c_str();
}

void log(const char *msg) {
#if THREADS  
  EnterCriticalSection(&log_crit);
#endif  
    FILE *f = fopen((bin_dir + "log.txt").c_str(), "a+");
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    fclose(f);
    
    if(size < 10000000)
        f = fopen((bin_dir + "log.txt").c_str(), "a+");
    else
        f = fopen("log.txt", "w+");
    fprintf(f, msg);
    fclose(f);
#if THREADS    
  LeaveCriticalSection(&log_crit);
#endif
}

void trace(const str &s) {
  log(str(get_date() + " " + s + "\n").c_str());
}

void trace(int i) {
  trace(i2a(i));
}

void trace(float f) {
  trace(f2a(f));
}

void exit() {
  log("End of execution. \n\n");  
#if THREADS  
  DeleteCriticalSection(&log_crit);
#endif  
}

/*
 *
 */

int                     argc;
char                    **argv;
bool                    run;
int                     resx, resy;
str                     bin_dir;
    
AW_game_instance_t      client_gi;

LARGE_INTEGER           pfrequency;
float                   ref_time;


TCOD_console_t          con;
unsigned char           zlib_in[ZLIB_CHUNK],
                        zlib_out[ZLIB_CHUNK];

int                     CON_RES_X,
                        CON_RES_Y,
                        MINIMAP_START_X,
                        LAGOMETER_START_X;

#ifdef WIN32
  CRITICAL_SECTION      log_crit;
#endif

short                   halfes[MAX_UNIT_SIZE];

AW_game_instance_t      *renderer_gi;

/*
 * End
 */ 