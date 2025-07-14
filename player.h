#ifndef PLAYER_H
#define PLAYER_H
#include <math.h>
#include "settings.h"
#define PI 3.14159265358979323846

#undef main


namespace Player {

class Player {
public:

    float x, y, speed, angle, zOffset = 0;
    bool attacking = false, animation_trigger = false, alive = true;
    double animation_time = 60, animation_time_prev = (double)SDL_GetTicks();
    SDL_Texture* weaponanimation[4];
    SDL_Texture* currentweaponframe;
    int damage = 2, mapx, mapy;
    float health;
    int camX = 0, camY = 0, camSpeed = 10, interactionsCooldown = 0;
    SDL_Renderer *renderer;
    SDL_Window *window;
    Player( SDL_Renderer *renderer, SDL_Window *window) {
        x = Settings::px;
        y = Settings::py;
        speed = Settings::ps;
        angle = Settings::pa; // Convert angle to radians
        health = 100; // Initial health
        this->renderer = renderer;
        this->window = window;
        mapx = (int)x;
        mapy = (int)y;
    }

    void move(double deltaTime, int map[Settings::mapHeight][Settings::mapWidth]) {
        float sin_a = sin(angle);
        float cos_a = cos(angle);
        float cos_a2 = cos(angle + M_PI / 2);
        float sin_a2 = sin(angle + M_PI / 2);
        float dx = 0, dy = 0;
        float speedx = this->speed * cos_a * deltaTime;
        float speedy = this->speed * sin_a * deltaTime;

        const Uint8* state = SDL_GetKeyboardState(NULL);
        // Movement controls
        if (state[SDL_SCANCODE_W]) {
            dx += speedx;
            dy += speedy;
        }
        if (state[SDL_SCANCODE_S]) {
            dx -= speedx;
            dy -= speedy;
        }
        if (state[SDL_SCANCODE_A]) {
            dx -= speed * cos_a2 * deltaTime;
            dy -= speed * sin_a2 * deltaTime;
        }
        if (state[SDL_SCANCODE_D]) {
            dx += Settings::pss * cos_a2 * deltaTime;
            dy += Settings::pss * sin_a2 * deltaTime;
        }
        /*
        if (state[SDL_SCANCODE_UP]) {
            this->z += Settings::zspeed * deltaTime;
        }

        if (state[SDL_SCANCODE_DOWN]) {
            this->z -= Settings::zspeed * deltaTime;
        }
        */
        float scale = Settings::psize / deltaTime;
        //Interaction controls
        if(state[SDL_SCANCODE_E]) {
            int tile = map[(int)(y+ dy * scale)][(int)(x + dx * scale)];
            if(tile == 2 && interactionsCooldown == 0)
            {
                map[(int)(y+ dy * scale)][(int)(x + dx * scale)] = -1;
                interactionsCooldown = 30;
            }
            else if(tile == -1  && interactionsCooldown == 0)
            {
                map[(int)(y+ dy * scale)][(int)(x + dx * scale)] = 2;
                interactionsCooldown = 30;
            }
        }
        //Running controls
        if(state[SDL_SCANCODE_LSHIFT]) {
                speed = Settings::pruns;
        }
        else {
            speed = Settings::ps; // Reset to normal speed
        }
        //Collisions
        if(map[(int)mapy][(int)(x + dx * scale)] == 0 || map[(int)mapy][(int)(x + dx * scale)] == -1)
        {
            x += dx;
        }
        if(map[(int)(y + dy * scale)][(int)mapx] == 0 || map[(int)(y + dy * scale)][(int)mapx] == -1)
        {
            y += dy;
        }

        if(state[SDL_SCANCODE_UP])
        {
            camY += camSpeed;
        }
        if(state[SDL_SCANCODE_DOWN])
        {
            camY -= camSpeed;
        }
        if(state[SDL_SCANCODE_LEFT])
        {
            camX += camSpeed;
        }
        if(state[SDL_SCANCODE_RIGHT])
        {
            camX -= camSpeed;
        }

        //std::cout<<"player angle : "<< angle <<std::endl;
        if(angle > 2*PI) {angle  = 0.01;} 
        if(angle < 0) {angle += 2*PI - 0.01;}


    }

    void animate()
    {
        if(animation_trigger && attacking)
        {
            int i;
            for(i = 0; i < 4; i++)
            {
                if(currentweaponframe == weaponanimation[i])
                {
                    break;
                }
            }
            if(i != 3)
            {
                currentweaponframe = weaponanimation[i+1];
            }
            else if (i == 3)
            {
                currentweaponframe = weaponanimation[0];
                attacking = false;
            }
        }
    }

    void drawWeapon()
        {
            animation_trigger = false;
            double time_now = (double)SDL_GetTicks();
            if (time_now - animation_time_prev > animation_time)
            {
                animation_time_prev = time_now;
                animation_trigger = true;
            }
            this->animate();
            int w;
            int h;
            SDL_QueryTexture(currentweaponframe, NULL, NULL, &w, &h);
            SDL_Rect rect = { 300, (int)(Settings::HEIGHT - h*1.5) + 10, (int)(w*1.5), (int)(h*1.5)};
            SDL_RenderCopy(renderer, currentweaponframe, NULL, &rect);
        }

    void mouse_control(float deltaTime){
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        if(mouseX < 5) {
            SDL_WarpMouseInWindow(window, Settings::WIDTH  - 5, mouseY);
        }
        if(mouseX > Settings::WIDTH - 5) {
            SDL_WarpMouseInWindow(window, 5, mouseY);
        }
        if(mouseY < 5) {
            SDL_WarpMouseInWindow(window, mouseX, 5);
        }
        if(mouseY > Settings::HEIGHT - 5) {
            SDL_WarpMouseInWindow(window, mouseX, Settings::HEIGHT - 5);
        }

        int mouseRelX = mouseX - Settings::HALF_WIDTH;
        this->zOffset = mouseY - Settings::HALF_HEIGHT;
        angle = (mouseX * (360*Settings::DR) / Settings::WIDTH);
    }

    void drawGUI()
    {
        SDL_Rect healthbar = {5, 5, 200, 20};
        SDL_Rect manabar = {5, 30, 200, 20};
        SDL_Rect life = {5, 5, (int)(health*2), 20};

        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &healthbar);

        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &manabar);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &life);

    }

    void draw() {
        float sin_a = sin(angle);
        float cos_a = cos(angle);
        //std::cout << "Drawing player at (" << x << ", " << y << ") with angle " << angle << " and cos : " << cos_a << ", sin : " << sin_a << std::endl;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, this->camX + (int)(x*Settings::SCALE), this->camY + (int)(y*Settings::SCALE), this->camX + (int)(x*Settings::SCALE) + (int)(20 * cos(angle)), this->camY + (int)(y*Settings::SCALE) + (int)(20 * sin(angle)));
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for player
        SDL_Rect rect = { this->camX + (int)(x*Settings::SCALE)-10, this->camY + (int)(y*Settings::SCALE)-10, 20, 20 };
        SDL_RenderFillRect(renderer, &rect);
    }

    void update(double deltaTime, int map[Settings::mapHeight][Settings::mapWidth]) {
        if(interactionsCooldown > 0) {interactionsCooldown -= 1;}
        mapx = (int)x;
        mapy = (int)y;
        this->move(deltaTime, map);
        this->mouse_control(deltaTime);
        if(health <= 0) {alive = false;}
        else if(health < 100) {health +=0.1;}

    }

};

}

#endif // PLAYER_H