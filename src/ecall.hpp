#pragma once
#include <cstdint>
typedef enum{
    PRINTSTRING,//---Terminal---
    PRINTCHAR,
    PRINTINT,
    SCANSTRING,
    SCANCHAR,
    SCANINT,
    INITWINDOW,//---Raylib---
    RESIZEWINDOW,
    CLOSEWINDOW,
    WINDOWSHOULDCLOSE,
    SETLOGLEVEL,
    SETFPS,
    SHOWCURSOR,
    HIDECURSOR,
    SETCURSOR,
    ISCURSORHIDDEN,
    GETMOUSEINFO,
    SETMOUSEPOS,
    GETCHAR,
    GETKEY,
    BEGINDRAWING,
    ENDDRAWING,
    CLEARBACKGROUND,
    DRAWRECT,
    DRAWTRI,
    DRAWCIRCLE,
    DRAWLINE,
    DRAWPIXEL,
    DRAWTEXT,
    DRAWPOLY,
    JOYSTICK,
    DPAD,
    SWITCHES,
    LOADIMAGE,
    DRAWIMAGE,
    SIN,//---MATH---
    COS,
    TAN,
    SQRT,
    BREAKPOINT,//---OS---
    GETTIME,
    SLEEP,
    SETRAND,
    GETRAND,
    EXIT,
} ECALLCODE;
#define select(condition) case ECALLCODE::condition:
Texture2D tex;
void custom_ecall_handler(Machine<RISCV64>& machine) {
    const uint64_t ecall_num = machine.cpu.reg(17);

    //std::cout << "[ECALL] Custom ecall: " << ecall_num << std::endl;
    switch (ecall_num) {
        select(PRINTSTRING){//---Terminal---
            uint64_t addr = machine.cpu.reg(10);
            uint64_t len = machine.cpu.reg(11);
            std::cout << machine.memory.memstring(addr,len) << std::endl;
            break;
        }
        select(PRINTCHAR){
            uint64_t c = machine.cpu.reg(10);
            std::cout << (char)(c) << std::endl;
            break;
        }
        select(PRINTINT){
            int64_t i = machine.cpu.reg(10);
            std::cout << i << std::endl;
            break;
        }
        select(SCANSTRING){
            uint64_t addr = machine.cpu.reg(10);
            uint64_t len = machine.cpu.reg(11);
            //getline(std::cin,machine.memory.memstring(addr,len));
            break;
        }
        select(SCANCHAR){
            char c;
            std::cin >> c;
            machine.cpu.reg(10)=c;
            break;
        }
        select(SCANINT){
            int i;
            std::cin >> i;
            machine.cpu.reg(10) = i;
            break;
        }
        select(INITWINDOW){//---Raylib---
            uint64_t width = machine.cpu.reg(10);
            uint64_t height = machine.cpu.reg(11);
            uint64_t titleAddr = machine.cpu.reg(12);
            uint64_t titleLen = machine.cpu.reg(13);

            std::string title = machine.memory.memstring(titleAddr, titleLen);
            InitWindow(width, height, title.c_str());
            break;
        }
        select(RESIZEWINDOW){
            uint64_t width = machine.cpu.reg(10);
            uint64_t height = machine.cpu.reg(11);
            SetWindowSize(width, height);
        }
        select(CLOSEWINDOW){
            CloseWindow();
            break;
        }
        select(WINDOWSHOULDCLOSE){
            machine.cpu.reg(10) = WindowShouldClose();
            break;
        }
        select(SETLOGLEVEL){
            uint64_t level = machine.cpu.reg(10);
            SetTraceLogLevel(level);
            break;
        }
        select(SETFPS){
            uint64_t fps = machine.cpu.reg(10);
            SetTargetFPS(fps);
            break;
        }
        select(SHOWCURSOR){
            ShowCursor();
            break;
        }
        select(HIDECURSOR){
            HideCursor();
            break;
        }
        select(ISCURSORHIDDEN){
            machine.cpu.reg(10)=IsCursorHidden();
            break;
        }
        select(GETMOUSEINFO){
            SetMousePosition(machine.cpu.reg(10)=GetMouseX(),machine.cpu.reg(11)=GetMouseY());
            break;
        }
        select(SETMOUSEPOS){
            machine.cpu.reg(10);
            machine.cpu.reg(11);
            break;
        }
        select(GETCHAR){
            machine.cpu.reg(10)=GetCharPressed();
            break;
        }
        select(GETKEY){
            machine.cpu.reg(10)=GetKeyPressed();
            break;
        }
        select(BEGINDRAWING){
            BeginDrawing();
            break;
        }
        select(ENDDRAWING){
            EndDrawing();
            break;
        }
        select(CLEARBACKGROUND){
            uint64_t r = machine.cpu.reg(10);
            uint64_t g = machine.cpu.reg(11);
            uint64_t b = machine.cpu.reg(12);
            ClearBackground((Color){r,g,b,255});
        }
        select(DRAWRECT){
            uint64_t x = machine.cpu.reg(10);
            uint64_t y = machine.cpu.reg(11);
            uint64_t w = machine.cpu.reg(12);
            uint64_t h = machine.cpu.reg(13);
            uint64_t r = machine.cpu.reg(14);
            uint64_t g = machine.cpu.reg(15);
            uint64_t b = machine.cpu.reg(16);
            DrawRectangle(x, y, w, h, (Color){r,g,b,255});
            break;
        }
        select(DRAWTRI) {
            // Expect 6 coords (x1,y1,x2,y2,x3,y3) and color r,g,b in regs 10..16
            float x1 = (float)machine.cpu.reg(10);
            float y1 = (float)machine.cpu.reg(11);
            float x2 = (float)machine.cpu.reg(12);
            float y2 = (float)machine.cpu.reg(13);
            float x3 = (float)machine.cpu.reg(14);
            float y3 = (float)machine.cpu.reg(15);
            uint8_t r = (uint8_t)machine.cpu.reg(16);
            uint8_t g = 0, b = 0;
            // If more regs used, read them (safely)
            DrawTriangle((Vector2){x1,y1}, (Vector2){x2,y2}, (Vector2){x3,y3}, (Color){r,g,b,255});
            break;
        }

        select(DRAWCIRCLE) {
            int x = (int)machine.cpu.reg(10);
            int y = (int)machine.cpu.reg(11);
            int radius = (int)machine.cpu.reg(12);
            uint8_t r = (uint8_t)machine.cpu.reg(13);
            uint8_t g = (uint8_t)machine.cpu.reg(14);
            uint8_t b = (uint8_t)machine.cpu.reg(15);
            DrawCircle(x, y, (float)radius, (Color){r,g,b,255});
            break;
        }

        select(DRAWLINE) {
            int x1 = (int)machine.cpu.reg(10);
            int y1 = (int)machine.cpu.reg(11);
            int x2 = (int)machine.cpu.reg(12);
            int y2 = (int)machine.cpu.reg(13);
            uint8_t r = (uint8_t)machine.cpu.reg(14);
            uint8_t g = (uint8_t)machine.cpu.reg(15);
            uint8_t b = (uint8_t)machine.cpu.reg(16);
            DrawLine(x1, y1, x2, y2, (Color){r,g,b,255});
            break;
        }

        select(DRAWPIXEL) {
            int x = (int)machine.cpu.reg(10);
            int y = (int)machine.cpu.reg(11);
            uint8_t r = (uint8_t)machine.cpu.reg(12);
            uint8_t g = (uint8_t)machine.cpu.reg(13);
            uint8_t b = (uint8_t)machine.cpu.reg(14);
            DrawPixel(x, y, (Color){r,g,b,255});
            break;
        }

        select(DRAWTEXT) {
            uint64_t textAddr = machine.cpu.reg(10);
            uint64_t textLen = machine.cpu.reg(11);
            int x = (int)machine.cpu.reg(12);
            int y = (int)machine.cpu.reg(13);
            int fontSize = (int)machine.cpu.reg(14);
            std::string text = machine.memory.memstring(textAddr, textLen);
            DrawText(text.c_str(), x, y, fontSize, BLACK);
            break;
        }
        select(DRAWPOLY){
            break;
        }
        select(JOYSTICK){
            uint64_t x = machine.cpu.reg(10);
            uint64_t y = machine.cpu.reg(11);
            uint64_t w = machine.cpu.reg(12);
            uint64_t h = machine.cpu.reg(13);
            uint64_t r = machine.cpu.reg(14);
            uint64_t g = machine.cpu.reg(15);
            uint64_t b = machine.cpu.reg(16);
            DrawRectangle(x, y, w, h, (Color){r,g,b,255});
            int mX = GetMouseX(), mY = GetMouseY();
            if(mX>x&&mX<x+w&&mY>y&&mY<y+h&&IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                machine.cpu.reg(10)=(((float)mX-(float)x)/(float)w)*8;
            }
            break;
        }
        select(DPAD){
            break;
        }
        select(SWITCHES){
            break;
        }
        select(LOADIMAGE){
            tex = LoadTexture(machine.memory.memstring(machine.cpu.reg(10),machine.cpu.reg(11)).c_str());
            break;
        }
        select(DRAWIMAGE){
            uint64_t x = machine.cpu.reg(10);
            uint64_t y = machine.cpu.reg(11);
            DrawTexture(tex,x,y,WHITE);
            break;
        }
        select(SIN){//---MATH---
            machine.cpu.reg(10)=sin(machine.cpu.reg(10));
            break;
        }
        select(COS){
            machine.cpu.reg(10)=cos(machine.cpu.reg(10));
            break;
        }
        select(TAN){
            machine.cpu.reg(10)=tan(machine.cpu.reg(10));
            break;
        }
        select(SQRT){
            machine.cpu.reg(10)=sqrt(machine.cpu.reg(10));
            break;
        }
        select(BREAKPOINT){//---OS---
            machine.stop();
            std::cout << "BREAKPOINT" << std::endl;
            while(std::cin.get()!='\n');
            break;
        }
        select(GETTIME){
            struct timespec res;
            clock_gettime(CLOCK_REALTIME,&res);
            machine.cpu.reg(10) = res.tv_nsec;
            break;
        }
        select(SLEEP){
            struct timespec remaining, request = {0, machine.cpu.reg(10)};
            int response = nanosleep(&request, &remaining);
            machine.cpu.reg(10)=remaining.tv_nsec;
        }
        select(SETRAND){
            uint64_t max = machine.cpu.reg(10);
            srand(max);
            break;
        }
        select(GETRAND){
            machine.cpu.reg(10)=rand();
            break;
        }
        select(EXIT){
            uint64_t code = machine.cpu.reg(10);
            std::cout << "[CUSTOM] Exit with code: " << code << std::endl;
            machine.stop();
            break;
        }
        default:
            std::cout << "[ECALL] Unknown custom ecall: " << ecall_num << std::endl;
            break;
    }
}
