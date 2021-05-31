#include<windows.h>
#include<string.h>
#include<math.h>

#define n1 0
#define n2 3
#define n3 0
#define n4 4
#define n 10
#define half_edge 120
#define edge 240
#define height 200
#define d 15
#define beta 0.4

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
char szProgName[]="Имя программы";
    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow){
    HWND hWnd;
    MSG lpMsg;
    WNDCLASS w;
    w.lpszClassName=szProgName; //имя программы - объявлено выше
    w.hInstance=hInstance; //идентификатор текущего приложения
    w.lpfnWndProc=WndProc; //указатель на функцию окна
    w.hCursor=LoadCursor(NULL, IDC_ARROW); //загружаем курсор
    w.hIcon=0;
    w.lpszMenuName=0;
    w.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH); //цвет фона окна
    w.style=CS_HREDRAW|CS_VREDRAW;
    w.cbClsExtra=0;
    w.cbWndExtra=0;
    //Если не удалось зарегистрировать класс окна - выходим
    if(!RegisterClass(&w))
        return 0;
    //Создадим окно в памяти, заполнив аргументы CreateWindow
    hWnd=CreateWindow(szProgName, //Имя программы
                      "Грфические возможности Win32 API", //Заголовок окна
                      WS_OVERLAPPEDWINDOW, //Стиль окна - перекрывающееся
                      0, //положение окна на экране по х
                      0, //положение по у
                      1000, //ширина
                      1000, //высота
                      (HWND)NULL, //идентификатор родительского окна
    (HMENU)NULL, //идентификатор меню
    (HINSTANCE)hInstance, //идентификатор экземпляра программы
    (HINSTANCE)NULL); //отсутствие дополнительных параметров

    //Выводим окно из памяти на экран
    ShowWindow(hWnd, nCmdShow);
    //Обновим содержимое окна
    UpdateWindow(hWnd);

    //Цикл обработки сообщений

    while(GetMessage(&lpMsg, NULL, 0, 0)) {
        TranslateMessage(&lpMsg);
        DispatchMessage(&lpMsg);
    }
    return(lpMsg.wParam);
}

int random(int min, int max){
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

void random_matrix(size_t s, int A[s][s]){
    for (int i = 0; i < s; i++){
        for (int j = 0; j < s; j++){
            A[i][j] = random(0, 2);
        }
    }
}

void mulmr(double k, size_t s, int A[s][s]){
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            A[i][j] *= k;
            if(A[i][j] < 1.0){
                A[i][j] = 0;
            } else {
                A[i][j] = 1;
            }
        }
    }
}


//Функция рисования линии от точки к точке
BOOL Line(HDC hdc, int x1, int y1, int x2, int y2){
    MoveToEx(hdc, x1, y1, NULL);
    return LineTo(hdc, x2, y2);
}

//Функция дорисовки наконечника стрелок
void Arrowhead(HDC hdc, int x1, int y1, int x2, int y2){
    double fi;
    int rx, ry, lx, ly;
    if(x1 != x2){
        fi = atan((y1 - y2) / (x2 - x1));
        if (x1 < x2) {
            lx = x2 - 15 * cos(fi - 0.5);
            rx = x2 - 15 * cos(fi + 0.5);
            ly = y2 + 15 * sin(fi - 0.5);
            ry = y2 + 15 * sin(fi + 0.5);
        } else {
            lx = x2 + 15 * cos(fi - 0.5);
            rx = x2 + 15 * cos(fi + 0.5);
            ly = y2 - 15 * sin(fi - 0.5);
            ry = y2 - 15 * sin(fi + 0.5);
        }
    } else{
        lx = x2 - 15*sin(0.5);
        rx = x2 + 15*sin(0.5);
        if(y1 < y2){
            ly = y2 - 15*cos(0.5);
            ry = y2 - 15*cos(0.5);
        } else{
            ly = y2 + 15*cos(0.5);
            ry = y2 + 15*cos(0.5);
        }
    }
    Line(hdc, lx, ly, x2, y2);
    Line(hdc, x2, y2, rx, ry);
}


