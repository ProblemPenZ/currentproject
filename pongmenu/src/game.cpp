#include "game.h"



Game::Game() {
  mouse.w=mouse.h=1;
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &win, &ren); 
  SDL_SetWindowTitle(win, "Window");
  TTF_Init();
  pongX=WIDTH/2-5;pongY=HEIGHT/2;
  pY=HEIGHT/2;
  oY=HEIGHT/2;
  Gmenu();  	
}

Game::~Game() {
  TTF_Quit();
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  IMG_Quit();
  SDL_Quit();
}

void Game::Gmenu() {
    
    click=false;
    TTF_Font *font;
    font = TTF_OpenFont("res/ttfs/impact.ttf", 25);
    menu = true; 
    running = true; 
    while (menu){
        static int lastTime = 0;
        lastFrame=SDL_GetTicks();
      if(lastFrame >= (lastTime+1000)) {
        lastTime=lastFrame;
        frameCount=0;

      }

    SDL_SetRenderDrawColor(ren, 34, 42, 68, 255);
    SDL_Rect rrect;rrect.x=0;rrect.y=0;rrect.w=WIDTH;rrect.h=HEIGHT;
    SDL_Rect gamerect; gamerect.x = 50; gamerect.y = 25; gamerect.w = 300; gamerect.h = 150;
    SDL_Rect exitrect; exitrect.x = 50; exitrect.y = 200; exitrect.w = 300; exitrect.h = 150;
    SDL_RenderFillRect(ren, &rrect);
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_RenderFillRect(ren, &gamerect);
    SDL_RenderFillRect(ren, &exitrect);

    SDL_Color color;
    color.r=0;color.g=255;color.b=0;color.a=255; 
    SDL_Surface* surface = TTF_RenderText_Solid(font,"Game", color);
    SDL_Surface* surfaceb = TTF_RenderText_Solid(font,"Exit", color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
    SDL_Texture* textureb = SDL_CreateTextureFromSurface(ren, surfaceb);
    int texW = 0;
    int texH = 0;

    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = { 60, 35, texW, texH };
    SDL_Rect dstrectb = { 60, 210, texW, texH };
    SDL_FreeSurface(surface); 
    SDL_FreeSurface(surfaceb); 
    SDL_RenderCopy(ren, texture, NULL, &dstrect);  
    SDL_RenderCopy(ren, textureb, NULL, &dstrectb);

    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(textureb);

    
    if(timerFPS<(1000/60)) {
    SDL_Delay((1000/60)-timerFPS);
  }
  SDL_RenderPresent(ren);
  
  SDL_Event f;
  SDL_PollEvent(&f);
  switch (f.type) {

    case SDL_QUIT:
        menu=running=false;
        break;

    

    case SDL_MOUSEBUTTONDOWN:
        switch(f.button.button) {

            case SDL_BUTTON_LEFT:
                SDL_GetMouseState(&mousex, &mousey);
                mouse.x=mousex; mouse.y=mousey;
                click = true;
                break;
            default:
                SDL_GetMouseState(&mousex, &mousey);
                mouse.x=mousex; mouse.y=mousey;
                break;
        }   
    }
    if(SDL_HasIntersection(&mouse,&gamerect)) {
        if(click==true) {
            running=true;
            menu=false;
            loop();
            }
        } 
    if(SDL_HasIntersection(&mouse,&exitrect)) {
    if(click) {
        click=false;
        menu=false; 
        }
    }
    
    click = false; 
    
    } 
    TTF_CloseFont(font);
}


void Game::loop() {     
    click=false;   
	static int lastTime = 0;
	while(running) {

	  lastFrame=SDL_GetTicks();
	  if(lastFrame >= (lastTime+1000)) {
	    lastTime=lastFrame;
	    frameCount=0;

	  }

	  render(); 
	  input();
	  update();
	}
}

void Game::render() {
  int grey[3]; grey[0]=grey[1]=grey[2]=200;
  int nvblue[3];nvblue[0]=34;nvblue[1]=42;nvblue[2]=68;

  SDL_SetRenderDrawColor(ren, nvblue[0], nvblue[1], nvblue[2], 255);

  SDL_Rect rect;rect.x=rect.y=0;rect.w=WIDTH;rect.h=HEIGHT;  

  SDL_Rect ball;ball.x=pongX;ball.y=pongY;ball.w=10;ball.h=10;

  SDL_Rect opponent;opponent.x=WIDTH -20;opponent.y=oY;opponent.w=10;opponent.h=140;

  SDL_Rect player;player.x= 10; player.y=pY;player.w=10;player.h=140;

  
  SDL_RenderFillRect(ren, &rect);

  SDL_SetRenderDrawColor(ren, grey[0], grey[1], grey[2], 255);  

  SDL_RenderFillRect(ren, &opponent);

  SDL_RenderFillRect(ren, &player);

  drawCircle(ren, ball.x, ball.y,10);

  fillCircle(ren, ball.x, ball.y,10);

  SDL_RenderDrawLine(ren, WIDTH/2, 0, WIDTH/2, HEIGHT+10);

  if (SDL_HasIntersection(&ball, &opponent) || SDL_HasIntersection(&ball, &player)) {bSpeedx *= -1;}

  frameCount++;
  int timerFPS = SDL_GetTicks()-lastFrame;
  if(timerFPS<(1000/60)) {
    SDL_Delay((1000/60)-timerFPS);
  }

  SDL_RenderPresent(ren);
}

void Game::draw(Object o) {
	SDL_Rect dest = o.getDest();
	SDL_Rect src = o.getSource();
	SDL_RenderCopyEx(ren, o.getTex(), &src, &dest, 0, NULL, SDL_FLIP_NONE);
}


void Game::input() {
  SDL_Event e;
  while(SDL_PollEvent(&e)) {
    if(e.type == SDL_QUIT) {running=false;menu=false; cout << "Quitting" << endl;}
    if(e.type == SDL_KEYDOWN) {
      if(e.key.keysym.sym == SDLK_ESCAPE){menu=true;click=false;running=false;}
      if(e.key.keysym.sym == SDLK_s){pSpeed =7;}
      if(e.key.keysym.sym == SDLK_w){pSpeed =-7;}

      if(e.key.keysym.sym == SDLK_DOWN){oSpeed =7;}
      if(e.key.keysym.sym == SDLK_UP){oSpeed =-7;}
      

    }
    if(e.type == SDL_KEYUP) {
      if(e.key.keysym.sym == SDLK_s){pSpeed =0;}
      if(e.key.keysym.sym == SDLK_w){pSpeed = 0;}

      if(e.key.keysym.sym == SDLK_DOWN){oSpeed =0;}
      if(e.key.keysym.sym == SDLK_UP){oSpeed = 0;}
    }

    SDL_GetMouseState(&mousex, &mousey);
  }
}

void Game::update() {
  playeranim();

}

bool Game::objectCollision(Object a, Object b) {
  if((a.getDX() < (b.getDX()+b.getDW())) && ((a.getDX() + a.getDW()) > b.getDX()) 
  && (a.getDY() < (b.getDY() + b.getDH())) && ((a.getDY() + a.getDH()) > b.getDY())) {
    return true;
  } else {
    return false;
  }
}

bool Game::checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
        cout << "Collided!" << endl;

    }

    if( topA >= bottomB )
    {
        return false;
        cout << "Collided!" << endl;

    }

    if( rightA <= leftB )
    {
        return false;
        cout << "Collided!" << endl;

    }

    if( leftA >= rightB )
    {
        return false; 
        cout << "Collided!" << endl;
    }

    //If none of the sides from A are outside B
    return true;
}


