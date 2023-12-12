/******************************************************************************

    Ejemplo del uso de la capa de dibujado (canvas) para pintura
    Archivo de Demo (Programa)

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.15.0-stable o superior
	(c) 2016 - 2023 by Cesar Rincon "NightFox"
	https://nightfoxandco.com

    Requiere GCC 11.3.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.26.3) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.20.2) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (3.0.0) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/

******************************************************************************/



/*** Includes ***/
// c++
#include <cstdio>
#include <iostream>
#include <ctime>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicializa los punteros de datos
    bg_data = NULL;
    sprite_data = NULL;
    brush_data = NULL;

    // Inicializa las listas de sprites
    bird.clear();
    bird.resize(NUMBER_OF_SPRITES);
    for (uint32_t i = 0; i < bird.capacity(); i ++) bird[i].sprite = NULL;
    brush = NULL;

    // Inicializa los punteros de objetos
    bg = NULL;
    canvas = NULL;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los sprites
    for (uint32_t i = 0; i < bird.capacity(); i ++) {
        delete bird[i].sprite;
        bird[i].sprite = NULL;
    }
    bird.clear();
    delete brush; brush = NULL;

    // Borra los objetos
    delete bg; bg = NULL;
    delete canvas; canvas = NULL;

    // Borra los datos
    delete bg_data; bg_data = NULL;
    delete sprite_data; sprite_data = NULL;
    delete brush_data; brush_data = NULL;

}



/*** Inicializa N'GINE ***/
bool Demo::Awake() {

    // Inicializa la libreria
    if (!ngn->Init()) {
        std::cout << "Critical error, can't initialize n'gine." << std::endl;
        return false;
    }

    // Habilita el archivo de registro
    ngn->log->OpenLogFile("debug_log.txt");

    // Crea la ventana con la resolucion nativa
    if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT, NGN_SCR_WINDOW, BILINEAR_FILTER, VSYNC)) return false;
    ngn->graphics->Update();

    // Visibilidad del cursor del raton
    ngn->graphics->ShowMouse(SHOW_MOUSE);

    // Contador de FPS activo?
    ngn->system->fps_counter = FPS_COUNTER;

    // Selecciona el modo grafico (ventana/full screen)
    #if defined (OS_WINDOWS)
        ngn->graphics->SetMode(SCR_MODE_WINDOWS);
    #elif defined (OS_LINUX)
        ngn->graphics->SetMode(SCR_MODE_LINUX);
    #endif
    ngn->graphics->Update();

    // Usa el archivo empaquetado de datos si es la version release
    #if defined (MODE_RELEASE)
        if(!ngn->load->SetPackage("data.pkg", "0123456789ABCDEF")) return false;
    #endif

	// Muestra la version de la libreria en la consola
	#if defined (MODE_DEBUG)
		ngn->log->Message("Versions check:\n" + ngn->system->GetVersion());
		ngn->log->Message("N'gine started successfully!");
	#endif

    // Inicializacion completada con exito
    return true;

}



/*** Al iniciar el programa ***/
bool Demo::Start() {

    // Cambia la semilla del random
    srand(time(NULL));

    // Carga los archivos
    if (!Load()) return false;

    // Crea la escena
    CreateStage();

    // Crea los sprites
    CreateSprites();

    // Procesos correctos
    return true;

}



/*** Ejecucion del programa ***/
int8_t Demo::Run() {

    // Control del loop
    int8_t loop = -1;

    while (loop < 0) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();            // Actualiza los eventos

        // Actualizacion del programa
        Update();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();

        // Control del bucle principal
        if (ngn->system->quit) {    // Si se pulsa la [X] de la ventana
            loop = 0;
        } else if (ngn->input->key_ESC->down || ngn->input->controller[0].button[10].down) {    // Si se pulsa la tecla [ESC] O se pulsa el boton XBOX
            loop = 1;
        }

    }

    // Devuelve el resultado
    return loop;

}



/*** Carga todos los archivos necesarios ***/
bool Demo::Load() {

    // Carga la imagen para el fondo
    bg_data = ngn->load->Texture("data/bg_sample.png");
    if (bg_data == NULL) return false;

    // Carga el grafico del sprite
    sprite_data = ngn->load->Sprite("data/blue_bird.spr");
    if (sprite_data == NULL) return false;

    // Carga el grafico del pincel
    brush_data = ngn->load->Sprite("data/brush.spr");
    if (brush_data == NULL) return false;

    // Carga correcta
    return true;

}