//Функция рисования стрелы
void Arrow(HDC hdc, int x1, int y1, int x2, int y2){
    double fi, dx, dy;
    int px, py;
    if(x1 != x2){
        if(y1 < y2) {
            fi = (atan((y1 - y2) / (x2 - x1)));
        } else{
            fi = (atan((y1 - y2) / (x1 - x2)));
        }
        dx = d * cos(fi);
        dy = d * sin(fi);
        if (x1 < x2) {
            px = x2 - dx;
            if (y1 < y2) {
                py = y2 + dy;
            } else {
                py = y2 - dy;
            }
        } else {
            px = x2 + dx;
            if (y1 < y2) {
                py = y2 - dy;
            } else {
                py = y2 + dy;
            }
        }
    } else{
        px = x2;
        if(y1 < y2){
            py = y2 - d;
        } else{
            py = y2 + d;
        }
    }
    Line(hdc, x1, y1, px, py);
    Arrowhead(hdc, x1, y1, px, py);
}

//Функция рисования ломаной стрелки
void Edge_Break(HDC hdc, int x1, int y1, int x2, int y2){
    int x, y;
    double fi, alpha, dx, dy;
    if(x1 != x2){
        if (y1 != y2){
            x = (x1 + x2) / 2;
            y = (y1 + y2) / 4;
        } else{
            x = (x1 + x2)/2;
            y = y1 + (x2 - x)*tan(0.3);
        }
        Line(hdc, x1, y1, x, y);
        Arrow(hdc, x, y, x2, y2);
    } else{
        Arrow(hdc, x1, y1, x2, y2);
    }
}

//Функция рисования ломаной стрелки
void Broken_arrow(HDC hdc, int x1, int y1, int x2, int y2){
    int x, y, dx, dy, mid_x, mid_y;
    x = fabs(x1 - x2);
    y = fabs(y1 - y2);
    if (x1 == x2){
        dy = y / 2;
        dx = tan(beta/2) * dy;
        if(y1 < y2){
            mid_y = y1 + dy;
            mid_x = x1 + dx;
        } else{
            mid_y = y1 - dy;
            mid_x = x1 - dx;
        }
    } else if (x1 != x2){
        double alpha, fi;
        alpha = atan(y / x);
        dx = x / 2;
        if (y1 != y2){
            fi = alpha + beta;
            dy = tan(fi) * dx;
            if (x1 < x2) {
                mid_x = x1 + dx;
                if (y1 > y2) {
                    mid_y = y1 - dy;
                } else {
                    mid_y = y1 + dy;
                }
            } else {
                mid_x = x1 - x / 2;
                if (y1 > y2) {
                    mid_y = y1 - dy;
                } else {
                    mid_y = y1 + dy;
                }
            }
        } else{
            dy = tan(beta/2) * dx;
            if (x1 < x2){
                mid_x = x1 + dx;
                mid_y = y1 + dy;
            } else {
                mid_x = x1 - dx;
                mid_y = y1 - dy;
            }
        }
    }
    Line(hdc, x1, y1, mid_x, mid_y);
    Arrow(hdc, mid_x, mid_y, x2, y2);
}

//Функция рисования ломаной стрелки-петли
void Loop(HDC hdc, int x0, int y0){
    int x1, y1, x2, y2, x3, y3;
    double fi;
    x1 = x0 - d*4/3;
    y1 = y0 - d*2;
    x2 = x1 + d*2;
    y2 = y1 - d;
    fi = atan(3);
    x3 = x0 + d*cos(fi);
    y3 = y0 - d*sin(fi);
    Line(hdc, x0, y0, x1, y1);
    LineTo(hdc, x2, y2);
    LineTo(hdc, x3, y3);
    Arrowhead(hdc, x2, y2, x3, y3);
}

//Функция проверки длины ребра
int edge_length(int x1, int y1, int x2, int y2){
    return ceil(sqrt(pow(x1-x2, 2) + pow(y1-y2, 2)));
}

