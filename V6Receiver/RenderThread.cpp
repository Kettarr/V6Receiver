#include "RenderThread.h"
#include <chrono>
#include <iostream>
#include <libavutil/imgutils.h>
#include <thread>
#include "UDPSender.h"



void RenderThread::sendInformation(Inputs inputs) {

  
     // sizeof(std::vector<int>) + (sizeof(int) * inputs.keys.size());
    std::size_t size = sizeof(inputs);

    //std::cout << size << std::endl;

    sender.sendInputs(inputs, size);




}

void RenderThread::renderFrame(SDL_Renderer* renderer, SDL_Texture* texture)
{



    if(tQ == nullptr) {
        std::cout << "tQ is null" << std::endl;
        exit(1);
           }

          
             AVFrame* frame = tQ->front();
            
            
             if (frame == nullptr) {
                 std::cout << "frame is null" << std::endl;
                 return;
             }
           
            
            
            SDL_UpdateTexture(texture, nullptr, frame->data[0], frame->linesize[0]);
            

            if (texture != nullptr) {
               

                SDL_RenderClear(renderer);


                SDL_RenderCopy(renderer, texture, nullptr, nullptr);

               // std::cout << "still trying to render" << std::endl;
                SDL_RenderPresent(renderer);

         
            }

          
            tQ->pop();
            
            
            //av_frame_free(&frame);
       
    //}

}


void RenderThread::checkLoop() {
  
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Frame Viewer", 1000, 350, 800, 600, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        return;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 704, 576);

    bool quit = false;

    SDL_Event event;
   

    while (!quit) {

       while(SDL_PollEvent(&event))
           if (event.type == SDL_QUIT) {
               quit = true;
           }
           
           else if (event.type == SDL_KEYDOWN) {
               SDL_Keycode keyPressed = event.key.keysym.sym;
               std::cout << keyPressed << std::endl;
             
               const Uint8* keystates = SDL_GetKeyboardState(NULL);
               if (keystates[SDL_SCANCODE_W] && keystates[SDL_SCANCODE_A]) {
                   inputs.keyA = 119;
                   inputs.keyB = 97; 

               }
               if (keystates[SDL_SCANCODE_D] && keystates[SDL_SCANCODE_S]) {
                  
                   inputs.keyA = 100;
                   inputs.keyB = 115;
                 
               }
               if (keystates[SDL_SCANCODE_D] && keystates[SDL_SCANCODE_W]) {
                
                   inputs.keyA = 119;
                   inputs.keyB = 100;
                 
               }
                 
               if (keystates[SDL_SCANCODE_S] && keystates[SDL_SCANCODE_A]) {
             
                   inputs.keyA = 115;
                   inputs.keyB = 97;
                   
               }
               if (inputs.keyA == 0 && inputs.keyB == 0)
                   inputs.keyA = keyPressed;
                
           }
           else if (event.type == SDL_MOUSEBUTTONDOWN) {
               inputs.mouse = true;
               auto mouse = SDL_GetMouseState(&inputs.x, &inputs.y);
           }
      
        
          if (!inputs.keyA == 0 || inputs.mouse) {
               sendInformation(inputs);
               inputs.keyA = 0;
               inputs.keyB = 0;
               inputs.x = 0;
               inputs.y = 0;
               inputs.mouse = false;
           }

          

        tQ->mt.lock();
        if (!tQ->empty()) {
            renderFrame(renderer, texture);
        }
        tQ->mt.unlock();
        
       
       
      
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();


}