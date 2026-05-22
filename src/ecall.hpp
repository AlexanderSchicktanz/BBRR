#pragma once
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
            uint64_t offset = machine.cpu.reg(10);
            uint64_t len = machine.cpu.reg(11);

            // Get the start address of the memory buffer
            char* memory_start = (char*)machine.memory.start_address();
            if (!memory_start) {
                // Handle error: memory not accessible
                return;
            }

            // Calculate the target address
            char* target_addr = (char*)(memory_start + offset);

            // Read input from std::cin and write it to the target address
            std::string input;
            std::getline(std::cin, input);

            // Ensure we don't overflow the allocated memory
            size_t bytes_to_write = std::min(input.size(), static_cast<size_t>(len));
            std::memcpy(target_addr, input.data(), bytes_to_write);

            // Null-terminate the string if needed (optional)
            if (bytes_to_write < len) {
                target_addr[bytes_to_write] = '\0';
            }
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
            int x = (int)machine.cpu.reg(10);
            int y = (int)machine.cpu.reg(11);
            uint8_t sides = (uint8_t)machine.cpu.reg(12);
            uint8_t radius = (uint8_t)machine.cpu.reg(13);
            int r = (int)machine.cpu.reg(14);
            int g = (int)machine.cpu.reg(15);
            int b = (int)machine.cpu.reg(16);
            DrawPoly((Vector2){x,y}, sides, (float)radius, 0.0, (Color){r,g,b});
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
            if(mX>x-w/16&&mX<x+w/16+w&&mY>y-h/16&&mY<y+h/16+h&&IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                machine.cpu.reg(10)=(((float)mX-(float)x)/(float)w)*8;
                machine.cpu.reg(11)=(((float)mY-(float)y)/(float)h)*8;
                DrawLineEx((Vector2){x+w/8*machine.cpu.reg(10),y+h/8*machine.cpu.reg(11)},(Vector2){x+w/2, y+h/2},5,BLACK);
                DrawCircle(x+w/8*machine.cpu.reg(10), y+h/8*machine.cpu.reg(11), (float)((w<h)?w/4:h/4), (Color){255-r,255-g,255-b,255});
            }else{
                DrawCircle(x+w/2, y+h/2, (float)((w<h)?w/4:h/4), (Color){255-r,255-g,255-b,255});
            }
            break;
        }
        select(DPAD){
            uint64_t x = machine.cpu.reg(10);
            uint64_t y = machine.cpu.reg(11);
            uint64_t w = machine.cpu.reg(12);
            uint64_t h = machine.cpu.reg(13);
            uint64_t r = machine.cpu.reg(14);
            uint64_t g = machine.cpu.reg(15);
            uint64_t b = machine.cpu.reg(16);
            DrawRectangle(x, y, w, h, (Color){r, g, b, 255});

            uint64_t buttonSize = w / 4;
            uint64_t centerX = x + w / 2;
            uint64_t centerY = y + h / 2;

            Rectangle upButton = {centerX - buttonSize / 2, y, buttonSize, buttonSize};
            Rectangle downButton = {centerX - buttonSize / 2, y + h - buttonSize, buttonSize, buttonSize};
            Rectangle leftButton = {x, centerY - buttonSize / 2, buttonSize, buttonSize};
            Rectangle rightButton = {x + w - buttonSize, centerY - buttonSize / 2, buttonSize, buttonSize};

            int mouseX = GetMouseX();
            int mouseY = GetMouseY();
            bool mousePressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
            if (mousePressed) {
                if (CheckCollisionPointRec((Vector2){mouseX, mouseY}, upButton)) {
                    machine.cpu.reg(10) = 0;
                    DrawRectangleRec(upButton, RED);
                    DrawLineEx((Vector2){centerX, centerY}, (Vector2){centerX, y}, 5, BLACK);
                } else if (CheckCollisionPointRec((Vector2){mouseX, mouseY}, downButton)) {
                    machine.cpu.reg(10) = 1;
                    DrawRectangleRec(downButton, RED);
                    DrawLineEx((Vector2){centerX, centerY}, (Vector2){centerX, y + h}, 5, BLACK);
                } else if (CheckCollisionPointRec((Vector2){mouseX, mouseY}, leftButton)) {
                    machine.cpu.reg(10) = 2;
                    DrawRectangleRec(leftButton, RED);
                    DrawLineEx((Vector2){centerX, centerY}, (Vector2){x, centerY}, 5, BLACK);
                } else if (CheckCollisionPointRec((Vector2){mouseX, mouseY}, rightButton)) {
                    machine.cpu.reg(10) = 3;
                    DrawRectangleRec(rightButton, RED);
                    DrawLineEx((Vector2){centerX, centerY}, (Vector2){x + w, centerY}, 5, BLACK);
                }
                else machine.cpu.reg(10) = 4;
            }
            else machine.cpu.reg(10) = 4;
            DrawRectangleRec(upButton, (Color){255 - r, 255 - g, 255 - b, 255});
            DrawRectangleRec(downButton, (Color){255 - r, 255 - g, 255 - b, 255});
            DrawRectangleRec(leftButton, (Color){255 - r, 255 - g, 255 - b, 255});
            DrawRectangleRec(rightButton, (Color){255 - r, 255 - g, 255 - b, 255});

            DrawCircle(centerX, centerY, buttonSize / 2, (Color){255 - r, 255 - g, 255 - b, 255});

            break;
        }
        select(SWITCHES){
            char* memory_start = (char*)machine.memory.start_address();
            uint64_t numSwitches = machine.cpu.reg(10);
            uint64_t memPosSwitches = machine.cpu.reg(11);
            uint64_t x = machine.cpu.reg(12);
            uint64_t y = machine.cpu.reg(13);
            uint64_t w = machine.cpu.reg(14);
            uint64_t h = machine.cpu.reg(15);

            for(int i = 0; i < numSwitches; i++){
                // Draw the switch background (blue rectangle)
                DrawRectangle(x + i * w, y, w, h, BLUE);

                // Draw the white part of the switch
                DrawRectangle(x + i * w + w / 4, y + h * 2 / 5, w / 2, h * 4 / 5, WHITE);

                // Draw the black part of the switch based on its state
                if(*(memory_start + i)){
                    // Draw black switch top (switch is ON)
                    DrawRectangle(x + i * w + w / 4, y + h * 2 / 5, w / 2, h / 2, BLACK);
                } else {
                    // Draw black switch bottom (switch is OFF)
                    DrawRectangle(x + i * w + w / 4, y + h * 2 / 5 + h / 2, w / 2, h / 2, BLACK);
                }

                // Check if the mouse clicks on the switch
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                    CheckCollisionPointRec(GetMousePosition(), (Rectangle){x + i * w, y, w, h})){
                    *(memory_start + i) ^= 1; // Toggle the switch state
                    }
            }
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