//Функция окна
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    HDC hdc; //создаём контекст устройства
    PAINTSTRUCT ps; //создаём экземпляр структуры графического вывода
    LOGFONT lf;
    HFONT hFont;
    RECT r;
    HBRUSH hBrush;
    HPEN hPen;

    //Цикл обработки сообщений
    switch(message)
        {
        //сообщение рисования
        case WM_PAINT :
        hdc=BeginPaint(hWnd, &ps);

        int count = 0, floor = 1;
        int A_cords[10][4];
        float x_cord = 500, y_cord = 50;
        while (count != n){
        for(int i = 0; i < floor; i++){
        //Координата по x
        if(i == 0){
        A_cords[count][0] = x_cord - half_edge * (floor - 1);
        } else{
        A_cords[count][0] = A_cords[count-1][0] + edge;
        }
        //Координата по y
        A_cords[count][1] = y_cord;
        //Номер узла
        A_cords[count][2] = count + 1;
        count++;
        }
        floor++;
        y_cord += height;
        }

        double t = 1.0 - n3*0.01 - n4*0.01 - 0.3;
        srand(0304);
        int A[n][n];
        random_matrix(10, A);
        mulmr(t, 10, A);


        HPEN pen1 = CreatePen(PS_SOLID, 1, RGB(132, 56, 214));
        HPEN pen2 = CreatePen(PS_SOLID, 2, RGB(255, 82, 252));
        HPEN pen3 = CreatePen(PS_SOLID, 2, RGB(71, 214, 222));
        HPEN pen4 = CreatePen(PS_SOLID, 1, RGB(247, 143, 52));
        hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        SelectObject(hdc, hPen);

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(A[i][j] == 1){
                    if(i == j){
                        SelectObject(hdc, hPen);
                        Loop(hdc, A_cords[i][0], A_cords[i][1]);
                    } else if (A_cords[i][1] == A_cords[j][1] && fabs(A_cords[i][0]-A_cords[j][0]) != edge){
                        SelectObject(hdc, pen1);
                        Broken_arrow(hdc, A_cords[i][0], A_cords[i][1], A_cords[j][0], A_cords[j][1]);
                    } else if (A[j][i] == 1){
                        SelectObject(hdc, pen2);
                        Broken_arrow(hdc, A_cords[i][0], A_cords[i][1], A_cords[j][0], A_cords[j][1]);
                    } else if (fabs(A_cords[i][0]-A_cords[j][0]) == edge && fabs(A_cords[i][1]-A_cords[j][1]) > height){
                        SelectObject(hdc, pen3);
                        Broken_arrow(hdc, A_cords[i][0], A_cords[i][1], A_cords[j][0], A_cords[j][1]);
                    } else if (fabs(A_cords[i][0]-A_cords[j][0]) == half_edge*3 && fabs(A_cords[i][1]-A_cords[j][1]) > height){
                        SelectObject(hdc, pen4);
                        Broken_arrow(hdc, A_cords[i][0], A_cords[i][1], A_cords[j][0], A_cords[j][1]);
                    } else {
                        //normal
                        SelectObject(hdc, hPen);
                        Arrow(hdc, A_cords[i][0], A_cords[i][1], A_cords[j][0], A_cords[j][1]);
                    }
                }
            }
        }

        char *nn[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
        hBrush = CreateSolidBrush(RGB(255, 255, 255));
        hPen = CreatePen(PS_SOLID, 3, RGB(132, 56, 214));
        SelectObject(hdc, hPen);
        for(int i = 0; i < n; i++){
            Ellipse(hdc, A_cords[i][0]-d, A_cords[i][1]-d, A_cords[i][0]+d, A_cords[i][1]+d);
            TextOut(hdc, A_cords[i][0] - 5, A_cords[i][1] - 8, nn[i], 1);
        }

        ValidateRect(hWnd, NULL);
        EndPaint(hWnd, &ps);
        break;

        //сообщение выхода - разрушение окна
        case WM_DESTROY:
        PostQuitMessage(0); //Посылаем сообщение выхода с кодом 0 - нормальное завершение
        DeleteObject(hPen);
        DeleteObject(hBrush);
        break;

        default:
        return(DefWindowProc(hWnd, message, wParam, lParam)); //освобождаем очередь приложения от нераспознаных
    }
    return 0;
}
