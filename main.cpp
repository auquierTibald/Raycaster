#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>
#include "assets/wall.h"
#include "assets/door.h"
#include "assets/kanye.h"
#include "assets/text.h"
#include "assets/bouton.h"

#undef main

using namespace walls;
using namespace doors;
using namespace kanye;
using namespace text;
using namespace boutons;

#define WIDTH 1200
#define HEIGHT 600
#define PI 3.1415
#define P2 PI / 2
#define P3 P2*3
#define DR 0.0174233/2;
const int mapw = 40, maph = 40, scale = 100, MAX_DEPTH = scale*mapw, tdscale = WIDTH/120, n_lvl = 1;
const float speed = scale/4;
int dOpened = 0, level = 0, keys = 0, textOpen = 0, gamestate = 1;
int  b[3] = {0, 0, 0};
float textv;
enum direction {L, R, C};
direction cdir = C;

float map[n_lvl][maph][mapw] = {
    {

    {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
    {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 4},
    {1, 0, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 4},
    {1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 4},
    {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {0, 0, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 5, 1, 1},
    {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 7, 0, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0},
    {1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0},
    {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 1, 0},
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0},
    {1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0},
    {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0}


    }
} ;

float px = 1.01*scale, py = 3.5*scale, pa = 0, pdx = cos(pa)*speed , pdy = sin(pa)*speed, pdx_l = cos(pa-P2)*speed, pdy_l = sin(pa-P2)*speed, pdx_r = cos(pa+P2)*speed, pdy_r = sin(pa+P2)*speed;

uint8_t red(uint32_t c) {
    return (c );
}
uint8_t green(uint32_t c) {
    return (c >> 8);
}
uint8_t blue(uint32_t c) {
    return (c >> 16);
}

float dist(float x, float y, float x2, float y2)
{
    return sqrt( (x2-x)*(x2-x) + (y2-y)*(y2-y) );
}

void drawPlayer2D(SDL_Renderer* r)
{   
    float ldx = cos(pa)*20, ldy = sin(pa)*20;
    SDL_Rect p = {px-5.0, py-5.0, 10, 10};
    SDL_SetRenderDrawColor(r, 185, 185, 185, 255);
    SDL_RenderFillRect(r, &p);
    SDL_SetRenderDrawColor(r, 0, 255, 255, 255);
    SDL_RenderDrawLine(r, px, py, px + ldx, py + ldy);

}

void drawMap2D(SDL_Renderer* renderer)
{
    SDL_Rect rect = {0, 0, scale-1, scale-1};
    SDL_Rect rect2 = {0, 0, scale*mapw, HEIGHT};
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(renderer, &rect2);
    for(int i = 0; i < maph;i++)
    {
        for (int j = 0; j < mapw; j++)
        {
            rect.x = j*scale + 1 ; rect.y = i*scale + 1;
            if(map[level][i][j] == 1)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
        
    }

}

void drawText(SDL_Renderer* renderer, float t)
{
    for(int i = 0;i<scale;i++)
    {
        for(int j = 0;j<scale;j++)
        {
            uint32_t c = wall_t1[(i*scale)+j];
            //SDL_Rect rect = {j*10, i*10, 10, 10};
            SDL_SetRenderDrawColor(renderer, (int)red(c), (int)green(c), (int)blue(c), 255);
            SDL_RenderDrawPoint(renderer, j, i);
        }
    }
}

void drawRays3D(SDL_Renderer* renderer)
{
    int mpx = (int)((px)/scale), mpy = (int)((py)/scale);
    float ra = pa-60*DR
    float rx, ry;
    int d = 0, mx, my, f;
    if(ra < 0) {ra += 2*PI;} if(ra > 2*PI) {ra = 0;}
    for(int ray=0;ray<120;ray++)
    {
        d=0;f=0;

        for(d=0;d<MAX_DEPTH;d++)
        {
            rx = px +cos(ra)*d; ry = py + sin(ra)*d;
            mx = (int)(rx/scale); my = (int)(ry/scale);
            int tx = abs((int)rx%scale+(int)ry%scale);     
            if(map[level][my][mx] == 1) {f=1; break;}//hit wall
            else if(map[level][my][mx] == 7 && map[level][(int)py][(int)px] != 7) {f=7; break;}//hit fake wall
            else if(map[level][my][mx] == 2 || map[level][my][mx] == 6 ) {f=2; break;}//hit door
            else if(map[level][my][mx] == 3 ) {f=3; break;}//hit kanye
            else if((int)map[level][my][mx] == 4 ) {f=4; break;}//hit text wall
            else if(map[level][my][mx] == 5 ) {f=5; break;}//hit key wall
        }
        //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        //SDL_RenderDrawLine(renderer, px, py, rx, ry);
        float disT=dist(px, py, rx, ry);

        //3D drawing
        float ca=pa-ra; if(ca<0) {ca+=2*PI;} if(ca>2*PI) {ca = 0;} disT=disT*cos(ca); //fix fisheye
        float lineH = 0;
        float ty_step;
        if(f>0){lineH = HEIGHT*40/disT; ty_step = ((float)scale-1.0) / lineH;}
        float lineO = (HEIGHT-lineH)/2;
        float vshade = scale/disT; if(vshade > 1){vshade=1;}
        float hshade = 1;

        //Ceiling
        for (int y = 0; y < (int)lineO; y++)
        {   
            SDL_Rect ceiling = {tdscale*ray, y, tdscale, 1};
            SDL_SetRenderDrawColor(renderer, 125*hshade, 125*hshade, 125*hshade, 225);
            SDL_RenderFillRect(renderer, &ceiling);
            hshade-=0.00125;
        }

        hshade=0;
        //Floor
        for (int y = HEIGHT/2; y < HEIGHT; y++)
        {   
            SDL_Rect floor = {tdscale*ray, y, tdscale, 1};
            SDL_SetRenderDrawColor(renderer, 150*hshade, 150*hshade, 150*hshade, 225);
            SDL_RenderFillRect(renderer, &floor);
            hshade+=0.00125; if(hshade>1){hshade =1;}
        }
        if(f==1)//Walls
        {
            float ty=0; //if(ra > PI) {ty-=ty_step;}
            int tx = abs((int)rx%scale+(int)ry%scale); 
            for (int y = 0; y < lineH; y++)
            {   
                uint32_t c = wall_t1[((int)ty)*scale+tx];
                SDL_Rect wall = {tdscale*ray, (int)lineO+y, tdscale, 1};
                SDL_SetRenderDrawColor(renderer, (int)red(c)*vshade, (int)green(c)*vshade, (int)blue(c)*vshade, 225);
                SDL_RenderFillRect(renderer, &wall);
                ty = y*ty_step;
            }
               
        }
        else if(f==2)//Doors
        {
            float ty=0; //if(ra > PI) {ty-=ty_step;}
            int tx = abs((int)rx%scale+(int)ry%scale); 
            for (int y = 0; y < lineH; y++)
            {   
                uint32_t c =door_t[((int)ty)*scale+tx];
                SDL_Rect wall = {tdscale*ray, (int)lineO+y, tdscale, 1};
                SDL_SetRenderDrawColor(renderer, (int)red(c)*vshade, (int)green(c)*vshade, (int)blue(c)*vshade, 225);
                SDL_RenderFillRect(renderer, &wall);
                ty = y*ty_step;
            }
        }
        else if(f==3)//Kanye
        {   
            float ty=0; //if(ra > PI) {ty-=ty_step;}
            int tx = abs((int)rx%scale+(int)ry%scale); 
            for (int y = 0; y < lineH; y++)
            {   
                uint32_t c = kanyewest[((int)ty)*scale+tx];
                SDL_Rect wall = {tdscale*ray, (int)lineO+y, tdscale, 1};
                SDL_SetRenderDrawColor(renderer, (int)red(c)*vshade, (int)green(c)*vshade, (int)blue(c)*vshade, 225);
                SDL_RenderFillRect(renderer, &wall);
                ty = y*ty_step;
            }
        }
        else if(f==4)//Text wall
        {   
            float ty=0; //if(ra > PI) {ty-=ty_step;}
            int tx = abs((int)rx%scale+(int)ry%scale); 
            for (int y = 0; y < lineH; y++)
            {   
                uint32_t c = wall_t2[((int)ty)*scale+tx];
                SDL_Rect wall = {tdscale*ray, (int)lineO+y, tdscale, 1};
                SDL_SetRenderDrawColor(renderer, (int)red(c)*vshade, (int)green(c)*vshade, (int)blue(c)*vshade, 225);
                SDL_RenderFillRect(renderer, &wall);
                ty = y*ty_step;
            }
        }
        else if(f==5)//Key wall
        {   
            float ty=0; //if(ra > PI) {ty-=ty_step;}
            int tx = abs((int)rx%scale+(int)ry%scale); 
            for (int y = 0; y < lineH; y++)
            {   
                uint32_t c = wall_t3[((int)ty)*scale+tx];
                SDL_Rect wall = {tdscale*ray, (int)lineO+y, tdscale, 1};
                SDL_SetRenderDrawColor(renderer, (int)red(c)*vshade, (int)green(c)*vshade, (int)blue(c)*vshade, 225);
                SDL_RenderFillRect(renderer, &wall);
                ty = y*ty_step;
            }
        }
        else if(f==7)//Fake walls
        {
            float ty=0; //if(ra > PI) {ty-=ty_step;}
            int tx = abs((int)rx%scale+(int)ry%scale); 
            for (int y = 0; y < lineH; y++)
            {   
                uint32_t c = wall_t1[((int)ty)*scale+tx];
                SDL_Rect wall = {tdscale*ray, (int)lineO+y, tdscale, 1};
                SDL_SetRenderDrawColor(renderer, (int)red(c)*vshade*0.9, (int)green(c)*vshade*0.9, (int)blue(c)*vshade*0.9, 255);
                SDL_RenderFillRect(renderer, &wall);
                ty = y*ty_step;
            }
        }
        ra+=DR; if(ra < 0) {ra += 2*PI;} if(ra > 2*PI) {ra = 0;}       
    }
}
const Uint8* keyboard;
void update(SDL_Renderer* r)
{
    int mpx, mpy, x, y;
    float ldx = cos(pa)*scale, ldy = sin(pa)*scale, dx;
    SDL_Event event;
    SDL_PollEvent(&event);
    SDL_PumpEvents();
    keyboard = SDL_GetKeyboardState(NULL);
        if(keyboard[SDL_SCANCODE_W])
        {
            mpx = (int)((px+pdx)/scale); mpy = (int)((py+pdy)/scale);
            if(map[level][mpy][mpx] <= 0){py += pdy; px += pdx;}
        }
        if(keyboard[SDL_SCANCODE_S])
        {
            mpx = (int)((px-pdx)/scale); mpy = (int)((py-pdy)/scale);
            if(map[level][mpy][mpx] <= 0){py -= pdy; px -= pdx;}
        }
        if(keyboard[SDL_SCANCODE_A])
        {
            mpx = (int)((px+pdx_l)/scale); mpy = (int)((py+pdy_l)/scale);
            if(map[level][mpy][mpx] <= 0){px += pdx_l ; py += pdy_l;}
        }
        if(keyboard[SDL_SCANCODE_D])
        {
            mpx = (int)((px+pdx_r)/scale); mpy = (int)((py+pdy_r)/scale);
            if(map[level][mpy][mpx] <= 0){px += pdx_r ; py += pdy_r;}
        }
        if (keyboard[SDL_SCANCODE_E])
        {
            mpx = (int)((px+ldx)/scale); mpy = (int)((py+ldy)/scale);
            if(map[level][mpy][mpx] == 2 && dOpened == 0){map[level][mpy][mpx] = -1; dOpened = 10;}
            else if((int)map[level][mpy][mpx] == 7 && dOpened == 0){map[level][mpy][mpx] = -2; dOpened = 10;}
            else if(map[level][mpy][mpx] == 6 && dOpened == 0 && keys > 0){map[level][mpy][mpx] = -1; dOpened = 10; keys--;}
            else if(map[level][mpy][mpx] == 5){map[level][mpy][mpx] = 1; keys++;}
            else if((int)map[level][mpy][mpx] == 4 ){textOpen = 10; textv = map[level][mpy][mpx];}
            else if(map[level][mpy][mpx] == -1 && dOpened == 0){map[level][mpy][mpx] = 2; dOpened = 10;}
            else if((int)map[level][mpy][mpx] == -2 && dOpened == 0){map[level][mpy][mpx] = 7; dOpened = 10;}
                
        }
        if(keyboard[SDL_SCANCODE_X])
        {
            SDL_Quit();
        }

    SDL_GetMouseState(&x, &y);
    x -= WIDTH/2;
    dx = (float)x/300.0;
    if(x > 100){cdir = R;} 
    else if(x < -100){cdir = L;} 
    else{cdir = C;}
    switch(cdir)
    {
        case L:
            pa += 0.1 *dx; if(pa < 0) {pa += 2*PI;} pdx = cos(pa)*speed ; pdy = sin(pa)*speed; pdx_l = cos(pa-P2)*speed/2; pdy_l = sin(pa-P2)*speed/2; pdx_r = cos(pa+P2)*speed/2; pdy_r = sin(pa+P2)*speed/2;
            break;
        case R:
            pa += 0.1 *dx; if(pa > 2*PI) {pa = 0;} pdx = cos(pa)*speed ; pdy = sin(pa)*speed; pdx_l = cos(pa-P2)*speed/2; pdy_l = sin(pa-P2)*speed/2; pdx_r = cos(pa+P2)*speed/2; pdy_r = sin(pa+P2)*speed/2;
            break;
    }
}

void mainMenu(SDL_Renderer* renderer)
{
    for(int y = 0;y<100;y++)
    {
        for(int x = 0;x<320;x++)
        {
            uint32_t c = bouton_jouer[y*320+x];
            SDL_Rect bouton = {x + 100, y + 100, 1, 1}; 
            if(b[0] == 0)
            {
                SDL_SetRenderDrawColor(renderer, (int)red(c), (int)green(c), (int)blue(c), 255);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, (int)red(c)/2, (int)green(c)/2, (int)blue(c)/2, 255);
            }
            SDL_RenderFillRect(renderer, &bouton);
        }
    }
}

void mainMenuUpdate(SDL_Renderer* renderer)
{
    int x , y;
    SDL_Event event;
    SDL_PollEvent(&event);
    SDL_GetMouseState(&x, &y);
    if(x > 100 && x < 420 && y > 100 && y < 200)
    {
        b[0] == 1;
    }
    else
    {
        b[0] == 0;
    }

}

int main()
{
    int mpx, mpy, ipx, ipy;
    float ldx = cos(pa)*20, ldy = sin(pa)*20;
    SDL_Window* window = SDL_CreateWindow("RAYCASTER", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Renderer* r = SDL_CreateRenderer(window, -1, 0);
    //FreeConsole();
    bool isOpen = 1;
    if(gamestate==0)
    {
        while(isOpen==1)
        {
            SDL_Event event;
            SDL_PollEvent(&event);
            if(event.type == SDL_QUIT)
            {
                SDL_Quit();
                break;
            } 
            SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
            SDL_RenderClear(r);
            mainMenu(r);
            mainMenuUpdate(r);
            SDL_RenderPresent(r);
            SDL_Delay(100);
        }
    }
    else
    {
        while(isOpen == 1)
        {
            SDL_Event event;
            SDL_PollEvent(&event);
            if(event.type == SDL_QUIT)
            {
                SDL_Quit();
                break;
            } 
            SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
            SDL_RenderClear(r);
            if(textOpen > 0)
            {
                drawText(r, textv);
                textOpen--;
            }
            drawRays3D(r);
            //drawMap2D(r);
            //drawPlayer2D(r);
            update(r);
            //ipx =(int)px; ipy = (int)py;
                if(dOpened > 0){dOpened--;}
            //std::cout<<textv<<std::endl;
            SDL_RenderPresent(r);
            SDL_Delay(100);
            /*
            if(map[level][ipy][ipx] == -11302518)
            {
                level++;
                px = 1.01*scale; py = 1.5*scale;
            }
            std::cout<<map[level][ipy][ipx]<<std::endl;
            */

    }
    }
    
    
    return 0;
}