void Game::drawcubeCirc(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}

void Game::drawellipseline(SDL_Renderer* r, int x0, int y0, int radiusX, int radiusY)
{
    float pi  = 3.14159265358979323846264338327950288419716939937510;
    float pih = pi / 2.0; //half of pi

    //drew  28 lines with   4x4  circle with precision of 150 0ms
    //drew 132 lines with  25x14 circle with precision of 150 0ms
    //drew 152 lines with 100x50 circle with precision of 150 3ms
    const int prec = 27; // precision value; value of 1 will draw a diamond, 27 makes pretty smooth circles.
    float theta = 0;     // angle that will be increased each loop

    //starting point
    int x  = (float)radiusX * cos(theta);//start point
    int y  = (float)radiusY * sin(theta);//start point
    int x1 = x;
    int y1 = y;

    //repeat until theta >= 90;
    float step = pih/(float)prec; // amount to add to theta each time (degrees)
    for(theta=step;  theta <= pih;  theta+=step)//step through only a 90 arc (1 quadrant)
    {
        //get new point location
        x1 = (float)radiusX * cosf(theta) + 0.5; //new point (+.5 is a quick rounding method)
        y1 = (float)radiusY * sinf(theta) + 0.5; //new point (+.5 is a quick rounding method)

        //draw line from previous point to new point, ONLY if point incremented
        if( (x != x1) || (y != y1) )//only draw if coordinate changed
        {
            SDL_RenderDrawLine(r, x0 + x, y0 - y,    x0 + x1, y0 - y1 );//quadrant TR
            SDL_RenderDrawLine(r, x0 - x, y0 - y,    x0 - x1, y0 - y1 );//quadrant TL
            SDL_RenderDrawLine(r, x0 - x, y0 + y,    x0 - x1, y0 + y1 );//quadrant BL
            SDL_RenderDrawLine(r, x0 + x, y0 + y,    x0 + x1, y0 + y1 );//quadrant BR
        }
        //save previous points
        x = x1;//save new previous point
        y = y1;//save new previous point
    }
    //arc did not finish because of rounding, so finish the arc
    if(x!=0)
    {
        x=0;
        SDL_RenderDrawLine(r, x0 + x, y0 - y,    x0 + x1, y0 - y1 );//quadrant TR
        SDL_RenderDrawLine(r, x0 - x, y0 - y,    x0 - x1, y0 - y1 );//quadrant TL
        SDL_RenderDrawLine(r, x0 - x, y0 + y,    x0 - x1, y0 + y1 );//quadrant BL
        SDL_RenderDrawLine(r, x0 + x, y0 + y,    x0 + x1, y0 + y1 );//quadrant BR
    }
}


int Game::drawCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

int Game::fillCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

void Game::playeranim(){
  pY += pSpeed;
  oY += oSpeed;
  pongX += bSpeedx;
  pongY += bSpeedy;

  if(pongX <= 10 || pongX >= WIDTH-10){pongX=WIDTH/2-5;pongY=HEIGHT/2;}
  if(pongY <= 10 || pongY >= HEIGHT-10){bSpeedy *= -1;}

  if(pY <= 0){pY = 0;}
  if(pY+130 >= HEIGHT-10){pY = HEIGHT-140;}

  if(oY <= 0){oY = 0;}
  if(oY+130 >= HEIGHT-10){oY = HEIGHT-140;}
}