/*** Crea el escenario ***/
void Demo::CreateStage() {

    // Crea la textura de fondo
    bg = new NGN_Texture(bg_data, 0, 0);

    // Crea el pincel
    brush = new NGN_Sprite(brush_data);
    brush->frame = 0;

    // Crea el canvas
    canvas = new NGN_Canvas();
    canvas->Cls(0x000000FF);


}



/*** Crea los sprites ***/
void Demo::CreateSprites() {

    // Crea los sprites que se moveran
    float scale = 0.0f;
    float w = ((float)ngn->graphics->native_w / 2.0f);
    float h = ((float)ngn->graphics->native_h / 2.0f);
    float x = 0.0f;
    float y = 0.0f;
    for (uint32_t i = 0; i < bird.capacity(); i ++) {
        // Calcula la escala
        scale = (((((float)i + 1.0f) * 0.95f)/ (float)NUMBER_OF_SPRITES) + 0.05f);
        // Define la animacion
        bird[i].sprite = new NGN_Sprite(sprite_data, (192 + (i * 128)), 640);
        bird[i].sprite->AddAnimation("fly", 0, 7, 0, 5);
        bird[i].sprite->SetAnimation("fly");
        bird[i].sprite->frame = (i % 8);
        // Aplica la escala
        bird[i].sprite->Scale(scale);
        // Calcula los limites
        bird[i].left = w - (w * scale);
        bird[i].right = w + (w * scale);
        // Calcula las posiciones
        x = ((float)(rand() % ngn->graphics->native_w)) * scale;
        y = ((float)(rand() % ngn->graphics->native_h / 2) + (ngn->graphics->native_h / 4)) * scale;
        bird[i].sprite->position.x = (x + (w - (w * scale)));
        bird[i].sprite->position.y = (y + (h - (h * scale)));
        // Velocidades
        bird[i].speed = 5.0f * scale;
        if ((rand() % 100 > 50)) {
            bird[i].speed = -bird[i].speed;
            // Dale la vuelta al sprite
            bird[i].sprite->flip_h = true;
        }
    }

}



/*** Actualizacion del programa ***/
void Demo::Update() {

    // Mueve los sprites
    MoveSprites();
    // Dibuja en el canvas
    DrawCanvas();
    // Renderiza la escena
    Render();

}



/*** Mueve a todos los sprites ***/
void Demo::MoveSprites() {

    // Sprites en movimiento
    for (uint32_t i = 0; i < bird.capacity(); i ++) {
        // Animacion
        bird[i].sprite->PlayAnimation();
        // Movimiento
        bird[i].sprite->Translate(bird[i].speed, 0.0f);
        // Limites a derecha e izquierda
        if (bird[i].sprite->position.x > bird[i].right) {
            bird[i].sprite->position.x = bird[i].right;
            bird[i].speed = -bird[i].speed;
            bird[i].sprite->flip_h = true;
        }
        if (bird[i].sprite->position.x < bird[i].left) {
            bird[i].sprite->position.x = bird[i].left;
            bird[i].speed = -bird[i].speed;
            bird[i].sprite->flip_h = false;
        }
    }

    // Pincel
    brush->Position(ngn->input->mouse.x, ngn->input->mouse.y);


}



/*** Renderiza la escena ***/
void Demo::Render() {

    // Renderiza el fondo
    ngn->render->Texture(bg);

    // Renderiza todos los sprites
    for (uint32_t i = 0 ; i < bird.capacity(); i ++) ngn->render->Sprite(bird[i].sprite);

    // Renderiza el canvas
    ngn->render->Canvas(canvas);

    // Renderiza el pincel
    ngn->render->Sprite(brush);

}



/*** Dibuja en el canvas ***/
void Demo::DrawCanvas() {

    // Datos iniciales
    uint32_t color = 0x00000000;
    uint32_t r = 64;

    // Seleccion de pincel
    brush->frame = 0;
    if (ngn->input->key_R->held) {
        brush->frame = 1;
        color = 0xFF000080;
        r = 32;
    } else if (ngn->input->key_G->held) {
        brush->frame = 2;
        color = 0x00FF0080;
        r = 32;
    } else if (ngn->input->key_B->held) {
        brush->frame = 3;
        color = 0x0000FF80;
        r = 32;
    }

    // Al pulsar el boton del mouse
    if (ngn->input->mouse.LB.held) {
        canvas->Circle(ngn->input->mouse.x, ngn->input->mouse.y, r, color, r, true);
    }

}
